#include <ft2build.h>
#include FT_FREETYPE_H 

#include <glm/glm.hpp>

#include <GLFWE/texture.hpp>

#include <logger/logger.hpp>

#include <filesystem>
#include <map>

namespace GLFWE::Text {
class CharacterSet {
protected:
    static constexpr Logger logger = Logger("GLFWTEXT");

    struct Character {
        unsigned int TextureID;  // ID handle of the glyph texture
        glm::ivec2   Size;       // Size of glyph
        glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
        unsigned int Advance;    // Offset to advance to next glyph
    };
    
    std::map<char, Character> Characters;

    const unsigned int lower_ascii, upper_ascii;

public:
    CharacterSet(const std::filesystem::path & font_path, unsigned int font_height, unsigned int _lower_ascii = 0,  unsigned int _upper_ascii = 128):
    lower_ascii(_lower_ascii), upper_ascii(_upper_ascii) {
        // init ft
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            logger.log(Logger::CRITICAL) << "ERROR: Failed to init FreeType library";
        }
        
        // init face
        FT_Face face;
        if (FT_New_Face(ft, font_path.c_str(), 0, &face)) {
            logger.log(Logger::CRITICAL) << "ERROR: Failed to load font into FreeType";
        }

        // set ft font size
        FT_Set_Pixel_Sizes(face, 0, font_height);

        // load each character
        for (unsigned char character = lower_ascii; character < upper_ascii; character++) {

        }

        logger << "meow";
    }

protected:
    GLFWE::Texture generate_char_texture(unsigned char character) {

    }
};
}