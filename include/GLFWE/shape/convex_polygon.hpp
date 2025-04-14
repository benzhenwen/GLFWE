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
    bool contains_point(glm::vec2 point) {
        bool last_direction = glm::cross(glm::vec3{at(0)-at(size()-1), 0}, glm::vec3{point-at(size()-1), 0}).z >= 0;
        for (int i = 1; i < size(); i++) {
            if (last_direction != glm::cross(glm::vec3{at(i)-at(i-1), 0}, glm::vec3{point-at(i-1), 0}).z >= 0) {
                return false;
            };
        }
        return true;
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
    void draw(glm::vec3 color, float depth) {
        ShapeShader::set_draw_color(color);
        ShapeShader::set_draw_depth(depth);
        draw();
    }
    void draw(glm::vec3 color) {
        ShapeShader::set_draw_color(color);
        draw();
    }
    void draw(float depth) {
        ShapeShader::set_draw_depth(depth);
        draw();
    }

    void draw() {
        ShapeShader::use();
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