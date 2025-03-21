#pragma once

#include <glm/glm.hpp>

#include <GLFWE/texture.hpp>
#include <GLFWE/vertex_array.hpp>

#include <GLFWE/shape/shaders.hpp>

#include <map>

#include <logger/logger.hpp>

namespace GLFWE::Shape {
class Square {
protected:
    Square() = delete;

    static std::unique_ptr<VertexArray> VAO;

public:
    static void draw(glm::vec2 position, glm::vec2 dimentions, glm::vec4 color) {
        Shaders::use(Shaders::Type::FLAT_COLOR);
        glUniform4f(Shaders::get_uniform_location(Shaders::Type::FLAT_COLOR, "color"), color.r, color.g, color.b, color.a);

        if (VAO.get() == nullptr) init_vao();
    }

    static void cleanup() {
        VAO = nullptr;
    }

protected:
    static void init_vao() {
        VAO = std::make_unique<GLFWE::VertexArray>();
        VAO->assign_vertex_attribute(0, 3, GL_FLOAT, GL_FALSE);
        VAO->buffer_vertex_data(sizeof(float)*4*3, NULL, STATIC_DRAW);
    }
};
}