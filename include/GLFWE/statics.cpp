// Handling core members
#include <GLFWE/window.hpp>
#include <GLFWE/text/character_set.hpp>
#include <GLFWE/vertex_array.hpp>
#include <GLFWE/shader_program.hpp>

using namespace GLFWE;

u_int16_t Window::window_id_increment = 1;
u_int16_t Window::current_window_context = 0;
std::unordered_map<u_int16_t, std::unique_ptr<Window>> Window::window_instances;

// text vao and program
std::unique_ptr<VertexArray> Text::CharacterSet::VAO;
std::unique_ptr<ShaderProgram> Text::CharacterSet::program;