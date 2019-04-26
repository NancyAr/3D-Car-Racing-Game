#include "texture_utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <algorithm>

typedef glm::tvec4<GLubyte> ubvec4;

Texture2D* TextureUtils::SingleColor(glm::vec4 color, glm::ivec2 size){
    //Convert color data from floats to unsigned bytes
    ubvec4 c = {(GLubyte)(color.r*255),(GLubyte)(color.g*255),(GLubyte)(color.b*255),(GLubyte)(color.a*255)};
    //Allocate array for texture data
    ubvec4* data = new ubvec4[size.x * size.y];
    //Fill array with the same color
    std::fill_n(data, size.x * size.y, c);
    //Create Texture
    Texture2D * texture = new Texture2D();
    //Set Unpack Alignment to 4-byte (it means that each row takes multiple of 4 bytes in memory)
    //Note: this is not necessary since:
    //- Alignment is 4 by default
    //- Alignment of 1 or 2 will still work correctly but 8 will cause problems
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    //Send data to texture
    //NOTE: the internal format is set to GL_RGBA8 so every pixel contains 4 bytes, one for each channel
    texture->setup(GL_RGBA8, size.x, size.y, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //Generate Mipmaps after loading the texture
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
    delete[] data;
    return texture;
}

Texture2D* TextureUtils::CheckerBoard(glm::ivec2 size, glm::ivec2 patternSize, glm::vec4 color1, glm::vec4 color2){
    ubvec4 c1 = {(GLubyte)(color1.r*255),(GLubyte)(color1.g*255),(GLubyte)(color1.b*255),(GLubyte)(color1.a*255)};
    ubvec4 c2 = {(GLubyte)(color2.r*255),(GLubyte)(color2.g*255),(GLubyte)(color2.b*255),(GLubyte)(color2.a*255)};
    ubvec4* data = new ubvec4[size.x * size.y];
    int ptr = 0;
    for(int y = 0; y < size.y; y++){
        for(int x = 0; x < size.x; x++){
            data[ptr++] = ((x/patternSize.x)&1)^((y/patternSize.y)&1)?c1:c2;
        }
    }
    Texture2D * texture = new Texture2D();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    texture->setup(GL_RGBA8, size.x, size.y, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
    delete[] data;
    return texture;
 }

Texture2D* TextureUtils::Load2DTextureFromFile(const char * filename){
    int width, height, channels;
    //Since OpenGL puts the texture origin at the bottom left while images typically has the origin at the top left,
    //We need to till stb to flip images vertically after loading them
    stbi_set_flip_vertically_on_load(true);
    //Load image data and retrieve width, height and number of channels in the image
    //The last argument is the number of channels we want and it can have the following values:
    //- 0: Keep number of channels the same as in the image file
    //- 1: Grayscale only
    //- 2: Grayscale and Alpha
    //- 3: RGB
    //- 4: RGB and Alpha
    //Note: channels (the 4th argument) always returns the original number of channels in the file
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 4);
    Texture2D * texture = new Texture2D();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    texture->setup(GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
    stbi_image_free(data); //Free image data after uploading to GPU
    return texture;
}

//This function takes the filenames for the 6 sides of the cube in the followinf order:
//Right(+x), Left(-x), Up(+y), Down(-y), Front(+z), Back(-z)
TextureCube* TextureUtils::LoadCubTextureFromFiles(const char * filenames[6]){
    int width, height, channels;
    stbi_set_flip_vertically_on_load(false);//Cube faces don't need to be flipped
    unsigned char* data[6];
    for(int face = 0; face < 6; face++){
        data[face] = stbi_load(filenames[face], &width, &height, &channels, 4);;
    }
    TextureCube* texture = new TextureCube();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    texture->setup(GL_RGBA8, width, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid **)data);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    for(int face = 0; face < 6; face++){
        stbi_image_free(data[face]);
    }
    return texture;
}