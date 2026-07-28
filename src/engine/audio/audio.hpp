#pragma once
#include <array>
#include <filesystem>
#include <string>
#include <unordered_map>
#include "engine/coords/pixel_coord.hpp"

struct MIX_Audio;
struct MIX_Mixer;
struct MIX_Track;
class Camera;

struct LoopTrackState {
    MIX_Track* track = nullptr;
    std::string currentSoundName;
    bool updatedThisFrame = false;
};

class Audio {
    MIX_Mixer* mixer = nullptr;
    std::unordered_map<std::string, MIX_Audio*> sounds;
    std::array<MIX_Track*, 64> worldTrackPool;
    std::array<MIX_Track*, 4>     uiTrackPool;
    std::array<MIX_Track*, 2>  musicTrackPool;
    std::array<LoopTrackState, 16> loopTrackPool;
    //
    float masterVolume = 1.0f;
    float  worldVolume = 1.0f;
    float     uiVolume = 1.0f;
    float  musicVolume = 1.0f;
public:
    Audio();
    ~Audio();
    void loadSound(const std::string& name, const std::filesystem::path& path);
    void playDiegetic(const std::string& name, const PixelCoord object, const Camera& camera,
        float gainFactor = 1.0f, float pitch = 1.0f);
    void playLoopDiegetic(const std::string& name, const PixelCoord object, const Camera& camera, float gainFactor = 1.0f);
    void endFrame();
    //
    void playMusic(const std::string& name);
    void playUI(const std::string& name);
    //
    void pauseWorldSounds();
    void resumeWorldSounds();
    void clearWorldSounds();
    //
    void setMasterVolume(const float volume) noexcept { masterVolume = volume; }
    void setWorldVolume(const float volume)  noexcept { worldVolume = volume; }
    void setUIVolume(const float volume)     noexcept { uiVolume = volume; }
    void setMusicVolume(const float volume)  noexcept { musicVolume = volume; }
    void updateVolume();
};
