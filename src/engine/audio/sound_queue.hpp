#pragma once
#include <vector>
#include "audio.hpp"

class SoundQueue {
    std::vector<std::pair<const std::string, const PixelCoord>> queue;
public:
    SoundQueue() = default;
    //
    void pushSound(const std::string& id, const PixelCoord position) {
        for (auto& [existingID, _positon] : queue) {
            if (existingID == id)
                return;
        }
        queue.emplace_back(id, position);
    }
    void play(Audio& audio, const Camera& camera) {
        for (auto& [id, position] : queue) {
            audio.playDiegetic(id, position, camera);
        }
        queue.clear();
    }
};
