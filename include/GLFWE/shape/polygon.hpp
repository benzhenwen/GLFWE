#pragma once

#include <vector>

#include <logger/logger.hpp>

#include <glm/glm.hpp>

namespace GLFWE::Shape {
class Polygon: public std::vector<glm::vec2> {
public:

    using std::vector<glm::vec2>::vector;

    void translate(glm::vec2 offset) {
        for (glm::vec2 & point : *this) {
            point.x += offset.x;
            point.y += offset.y;
        }
    }
    void flip_horizontal() {
        for (glm::vec2 & point : *this) {
            point.x = -point.x;
        }
    }
    void flip_vertical() {
        for (glm::vec2 & point : *this) {
            point.y = -point.y;
        }
    }

    virtual bool contains_point(glm::vec2 point);

};
}

