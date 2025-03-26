#pragma once

#include <glm/glm.hpp>

#include <GLFWE/vertex_array.hpp>

#include <GLFWE/shape/shape_shader.hpp>

#include <vector>

#include <logger/logger.hpp>

namespace GLFWE::Shape {

class ConvexPolygon: public std::vector<glm::vec2> {
public:
    using std::vector<glm::vec2>::vector;

    // shoutout - https://www.youtube.com/watch?app=desktop&v=RSXM9bgqxJM
    bool contains_point(glm::vec2 point) {
        for (int i = 1; i < size(); i++) {
            const float x1 = at(i-1).x;
            const float y1 = at(i-1).x;
            const float x2 = at(i).x;
            const float y2 = at(i).x;

            bool mod_2_equals_1 = false;
            if ((point.y < y1) != (point.y < y2) && point.x < (x1 + ((point.y-y1)/(y2-y1)) * (x2-x1))) {
                mod_2_equals_1 = !mod_2_equals_1;
            }
            return mod_2_equals_1;
        }
    }


protected:
    static std::unique_ptr<VertexArray> VAO;

public:
    /*
    draws the convex polygon
    all points are expected to be in order either clockwise and counterclockwise
    if color or depth are not specified, the previous color / depth will be used instead
    shapes drawn at the same depth will be rendered last on top
    */
    void draw(glm::vec2 position, glm::vec3 color, float depth) {
        ShapeShader::set_draw_color(color);
        draw(position, depth);
    }
    void draw(glm::vec2 position, glm::vec3 color) {
        ShapeShader::set_draw_color(color);
        draw(position);
    }
    void draw(glm::vec2 position, float depth) {
        ShapeShader::set_draw_depth(depth);
        draw(position);
    }
    void draw(glm::vec2 position) {
        ShapeShader::set_draw_position(position);

        if (VAO.get() == nullptr) init_vao();
        VAO->bind();

        VAO->buffer_vertex_data(sizeof(data()) * size(), data(), DYNAMIC_DRAW);
        VAO->draw(GL_TRIANGLE_FAN, 4);
    }

protected:
    static void init_vao() {
        VAO = std::make_unique<GLFWE::VertexArray>();
        VAO->assign_vertex_attribute(0, 2, GL_FLOAT, GL_FALSE);
    }

};
}