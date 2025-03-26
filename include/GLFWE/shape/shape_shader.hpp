#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <map>
#include <string>
#include <memory>

#include <GLFWE/window.hpp>
#include <GLFWE/shader.hpp>
#include <GLFWE/shader_program.hpp>

#include <logger/logger.hpp>

namespace GLFWE::Shape {
class ShapeShader {
protected:
    static constexpr Logger logger = Logger("Shape Shader");

    ShapeShader() = delete;

    static std::unique_ptr<GLFWE::ShaderProgram> program;

public:
    static void use() {
        load();
        program->use();
    }

    static void set_draw_color(glm::vec3 color) {
        set_draw_color({color, 1.0f});
    }
    static void set_draw_color(glm::vec4 color) {
        use();
        glUniform4f(program->get_uniform_location("color"), color.r, color.g, color.b, color.a);
    }

    static void set_draw_position(glm::vec2 position) {
        use();
        glUniform2f(program->get_uniform_location("position"), position.x, position.y);
    }

    static void set_draw_depth(float depth) {
        use();
        glUniform1f(program->get_uniform_location("depth"), depth);
    }

    static void set_projection(glm::vec2 projection) {
        use();
        glm::mat4 projection_matrix = glm::ortho(0.0f, projection.x, 0.0f, projection.y);
        glUniformMatrix4fv(program->get_uniform_location("projection"), 1, GL_FALSE, glm::value_ptr(projection_matrix));
    }

    static void pre_load() {
        load();
    }

    static void clean() {
        program.release();
    }
    
protected:
    static void load() {
        if (program != nullptr) return; // already initialized
        program = std::make_unique<ShaderProgram>();

        Shader vertex_shader = Shader(VERTEX_SHADER).load_raw(R"(
            #version 330 core
            layout (location = 0) in vec2 aPos;

            uniform mat4 projection;

            uniform vec2 position;
            uniform int depth;
            
            void main()
            {
                gl_Position = projection * vec4(aPos.xy + position.xy, depth, 1.0);
        })");
        Shader fragment_shader = Shader(FRAGMENT_SHADER).load_raw(R"(
            #version 330 core
            uniform vec4 color;
            out vec4 FragColor;

            void main()
            {
                FragColor = color;
            }
        )");

        program->attach_shader(vertex_shader).attach_shader(fragment_shader).link().use();  
        glUniform4f(program->get_uniform_location("color"), 0, 0, 0, 1); 

        // automatically set projection if possible
        if (!Window::has_only_one_instance()) logger.log(Logger::WARNING) << "Multiple window instances detected. Please manually decalre the projection for GLFWE/Shape/ShapeShader";
        else {
            glm::vec2 proj_size = Window::get_single_instance_size();
            logger << "Automatically configuring projection for ShapeShader to: (" << proj_size.x << ", " << proj_size.y << ")";
            set_projection(proj_size);
        }
    }
};
}