#include <ft2build.h>
#include FT_FREETYPE_H 

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFWE/window.hpp>
#include <GLFWE/texture.hpp>
#include <GLFWE/vertex_array.hpp>
#include <GLFWE/shader.hpp>
#include <GLFWE/shader_program.hpp>

#include <logger/logger.hpp>

#include <filesystem>
#include <memory>
#include <map>

namespace GLFWE::Text {
class CharacterSet {
protected:
    static constexpr Logger logger = Logger("Text");

    struct Character {
        GLFWE::Texture texture; 
        glm::ivec2  size;        // Size of glyph
        glm::ivec2  bearing;     // Offset from baseline to left/top of glyph
        signed long advance;     // Offset to advance to next glyph
    };
    
    std::map<char, Character> characters;

    const unsigned int lower_ascii, upper_ascii;

    static std::unique_ptr<GLFWE::VertexArray> VAO;
    static std::unique_ptr<GLFWE::ShaderProgram> program;

public:
    CharacterSet(const std::filesystem::path & font_path, unsigned int font_height, unsigned int _lower_ascii = 0,  unsigned int _upper_ascii = 128):
    lower_ascii(_lower_ascii), upper_ascii(_upper_ascii) {
        // gl alignment
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  

        // prepare vertex array and program
        if (VAO == nullptr) {
            prepare_VAO_and_program();
        }
        
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
            if (FT_Load_Char(face, character, FT_LOAD_RENDER)) {
                logger.log(Logger::CRITICAL) << "FreeType failed to load character glyph for: " << character;
            }

            characters.emplace(character, Character());
            characters[character].size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
            characters[character].bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
            characters[character].advance = face->glyph->advance.x;

            characters[character].texture.buffer_image_2D(0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer, 1);
            characters[character].texture.set_wrapping_behavior(WRAP_CLAMP_EDGE).set_filtering_behavior(FILTER_LINEAR);            
        }
        
        // revert alignment
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);   

        // cleanup
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        logger << "Successfully loaded font: " << font_path.c_str() << " (" << lower_ascii << " - " << upper_ascii-1 << ")";
    }

    ~CharacterSet() {
        destroy();
    }

    void destroy() {
        if (Window::has_terminated()) return;
        characters.clear();
        logger << "Font destroyed";
    }

    static void set_projection(glm::vec2 projection) {
        program->use();
        glm::mat4 projection_matrix = glm::ortho(0.0f, projection.x, 0.0f, projection.y);
        glUniformMatrix4fv(program->get_uniform_location("projection"), 1, GL_FALSE, glm::value_ptr(projection_matrix));
    }

    void render_string(const std::string & text, glm::vec2 position, float scale, const glm::vec3 color) {
        program->use();
        VAO-> bind();

        // color
        glUniform3f(program->get_uniform_location("textColor"), color.x, color.y, color.z);

        // iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++) 
        {
            Character & ch = characters[*c];

            float xpos = position.x + ch.bearing.x * scale;
            float ypos = position.y - (ch.size.y - ch.bearing.y) * scale;

            float w = ch.size.x * scale;
            float h = ch.size.y * scale;

            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },            
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }           
            };
            
            ch.texture.bind();

            VAO->buffer_vertex_sub_data(0, vertices);
            VAO->draw(GL_TRIANGLES, 6, 0);

            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            position.x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
    }

private:
    static void prepare_VAO_and_program() {
        VAO = std::make_unique<GLFWE::VertexArray>();
        program = std::make_unique<GLFWE::ShaderProgram>();

        VAO->buffer_vertex_data(sizeof(float)*6*4, NULL, DYNAMIC_DRAW);
        VAO->assign_vertex_attribute(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float));

        auto vertex_shader = GLFWE::Shader(VERTEX_SHADER);
        vertex_shader.load_raw(
            R"(#version 330 core
            layout (location = 0) in vec4 vertex;
            out vec2 TexCoords;

            uniform mat4 projection;

            void main()
            {
                gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
                TexCoords = vertex.zw;
        })");
        auto fragment_shader = GLFWE::Shader(FRAGMENT_SHADER);
        fragment_shader.load_raw(
            R"(#version 330 core
            in vec2 TexCoords;
            out vec4 color;

            uniform sampler2D text;
            uniform vec3 textColor;

            void main()
            {    
                vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
                color = vec4(textColor, 1.0) * sampled;
            })"
        );

        program->attach_shader(vertex_shader).attach_shader(fragment_shader).link().use();

        if (!Window::has_only_one_instance()) logger.log(Logger::WARNING) << "Multiple window instances detected. Please manually decalre the projection for GLFWE/Shape/CharacterSet";
        else {
            glm::vec2 proj_size = Window::get_single_instance_size();
            logger << "Automatically configuring projection for CharacterSet to: (" << proj_size.x << ", " << proj_size.y << ")";
            set_projection(proj_size);
        }
    }
};
}