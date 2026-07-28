#include "audio.hpp"
//
#include <SDL3_mixer/SDL_mixer.h>
#include <span>
#include "engine/coords/transforms.hpp"
#include "engine/debug/logger.hpp"
#include "engine/io/io.hpp"
#include "game/player/camera.hpp"

constexpr float BASE_CAMERA_ALTITUDE = 10.0f;
constexpr Sint64 FADING_FRAME_COUNT = 2;
static std::string SDL_MIXER_ERROR = "SDL_Mixer error. ";
static debug::Logger logger("audio");

static void freePool(std::span<MIX_Track*> trackPool) {
    for (MIX_Track* track : trackPool) {
        MIX_DestroyTrack(track);
    }
}

static void fillPool(std::span<MIX_Track*> trackPool, MIX_Mixer* mixer) {
    for (auto& track : trackPool) {
        track = nullptr;
        track = MIX_CreateTrack(mixer);
        if (!track)
            throw std::runtime_error(SDL_MIXER_ERROR + SDL_GetError());
    }
}

Audio::Audio() {
    if (!MIX_Init())
        throw std::runtime_error(SDL_MIXER_ERROR + SDL_GetError());
    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (!mixer) {
        logger.error() << SDL_MIXER_ERROR + SDL_GetError();
        return;
    }
    fillPool(worldTrackPool, mixer);
    fillPool(uiTrackPool, mixer);
    fillPool(musicTrackPool, mixer);

    for (auto& state : loopTrackPool) {
        state.track = MIX_CreateTrack(mixer);
        if (!state.track)
            throw std::runtime_error(SDL_MIXER_ERROR + SDL_GetError());
    }

    SDL_AudioSpec spec;
    if (MIX_GetMixerFormat(mixer, &spec))
        sampleRate = spec.freq;
}

Audio::~Audio() {
    for (auto& [name, sound] : sounds) {
        MIX_DestroyAudio(sound.audio);
    }
    freePool(worldTrackPool);
    freePool(uiTrackPool);
    freePool(musicTrackPool);
    MIX_DestroyMixer(mixer);
    MIX_Quit();
}

void Audio::loadSound(const std::string& name, const std::filesystem::path& path) {
    if (sounds.contains(name)) {
        logger.warning() << "Track already exists." << path.filename().string();
        return;
    }
    std::string blob = io::readFile(path, io::Log::only_error);
    SDL_IOStream* stream = SDL_IOFromConstMem(blob.data(), blob.size());
    MIX_Audio* audio = MIX_LoadAudio_IO(mixer, stream, false, true);
    if (!audio) {
        logger.error() << "Failed to load audio: " << name << ". " << SDL_GetError();
        return;
    }
    const Sint64 frames = MIX_GetAudioDuration(audio);
    const Sint64 durationFrames = (frames > 0) ? frames : 1;
    sounds[name] = Sound(audio, durationFrames);
}

static MIX_Track* findFreeTrack(std::span<MIX_Track*> trackPool) {
    for (MIX_Track* track : trackPool) {
        if (!MIX_TrackPlaying(track)) {
            if (SDL_AudioStream* stream = MIX_GetTrackAudioStream(track))
                SDL_SetAudioStreamFrequencyRatio(stream, 1.0f);
            return track;
        }
    }
    return nullptr;
}

static void playMono(MIX_Audio* audio, MIX_Track* track) {
    MIX_SetTrackAudio(track, audio);
    MIX_PlayTrack(track, 0);
}

static constexpr PixelCoord applyGuardZone(const PixelCoord position) {
    constexpr PixelCoord GUARD_ZONE = PixelCoord(80.f, 80.f) / t1::TILE;
    return t1::contains(GUARD_ZONE * -1.f, GUARD_ZONE, position) ? PixelCoord(0.f, 0.f) : position;
}

static MIX_Point3D calculate3DPosition(const PixelCoord object, const Camera& camera) {
    const PixelCoord delta = applyGuardZone(object - camera.getRealCenter()) / t1::TILE;
    const float altitude = BASE_CAMERA_ALTITUDE / camera.getMapScale();
    return MIX_Point3D(delta.x, -delta.y, -altitude);
}

void Audio::playDiegetic(const std::string& name, const PixelCoord object, const Camera& camera,
    const float gainFactor, const float pitch) {
    MIX_Track* track = findFreeTrack(worldTrackPool);
    if (!track)
        return;
    const MIX_Point3D point3D = calculate3DPosition(object, camera);
    const float finalGain = masterVolume * worldVolume * gainFactor;
    MIX_SetTrack3DPosition(track, &point3D);
    MIX_SetTrackGain(track, finalGain);
    MIX_SetTrackAudio(track, sounds[name].audio);
    SDL_AudioStream* stream = MIX_GetTrackAudioStream(track);
    if (stream)
        SDL_SetAudioStreamFrequencyRatio(stream, pitch);
    MIX_PlayTrack(track, 0);
}

void Audio::playLoopDiegetic(const std::string& name, const PixelCoord object, const Camera& camera,
    const float gainFactor, const int64_t globalTimeMs) {
    const Sound& resource = sounds[name];
    const MIX_Point3D point3D = calculate3DPosition(object, camera);
    const float finalGain = masterVolume * worldVolume * gainFactor;

    for (auto& state : loopTrackPool) {
        if (state.currentSoundName == name && MIX_TrackPlaying(state.track)) {
            MIX_SetTrack3DPosition(state.track, &point3D);
            MIX_SetTrackGain(state.track, finalGain);
            state.updatedThisFrame = true;
            return;
        }
    }

    for (auto& state : loopTrackPool) {
        if (!MIX_TrackPlaying(state.track)) {
            state.currentSoundName = name;
            state.updatedThisFrame = true;
            MIX_SetTrack3DPosition(state.track, &point3D);
            MIX_SetTrackGain(state.track, finalGain);
            MIX_SetTrackAudio(state.track, resource.audio);
            MIX_SetTrackLoops(state.track, -1);
            MIX_PlayTrack(state.track, 0);
            constexpr int64_t MS_PER_SECOND = 1000;
            const int64_t globalFrames = globalTimeMs * sampleRate / MS_PER_SECOND;
            const int64_t startFrame = globalFrames % resource.durationFrames;
            MIX_SetTrackPlaybackPosition(state.track, startFrame);
            return;
        }
    }
}

void Audio::endFrame() {
    for (auto& state : loopTrackPool) {
        if (MIX_TrackPlaying(state.track) && !state.updatedThisFrame) {
            MIX_StopTrack(state.track, FADING_FRAME_COUNT);
            state.currentSoundName.clear();
        }
        state.updatedThisFrame = false;
    }
}

void Audio::playUI(const std::string& id) {
    MIX_Track* track = findFreeTrack(uiTrackPool);
    if (track)
        playMono(sounds[id].audio, track);
}

void Audio::playMusic(const std::string& id) {
    MIX_Track* track = findFreeTrack(musicTrackPool);
    if (track)
        playMono(sounds[id].audio, track);
}

void Audio::pauseWorldSounds() {
    for (MIX_Track* track : worldTrackPool) {
        MIX_PauseTrack(track);
    }
}

void Audio::resumeWorldSounds() {
    for (MIX_Track* track : worldTrackPool) {
        MIX_ResumeTrack(track);
    }
}

void Audio::clearWorldSounds() {
    for (MIX_Track* track : worldTrackPool) {
        MIX_StopTrack(track, FADING_FRAME_COUNT);
    }
}

static void updatePoolVolume(std::span<MIX_Track*> trackPool, const float volume) {
    for (MIX_Track* track : trackPool) {
        MIX_SetTrackGain(track, volume);
    }
}

void Audio::updateVolume() {
    updatePoolVolume(worldTrackPool, masterVolume * worldVolume);
    updatePoolVolume(uiTrackPool,    masterVolume *    uiVolume);
    updatePoolVolume(musicTrackPool, masterVolume * musicVolume);
}
