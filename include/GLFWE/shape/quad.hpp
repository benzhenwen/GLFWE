#pragma once

#include <glm/glm.hpp>

#include <GLFWE/texture.hpp>
#include <GLFWE/vertex_array.hpp>

#include <GLFWE/shape/shape_shader.hpp>

#include <vector>
#include <map>

#include <logger/logger.hpp>

namespace GLFWE::Shape {
class Quad {
protected:
    Quad() = delete;

    static std::unique_ptr<VertexArray> VAO;

public:
    static void draw_square(glm::vec2 position, glm::vec2 dimentions, float depth = 0.0f) {
        float data[] = {
            position.x,                position.y,                depth,
            position.x + dimentions.x, position.y,                depth,
            position.x + dimentions.x, position.y + dimentions.y, depth,
            position.x,                position.y + dimentions.y, depth 
        };
            
        draw(data);
    }
    static void draw_line(glm::vec2 point_a, glm::vec2 point_b, float thickness, float depth = 0.0f) {
        glm::vec2 ninety(point_b.y - point_a.y, -point_b.x + point_a.x);
        ninety *= thickness / glm::length(ninety);

        glm::vec2 p1 = point_a + ninety;
        glm::vec2 p2 = point_b + ninety;
        glm::vec2 p3 = point_b - ninety;
        glm::vec2 p4 = point_a - ninety;

        float data[] = {
            p1.x, p1.y, depth,
            p2.x, p2.y, depth,
            p3.x, p3.y, depth,
            p4.x, p4.y, depth
        };

        draw(data);
    }

    static void cleanup() {
        VAO.release();
    }

protected:
    static void draw(float * data) {
        ShapeShader::use();
        if (VAO.get() == nullptr) init_vao();
        VAO->bind();

        VAO->buffer_vertex_sub_data(0, sizeof(float) * 4 * 3, data);
        VAO->draw(GL_TRIANGLE_FAN, 4);
    }

    static void init_vao() {
        VAO = std::make_unique<GLFWE::VertexArray>();
        VAO->assign_vertex_attribute(0, 3, GL_FLOAT, GL_FALSE);
        VAO->buffer_vertex_data(sizeof(float)*4*3, NULL, DYNAMIC_DRAW);
    }
};
}