#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <GLFWE/window.hpp>

#include <logger/logger.hpp>

#include <vector>

namespace GLFWE {
class Buffer {
protected:
    static constexpr Logger logger = Logger("Buffer");

    unsigned int glfw_buffer;

public:
    Buffer() {
        glGenBuffers(1, &glfw_buffer);
        logger << "Buffer " << glfw_buffer << " successfully created";
    }

    Buffer(Buffer & other) = delete;
    Buffer(Buffer && other): glfw_buffer(other.glfw_buffer) {
        other.glfw_buffer = 0;
    }

    ~Buffer() {
        if (glfw_buffer) destroy();
    }

    void destroy() {
        if (!glfw_buffer || Window::has_terminated()) return;
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
    Buffer && buffer_data(GLenum buffer_type, std::vector<T> & data, GLenum access_type) {
        return buffer_data(buffer_type, sizeof(data.data()) * data.size(), data.data(), access_type);
    }
    template<typename T>
    Buffer && buffer_data(GLenum buffer_type, T & data, GLenum access_type) {
        return buffer_data(buffer_type, sizeof(data), data, access_type);
    }
    Buffer && buffer_data(GLenum buffer_type, unsigned int data_size, void * data, GLenum access_type) {
        bind(buffer_type);
        glBufferData(buffer_type, data_size, data, access_type);
        return std::move(*this);
    }

    template<typename T>
    Buffer && buffer_sub_data(GLenum buffer_type, unsigned int offset, const std::vector<T> & data) {
        return buffer_sub_data(buffer_type, offset, sizeof(data.data()) * data.size(), data.data());
    }
    template<typename T>
    Buffer && buffer_sub_data(GLenum buffer_type, unsigned int offset, T & data) {
        return buffer_sub_data(buffer_type, offset, sizeof(data), data);
    }
    Buffer && buffer_sub_data(GLenum buffer_type, unsigned int offset, unsigned int data_size, void * data) {
        bind(buffer_type);
        glBufferSubData(buffer_type, offset, data_size, data);
        return std::move(*this);
    }

protected:
    static unsigned int current_bound;
public:
    void bind(GLenum buffer_type) {
        if (current_bound == glfw_buffer) return;
        current_bound = glfw_buffer;
        
        if (!glfw_buffer) logger.log(Logger::WARNING) << "Attempting to bind a buffer ID 0";
        glBindBuffer(buffer_type, glfw_buffer);
    }
};
}