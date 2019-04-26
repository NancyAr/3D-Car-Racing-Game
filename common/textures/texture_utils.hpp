#ifndef TEXTURE_UTILS_HPP
#define TEXTURE_UTILS_HPP

#define GLM_FORCE_CXX11
#include <glm/glm.hpp>

#include "texture2d.hpp"
#include "texture_cube.hpp"

class TextureUtils {
public:
    static Texture2D* SingleColor(glm::vec4 color={1,1,1,1}, glm::ivec2 size={1,1});
    static Texture2D* CheckerBoard(glm::ivec2 size, glm::ivec2 patternSize, glm::vec4 color1, glm::vec4 color2);
    static Texture2D* Load2DTextureFromFile(const char * filename);
    static TextureCube* LoadCubTextureFromFiles(const char * filenames[6]);
};

#endif