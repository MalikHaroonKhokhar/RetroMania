#pragma once

#include "ecs/ECS.h"
#include "audio/AudioManager.h"
#include <algorithm>

namespace Forge {

class AudioSystem {
public:
    static void Update(Registry& registry) {
        auto view = registry.View<AudioSource>();

        for (Entity e : view) {
            auto& source = registry.GetComponent<AudioSource>(e);

            if (!source.Initialized && source.PlayOnAwake && source.Clip && source.Clip->GetNativeChunk()) {
                int loops = source.Loop ? -1 : 0;

                // Clamp volume
                float vol = std::clamp(source.Volume, 0.0f, 1.0f);
                Mix_VolumeChunk(source.Clip->GetNativeChunk(), (int)(vol * MIX_MAX_VOLUME));

                int channel = Mix_PlayChannel(-1, source.Clip->GetNativeChunk(), loops);
                if (channel != -1) {
                    source.Channel = channel;
                    source.Initialized = true;
                } else {
                    source.Channel = -1;
                    source.Initialized = false;
                    FORGE_LOG_ERROR("Failed to play audio chunk: ", Mix_GetError());
                }
            }
        }
    }
};

} // namespace Forge