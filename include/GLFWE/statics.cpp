#include <GLFWE/window.hpp>

#include <GLFWE/buffer.hpp>
#include <GLFWE/shader_program.hpp>
#include <GLFWE/texture.hpp>
#include <GLFWE/vertex_array.hpp>

#include <GLFWE/text/character_set.hpp>

#include <GLFWE/shape/shaders.hpp>
#include <GLFWE/shape/square.hpp>

using namespace GLFWE;

// window
u_int16_t Window::window_id_increment = 1;
u_int16_t Window::current_window_context = 0;
std::unordered_map<u_int16_t, std::unique_ptr<Window>> Window::window_instances;

// util classes
unsigned int Buffer::current_bound = 0;
unsigned int ShaderProgram::current_bound = 0;
unsigned int Texture::current_bound = 0;
unsigned int VertexArray::current_bound = 0;

// text vao and program
std::unique_ptr<VertexArray> Text::CharacterSet::VAO;
std::unique_ptr<ShaderProgram> Text::CharacterSet::program;

// shapes
std::map<Shape::Shaders::Type, std::unique_ptr<ShaderProgram>> Shape::Shaders::programs;

std::unique_ptr<VertexArray> Shape::Square::VAO;