#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <GLFWE/buffer.hpp>

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

    ~VertexArray() {
        // vertex buffer automatically deleted on de-allocation
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
    VertexArray buffer_vertex_data(T & data, GLenum draw_type) {
        bind();
        vertex_buffer.buffer_data(data, ARRAY_BUFFER, draw_type);
        return *this;
    }
    
    VertexArray assign_vertex_attribute(unsigned int location, unsigned int size, GLenum type, bool normalized, unsigned int stride = 0, unsigned int offset = 0) {        
        bind();
        
        glVertexAttribPointer(location, size, type, normalized, stride, (const void*) offset);
        glEnableVertexAttribArray(location);  
        return *this;
    }

private:
    static std::unordered_map<unsigned int, unsigned int> current_bound_array;
public:
    void bind() {
        if (glfw_vertex_array != current_bound_array[Window::get_current_context()]) {
            glBindVertexArray(glfw_vertex_array);
            current_bound_array[Window::get_current_context()] = glfw_vertex_array;
        }
    }
    static unsigned int get_current_bound_array() {
        return current_bound_array[Window::get_current_context()];
    }
    bool is_bound() {
        return current_bound_array[Window::get_current_context()] == glfw_vertex_array;
    }
};
}