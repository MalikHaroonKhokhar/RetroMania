#pragma once

#include <SDL2/SDL_ttf.h>
#include <string>
#include "core/ResourceManager.h"
#include "core/Log.h"

namespace Forge {

/**
 * @brief SDL2_ttf Font wrapper resource.
 */
class Font : public Resource {
public:
    Font(const std::string& path, int size) {
        m_Font = TTF_OpenFont(path.c_str(), size);
        if (!m_Font) {
            FORGE_LOG_ERROR("Failed to load font: ", path, " - Error: ", TTF_GetError());
        }
    }

    ~Font() {
        if (m_Font) {
            TTF_CloseFont(m_Font);
        }
    }

    TTF_Font* GetNativeFont() const { return m_Font; }

private:
    TTF_Font* m_Font = nullptr;
};

} // namespace Forge