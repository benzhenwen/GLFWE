#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <GLFWE/buffer.hpp>
#include <GLFWE/window.hpp>

#include <logger/logger.hpp>

namespace GLFWE {
class VertexArray {
protected:
    static constexpr Logger logger = Logger("Vertex Array");

    Buffer vertex_buffer;
    unsigned int glfw_vertex_array;

public:
    VertexArray() {
        glGenVertexArrays(1, &glfw_vertex_array);  
        logger << "vertex array " << glfw_vertex_array << " successfully created";
    }

    VertexArray(VertexArray & other) = delete;
    VertexArray(VertexArray && other): glfw_vertex_array(other.glfw_vertex_array) {
        other.glfw_vertex_array = 0;
    }

    ~VertexArray() {
        if (glfw_vertex_array) destroy();
    }

    void destroy() {
        if (!glfw_vertex_array || Window::has_terminated()) return;
        vertex_buffer.destroy();
        glDeleteVertexArrays(1, &glfw_vertex_array);
        logger << "vertex array " << glfw_vertex_array << " destroyed";
    }

    unsigned int id() {
        return glfw_vertex_array;
    }

    #define METHOD_POINTS GL_POINTS
    #define METHOD_LINES GL_LINES
    #define METHOD_LINE_LOOP GL_LINE_LOOP
    #define METHOD_LINE_STRIP GL_LINE_STRIP
    #define METHOD_TRIANGLES GL_TRIANGLES
    #define METHOD_TRIANGLE_STRIP GL_TRIANGLE_STRIP
    #define METHOD_TRIANGLE_FAN GL_TRIANGLE_FAN
    void draw(GLenum method, int length, int offset = 0) {
        bind();
        glDrawArrays(method, offset, length);
    }

    #define STREAM_DRAW GL_STREAM_DRAW // set once & only used a few times
    #define STATIC_DRAW GL_STATIC_DRAW // set once & used many times
    #define DYNAMIC_DRAW GL_DYNAMIC_DRAW // set often & used many times

    #define TYPE_FLOAT GL_FLOAT
    #define TYPE_INT GL_INT
    #define TYPE_UNSIGNED_INT GL_UNSIGNED_INT



    template<typename T>
    VertexArray && buffer_vertex_data(T & data, GLenum draw_type) {
        bind();
        vertex_buffer.buffer_data(ARRAY_BUFFER, data, draw_type);
        return std::move(*this);
    }
    VertexArray && buffer_vertex_data(unsigned int data_size, void * data, GLenum draw_type) {
        bind();
        vertex_buffer.buffer_data(ARRAY_BUFFER, data_size, data, draw_type);
        return std::move(*this);
    }

    template<typename T>
    VertexArray && buffer_vertex_sub_data(unsigned int offset, T & data) {
        bind();
        vertex_buffer.buffer_sub_data(ARRAY_BUFFER, offset, data);
        return std::move(*this);
    }
    VertexArray && buffer_vertex_sub_data(unsigned int offset, unsigned int data_size, void * data) {
        bind();
        vertex_buffer.buffer_sub_data(ARRAY_BUFFER, offset, data_size, data);
        return std::move(*this);
    }
    
    VertexArray && assign_vertex_attribute(unsigned int location, unsigned int size, GLenum type, bool normalized, unsigned int stride = 0, unsigned int offset = 0) {        
        bind();
        glEnableVertexAttribArray(location);  
        glVertexAttribPointer(location, size, type, normalized, stride, (const void*) offset);
        return std::move(*this);
    }

    Buffer & get_buffer() {
        return vertex_buffer;
    }

protected:
    static unsigned int current_bound;
public:
    void bind() {
        if (current_bound == glfw_vertex_array) return;
        current_bound = glfw_vertex_array;
        
        if (!glfw_vertex_array) logger.log(Logger::WARNING) << "Attempting to bind a vertex array ID 0";
        glBindVertexArray(glfw_vertex_array);
        vertex_buffer.bind(ARRAY_BUFFER);
    }
};
}