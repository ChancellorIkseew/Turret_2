#include "audio.hpp"
//
#include <SDL3_mixer/SDL_mixer.h>
#include <span>
#include <stdexcept>
#include "engine/coords/transforms.hpp"
#include "engine/debug/logger.hpp"
#include "engine/io/folders.hpp"
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
    if (!mixer)
        throw std::runtime_error(SDL_MIXER_ERROR + SDL_GetError());
    fillPool(worldTrackPool, mixer);
    fillPool(uiTrackPool, mixer);
    fillPool(musicTrackPool, mixer);
}

Audio::~Audio() {
    for (auto& [id, audio] : audioCache) {
        MIX_DestroyAudio(audio);
    }
    freePool(worldTrackPool);
    freePool(uiTrackPool);
    freePool(musicTrackPool);
    MIX_DestroyMixer(mixer);
    MIX_Quit();
}

void Audio::loadSound(const std::string& id, const std::filesystem::path& path) {
    if (!io::folders::fileExists(path)) {
        logger.error() << "Sound file does not exist: " << path;
        return;
    }
    if (audioCache.contains(id)) {
        logger.warning() << "Track already exists." << path.filename().string();
        return;
    }
    MIX_Audio* audio = MIX_LoadAudio(mixer, path.string().c_str(), false);
    if (!audio)
        logger.error() << "Failed to load audio: " << id << ". " << SDL_GetError();
    audioCache[id] = audio; // nullptr if error.
}

static MIX_Track* findFreeTrack(std::span<MIX_Track*> trackPool) {
    for (MIX_Track* track : trackPool) {
        if (!MIX_TrackPlaying(track))
            return track;
    }
    return nullptr;
}

static void play(MIX_Audio* audio, MIX_Track* track, const MIX_Point3D* point3D) {
    MIX_SetTrack3DPosition(track, point3D);
    MIX_SetTrackAudio(track, audio);
    MIX_PlayTrack(track, 0);
}

void Audio::playDiegetic(const std::string& id, const PixelCoord object, const Camera& camera) {
    MIX_Track* track = findFreeTrack(worldTrackPool);
    if (!track)
        return;
    const PixelCoord delta = (object - camera.getCenter()) / t1::TILE;
    const float altitude = BASE_CAMERA_ALTITUDE / camera.getMapScale();
    MIX_Point3D point3D(delta.x, -delta.y, -altitude); // Why -y, -z? See MIX_Point3D comments.
    play(audioCache[id], track, &point3D);
}

void Audio::playUI(const std::string& id) {
    MIX_Track* track = findFreeTrack(uiTrackPool);
    if (track)
        play(audioCache[id], track, nullptr);
}

void Audio::playMusic(const std::string& id) {
    MIX_Track* track = findFreeTrack(musicTrackPool);
    if (track)
        play(audioCache[id], track, nullptr);
}

void Audio::pauseWorldSounds() {
    for (MIX_Track* track : worldTrackPool) {
        if (MIX_TrackPlaying(track))
            MIX_PauseTrack(track);
    }
}

void Audio::resumeWorldSounds() {
    for (MIX_Track* track : worldTrackPool) {
        if (MIX_TrackPaused(track))
            MIX_ResumeTrack(track);
    }
}

void Audio::clearWorldSounds() {
    for (MIX_Track* track : worldTrackPool) {
        if (MIX_TrackPlaying(track))
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
