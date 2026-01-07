#pragma once
#include <array>
#include <filesystem>
#include <span>
#include <string>
#include <unordered_map>
#include "engine/coords/pixel_coord.hpp"

struct MIX_Audio;
struct MIX_Mixer;
struct MIX_Track;
class Camera;

class Audio {
    MIX_Mixer* mixer = nullptr;
    std::unordered_map<std::string, MIX_Audio*> audioCache;
    std::array<MIX_Track*, 64> worldTrackPool;
    std::array<MIX_Track*, 4>     uiTrackPool;
    std::array<MIX_Track*, 2>  musicTrackPool;
    //
    float masterVolume = 1.0f;
    float  worldVolume = 1.0f;
    float     uiVolume = 1.0f;
    float  musicVolume = 1.0f;
public:
    Audio();
    ~Audio();
    void loadSound(const std::string& id, const std::filesystem::path& path);
    void playDiegetic(const std::string& id, const PixelCoord object, const Camera& camera);
    void playMusic(const std::string& id);
    void playUI(const std::string& id);
    //
    void pauseWorldSounds();
    void resumeWorldSounds();
    void clearWorldSounds();
    //
    void setMasterVolume(const float volume) noexcept { masterVolume = volume; }
    void setWorldVolume(const float volume)  noexcept {  worldVolume = volume; }
    void setUIVolume(const float volume)     noexcept {     uiVolume = volume; }
    void setMusicVolume(const float volume)  noexcept {  musicVolume = volume; }
    void updateVolume();
};
