#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <GLFWE/window.hpp>

#include <logger/logger.hpp>

namespace GLFWE {
class Buffer {
protected:
    static constexpr Logger logger = Logger("Buffer");

    unsigned int glfw_buffer;

    bool destroyed = false;

public:
    Buffer() {
        glGenBuffers(1, &glfw_buffer);
        logger << "Buffer " << glfw_buffer << " successfully created";
    }

    Buffer(Buffer & other) = delete;
    Buffer(Buffer && other): glfw_buffer(other.glfw_buffer) {
        other.destroyed = true;
    }

    ~Buffer() {
        if (!destroyed) destroy();
    }

    void destroy() {
        if (destroyed || Window::has_terminated()) return;
        glDeleteBuffers(1, &glfw_buffer);
        logger << "Buffer " << glfw_buffer << " destroyed"; 
    }

    unsigned int id() {
        return glfw_buffer;
    }

    #define ARRAY_BUFFER GL_ARRAY_BUFFER

    #define STREAM_DRAW GL_STREAM_DRAW // set once & only used a few times
    #define STATIC_DRAW GL_STATIC_DRAW // set once & used many times
    #define DYNAMIC_DRAW GL_DYNAMIC_DRAW // set often & used many times

    template<typename T>
    Buffer & buffer_data(T & data, GLenum buffer_type, GLenum access_type) {
        bind(buffer_type);
        glBufferData(buffer_type, sizeof(data), data, access_type);
        return *this;
    }

private:
    static std::unordered_map<unsigned int, unsigned int> current_bound_buffer;
public:
    void bind(GLenum buffer_type) {
        if (glfw_buffer != current_bound_buffer[Window::get_current_context()]) {
            glBindBuffer(buffer_type, glfw_buffer);
            current_bound_buffer[Window::get_current_context()] = glfw_buffer;
        }
    }
    static unsigned int get_current_bound_array() {
        return current_bound_buffer[Window::get_current_context()];
    }
    bool is_bound() {
        return current_bound_buffer[Window::get_current_context()] == glfw_buffer;
    }
};
}