#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <GLFWE/shader.hpp>
#include <GLFWE/window.hpp>

#include <logger/logger.hpp>

namespace GLFWE {
class ShaderProgram {
protected:
    static constexpr Logger logger = Logger("Shader Program");

    const unsigned int glfw_shader_program;
    bool linked = false;

public:
    ShaderProgram(): glfw_shader_program(glCreateProgram()) {
        logger << "Program " << glfw_shader_program << " created.";
    }

    void destroy() {
        glDeleteProgram(glfw_shader_program);
    }

    unsigned int id() {
        return glfw_shader_program;
    }

    ShaderProgram use() {
        glUseProgram(glfw_shader_program);
        return *this;
    }

    ShaderProgram attach_shader(GLFWE::Shader shader) {
        glAttachShader(glfw_shader_program, shader.id());
        linked = false;
        logger << "Shader " << shader.id() << " successfully attached to program " << glfw_shader_program;
        return *this;
    }

    int get_uniform_location(std::string name) {
        return glGetUniformLocation(glfw_shader_program, name.data());
    }

    ShaderProgram link() {
        if (!linked) {
            glLinkProgram(glfw_shader_program);
            GLint success;
            glGetProgramiv(glfw_shader_program, GL_LINK_STATUS, &success);
            if(!success) {
                char* infoLog;
                glGetProgramInfoLog(glfw_shader_program, 512, NULL, infoLog);
                logger << "Program " << glfw_shader_program << " failed to link:\n" << infoLog;
            }
            linked = true;
            logger << "Program " << glfw_shader_program << " successfully linked";
        } else {
            logger << "Program " << glfw_shader_program << " ignored link request because it was already linked";
        }
        return *this;
    }
};
}