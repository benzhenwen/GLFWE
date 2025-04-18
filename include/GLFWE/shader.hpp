#pragma once

#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <GLFWE/window.hpp>

#include <logger/logger.hpp>

namespace GLFWE {
class Shader {
protected:
    static constexpr Logger logger = Logger("Shader");

    unsigned int glfw_shader;

public:
    #define VERTEX_SHADER GL_VERTEX_SHADER
    #define FRAGMENT_SHADER GL_FRAGMENT_SHADER

    Shader(GLenum type):
    glfw_shader(glCreateShader(type)) {}

    Shader(Shader & other) = delete;
    Shader(Shader && other): glfw_shader(other.glfw_shader) {
        other.glfw_shader = 0;
    }

    ~Shader() {
        if (glfw_shader) destroy();
    }

    void destroy() {
        if (!glfw_shader || Window::has_terminated()) return;
        glDeleteShader(glfw_shader);
        logger << "Shader " << glfw_shader << " destroyed"; 
    }

    unsigned int id() {
        return glfw_shader;
    }

    Shader && load_raw(const std::string & data) {
        const char *c_str = data.c_str();
        glShaderSource(glfw_shader, 1, &c_str, NULL);
        glCompileShader(glfw_shader);

        int success;
        glGetShaderiv(glfw_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(glfw_shader, 512, NULL, infoLog);
            logger.log(Logger::CRITICAL) << "Shader " << glfw_shader << " failed to compile: " << infoLog;
        } else {
            logger << "Shader " << glfw_shader << " successfully compiled";
        }
        return std::move(*this);
    }
    Shader && load_path(const std::string & path) {
        return load_raw(string_from_path(path));
    }

protected:
    inline std::string string_from_path(const std::string & path) {
        std::ifstream f(path);
        if (f) {
            std::stringstream buffer;
            buffer << f.rdbuf();
            f.close();
            return buffer.str();
        }
        logger.log(Logger::CRITICAL) << "Failed to load path: " << path;
        return "";
    }
};
}