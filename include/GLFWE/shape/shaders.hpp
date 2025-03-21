#pragma once

#include <map>
#include <memory>

#include <GLFWE/shader.hpp>
#include <GLFWE/shader_program.hpp>

#include <logger/logger.hpp>

namespace GLFWE::Shape {
class Shaders {
protected:
    static constexpr Logger logger = Logger("SHAPE");

    Shaders() = delete;

public:
    enum Type {FLAT_COLOR};

protected:
    static std::map<Type, std::unique_ptr<GLFWE::ShaderProgram>> programs;

public:
    static void use(Type t) {
        load_program(t);
        programs[t]->use();
    }

    static int get_uniform_location(Type t, const std::string & name) {
        return glGetUniformLocation(programs[t]->id(), name.data());
    }

    // loads the program
    static void pre_load(Type t) {
        load_program(t);
    }

    // unloads the program
    static void clean(Type t) {
        programs[t] = std::unique_ptr<GLFWE::ShaderProgram>();
    }
    
protected:
    static void load_program(Type t) {
        if (programs[t].get() != nullptr) return; // already initialized

        Shader vertex_shader(VERTEX_SHADER);
        Shader fragment_shader(FRAGMENT_SHADER);

        switch(t) {
            case FLAT_COLOR:
                vertex_shader.load_raw(R"(#version 330 core
                    layout (location = 0) in vec3 aPos;

                    void main()
                    {
                        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
                })");           
                fragment_shader.load_raw(R"(#version 330 core
                    out vec4 FragColor;
                    uniform Vec4 color;

                    void main()
                    {
                        FragColor = color;
                })");
                programs[FLAT_COLOR]->attach_shader(vertex_shader).attach_shader(fragment_shader).link();
                break;
            default:
                logger.log(Logger::WARNING) << "Attempted to initialize a shader in shape shaders on enum that doesn't exist";
        }
    }
};
}