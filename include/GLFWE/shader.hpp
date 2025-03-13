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

    const unsigned int glfw_shader;

    bool destroyed = false;

public:
    #define VERTEX_SHADER GL_VERTEX_SHADER
    #define FRAGMENT_SHADER GL_FRAGMENT_SHADER

    Shader(GLenum type):
    glfw_shader(glCreateShader(type)) {}

    Shader(Shader & other) = delete;
    Shader(Shader && other): glfw_shader(other.glfw_shader) {
        other.destroyed = true;
    }

    ~Shader() {
        if (!destroyed) destroy();
    }

    void destroy() {
        if (destroyed || Window::has_terminated()) return;
        glDeleteShader(glfw_shader);
        logger << "Shader " << glfw_shader << " destroyed"; 
    }

    unsigned int id() {
        return glfw_shader;
    }

    Shader & load_raw(std::string data) {
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
        return *this;
    }
    Shader & load_path(std::string path) {
        load_raw(string_from_path(path));
        return *this;
    }

protected:
    inline std::string string_from_path(std::string path) {
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