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
        GLFWE::Texture texture; 
        glm::ivec2  size;        // Size of glyph
        glm::ivec2  bearing;     // Offset from baseline to left/top of glyph
        signed long advance;     // Offset to advance to next glyph
    };
    
    std::map<char, Character> characters;

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

        // gl alignment
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);   

        // load each character
        for (unsigned char character = lower_ascii; character < upper_ascii; character++) {
            logger << character;
            if (FT_Load_Char(face, character, FT_LOAD_RENDER)) {
                logger.log(Logger::CRITICAL) << "FreeType failed to load character glyph for: " << character;
            }

            characters.emplace(character, Character());
            characters[character].size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
            characters[character].bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
            characters[character].advance = face->glyph->advance.x;

            characters[character].texture.buffer_image_2D(0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
            characters[character].texture.set_wrapping_behavior(WRAP_CLAMP_EDGE).set_filtering_behavior(FILTER_LINEAR);            
        }
        
        // revert alignment
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);   

        // cleanup
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        logger << "Successfully loaded font: " << font_path.c_str() << " (" << lower_ascii << " - " << upper_ascii-1 << ")";
    }

    void destroy() {
        for (unsigned int i = lower_ascii; i < upper_ascii; i++) {
            logger << "del char: " << i;
            characters[i].texture.destroy();
            characters.erase(i);
        }

        logger << "Font destroyed";
    }
};
}