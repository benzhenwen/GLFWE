#pragma once

#include <stb/stb_image.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <GLFWE/window.hpp>

#include <logger/logger.hpp>

namespace GLFWE {
class Texture {
protected:
    static constexpr Logger logger = Logger("Texture");

    unsigned int glfw_texture;

public:
    Texture() {
        glGenTextures(1, &glfw_texture);
    }

    Texture(Texture & other) = delete;
    Texture(Texture && other): glfw_texture(other.glfw_texture) {
        other.glfw_texture = 0;
    }

    ~Texture() {
        if (glfw_texture) destroy();
    }

    void destroy() {
        if (!glfw_texture || Window::has_terminated()) return;
        glDeleteTextures(1, &glfw_texture);
        glfw_texture = 0;
        logger << "Texture " << glfw_texture << " destroyed"; 
    }

    unsigned int id() {
        return glfw_texture;
    }
    
    // #define TEXTURE_1D GL_TEXTURE_1D
    // #define TEXTURE_2D GL_TEXTURE_2D
    // #define TEXTURE_3D GL_TEXTURE_3D


    Texture & buffer_image_from_path(std::string path) {
        stbi_set_flip_vertically_on_load(true);

        int width, height, nChannels;
        unsigned char * data = stbi_load(path.data(), &width, &height, &nChannels, 0);
        if (data) {
            buffer_image_2D(0, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
            logger << "Texture " << glfw_texture << " successfully loaded";
        } else {
            logger.log(Logger::CRITICAL) << "Texture " << glfw_texture << " failed to load path: " << path;
        }
        stbi_image_free(data);
        return *this;
    }

    Texture & buffer_image_2D(int mipmap_level, GLint store_format, int width, int height, GLenum source_format, GLenum source_datatype, void* data, unsigned int pack_alignment = 4) {
        bind();
        glPixelStorei(GL_UNPACK_ALIGNMENT, pack_alignment);
        glTexImage2D(GL_TEXTURE_2D, mipmap_level, store_format, width, height, 0, source_format, source_datatype, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        logger << "Texture " << glfw_texture << " successfully loaded";
        return *this;
    }

    #define WRAP_REPEAT GL_REPEAT
    #define WRAP_MIRROR_REPEAT GL_MIRRORED_REPEAT
    #define WRAP_CLAMP_EDGE GL_CLAMP_TO_EDGE
    #define WRAP_CLAMP_BORDER GL_CLAMP_TO_BORDER

    #define AXIS_X GL_TEXTURE_WRAP_S
    #define AXIS_Y GL_TEXTURE_WRAP_R
    #define AXIS_ALL 0
    struct Color {
        float r, g, b, a;
        GLfloat * data() { return (GLfloat*)this; }
    };

    // Set color mask only when you are using WRAP_CLAMP_BORDER
    Texture & set_wrapping_behavior(GLenum wrapping, GLenum axis = AXIS_ALL, Color mask_color = {0, 0, 0, 1}) {
        bind();
        if (wrapping == WRAP_CLAMP_BORDER) {
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, mask_color.data());
        }
        if (axis == AXIS_ALL) {
            glTexParameteri(GL_TEXTURE_2D, AXIS_X, wrapping);
            glTexParameteri(GL_TEXTURE_2D, AXIS_Y, wrapping);
        } else {
            glTexParameteri(GL_TEXTURE_2D, axis, wrapping);
        }
        return *this;
    }

    #define FILTER_NEAREST GL_NEAREST
    #define FILTER_LINEAR GL_LINEAR

    #define ACTION_MINIMIZING GL_TEXTURE_MIN_FILTER
    #define ACTION_MAGNIFYING GL_TEXTURE_MAG_FILTER
    #define ACTION_ALL 0
    Texture & set_filtering_behavior(GLenum behaviour, GLenum action = ACTION_ALL) {
        bind();
        if (action == ACTION_ALL) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, behaviour);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, behaviour);
        } else {
            glTexParameteri(GL_TEXTURE_2D, action, behaviour);
        }
        return *this;
    }

    // MIPMAP_(what mipmap to grab)_(pixel interpolation)
    #define MIPMAP_NEAREST_NEAREST GL_NEAREST_MIPMAP_NEAREST
    #define MIPMAP_NEAREST_LINEAR GL_NEAREST_MIPMAP_LINEAR
    #define MIPMAP_LINEAR_NEAREST GL_LINEAR_MIPMAP_NEAREST
    #define MIPMAP_LINEAR_LINEAR GL_LINEAR_MIPMAP_LINEAR

    #define ACTION_MINIMIZING GL_TEXTURE_MIN_FILTER
    #define ACTION_MAGNIFYING GL_TEXTURE_MAG_FILTER
    #define ACTION_ALL 0
    Texture & set_mipmap_behavior(GLenum behaviour, GLenum action = ACTION_ALL) {
        bind();
        if (action == ACTION_ALL) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, behaviour);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, behaviour);
        } else {
            glTexParameteri(GL_TEXTURE_2D, action, behaviour);
        }
        return *this;
    }

private:
    static std::unordered_map<unsigned int, unsigned int> current_bound_texture;
public:
    void bind() {
        if (!glfw_texture) logger.log(Logger::WARNING) << "Attempting to bind a texture ID 0";
        if (glfw_texture != current_bound_texture[Window::get_current_context()]) {
            glBindTexture(GL_TEXTURE_2D, glfw_texture);
            current_bound_texture[Window::get_current_context()] = glfw_texture;
        }
    }
    static unsigned int get_current_bound_array() {
        return current_bound_texture[Window::get_current_context()];
    }
    bool is_bound() {
        return current_bound_texture[Window::get_current_context()] == glfw_texture;
    }
};
}