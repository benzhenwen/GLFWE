#pragma once

#include <glm/glm.hpp>

#include <GLFWE/texture.hpp>
#include <GLFWE/vertex_array.hpp>

#include <GLFWE/shape/convex_polygon.hpp>

#include <vector>
#include <map>

#include <logger/logger.hpp>

namespace GLFWE::Shape {

    enum Center {TOP_LEFT, CENTER};

    static ConvexPolygon Rectangle(glm::vec2 dimentions, Center center = TOP_LEFT) {
        const float w = dimentions.x;
        const float h = dimentions.y;

        switch (center) {
            case TOP_LEFT:
                return ConvexPolygon {
                    {0, 0},
                    {w, 0},
                    {w, h},
                    {0, h}
                };

            case CENTER:
                const float w2 = w / 2;
                const float h2 = h / 2;
                return ConvexPolygon {
                    {-w2, -h2},
                    {w2, -h2},
                    {w2, h2},
                    {-w2, h2}
                };
        }
    }
    static ConvexPolygon line(glm::vec2 point_a, glm::vec2 point_b, float thickness) {
        glm::vec2 ninety(point_b.y - point_a.y, -point_b.x + point_a.x);
        ninety *= thickness / glm::length(ninety);

        return ConvexPolygon {
            point_a + ninety,
            point_b + ninety,
            point_b - ninety,
            point_a - ninety
        };
    }
}