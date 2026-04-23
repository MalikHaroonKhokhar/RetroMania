#pragma once

#include <SDL_mixer.h>
#include <string>
#include "core/ResourceManager.h"
#include "core/Log.h"

namespace Forge {

/**
 * @brief SDL2_mixer Chunk wrapper resource for short sound effects.
 */
class SoundEffect : public Resource {
public:
    SoundEffect(const std::string& path) {
        m_Chunk = Mix_LoadWAV(path.c_str());
        if (!m_Chunk) {
            FORGE_LOG_ERROR("Failed to load sound effect: ", path, " - Error: ", Mix_GetError());
        }
    }

    ~SoundEffect() {
        if (m_Chunk) {
            Mix_FreeChunk(m_Chunk);
        }
    }

    SoundEffect(const SoundEffect&) = delete;
    SoundEffect& operator=(const SoundEffect&) = delete;
    SoundEffect(SoundEffect&& other) noexcept : m_Chunk(other.m_Chunk) {
        other.m_Chunk = nullptr;
    }
    SoundEffect& operator=(SoundEffect&& other) noexcept {
        if (this != &other) {
            if (m_Chunk) {
                Mix_FreeChunk(m_Chunk);
            }
            m_Chunk = other.m_Chunk;
            other.m_Chunk = nullptr;
        }
        return *this;
    }

    Mix_Chunk* GetNativeChunk() const { return m_Chunk; }

private:
    Mix_Chunk* m_Chunk = nullptr;
};

/**
 * @brief SDL2_mixer Music wrapper resource for longer music tracks.
 */
class Music : public Resource {
public:
    Music(const std::string& path) {
        m_Music = Mix_LoadMUS(path.c_str());
        if (!m_Music) {
            FORGE_LOG_ERROR("Failed to load music: ", path, " - Error: ", Mix_GetError());
        }
    }

    ~Music() {
        if (m_Music) {
            Mix_FreeMusic(m_Music);
        }
    }

    Music(const Music&) = delete;
    Music& operator=(const Music&) = delete;
    Music(Music&& other) noexcept : m_Music(other.m_Music) {
        other.m_Music = nullptr;
    }
    Music& operator=(Music&& other) noexcept {
        if (this != &other) {
            if (m_Music) {
                Mix_FreeMusic(m_Music);
            }
            m_Music = other.m_Music;
            other.m_Music = nullptr;
        }
        return *this;
    }

    Mix_Music* GetNativeMusic() const { return m_Music; }

private:
    Mix_Music* m_Music = nullptr;
};

/**
 * @brief Global Audio Manager to handle initialization and global volume.
 */
class AudioManager {
public:
    static void Init() {
        FORGE_LOG_INFO("Initializing Audio Manager");

        // Initialize SDL_mixer
        int flags = MIX_INIT_MP3 | MIX_INIT_OGG;
        if ((Mix_Init(flags) & flags) != flags) {
            FORGE_LOG_ERROR("Failed to initialize SDL_mixer: ", Mix_GetError());
        }

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            FORGE_LOG_ERROR("SDL_mixer could not initialize! SDL_mixer Error: ", Mix_GetError());
        }
    }

    static void Shutdown() {
        Mix_CloseAudio();
        Mix_Quit();
    }

    static std::shared_ptr<SoundEffect> LoadSoundEffect(const std::string& path, const std::string& name) {
        std::string key = "SFX_" + name;
        if (auto cached = ResourceManager::TryGet<SoundEffect>(key)) {
            return cached;
        }

        auto sfx = std::make_shared<SoundEffect>(path);
        if (sfx->GetNativeChunk() == nullptr) {
            return nullptr;
        }

        return ResourceManager::Load<SoundEffect>(key, path);
    }

    static std::shared_ptr<Music> LoadMusic(const std::string& path, const std::string& name) {
        std::string key = "MUS_" + name;
        if (auto cached = ResourceManager::TryGet<Music>(key)) {
            return cached;
        }

        auto music = std::make_shared<Music>(path);
        if (music->GetNativeMusic() == nullptr) {
            return nullptr;
        }

        return ResourceManager::Load<Music>(key, path);
    }
};

/**
 * @brief Component for playing audio on an entity.
 */
struct AudioSource {
    std::shared_ptr<SoundEffect> Clip;
    bool PlayOnAwake = false;
    bool Loop = false;
    float Volume = 1.0f; // 0.0 to 1.0

    // Internal tracking
    int Channel = -1;
    bool Initialized = false;
};

} // namespace Forge