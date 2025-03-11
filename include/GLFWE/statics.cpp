#include <GLFWE/window.hpp>
#include <GLFWE/buffer.hpp>
#include <GLFWE/vertex_array.hpp>
#include <GLFWE/texture.hpp>

using namespace GLFWE;

u_int16_t Window::window_id_increment = 1;
u_int16_t Window::current_window_context = 0;
std::unordered_map<u_int16_t, std::unique_ptr<Window>> Window::window_instances;

std::unordered_map<unsigned int, unsigned int> Buffer::current_bound_buffer;
std::unordered_map<unsigned int, unsigned int> VertexArray::current_bound_array;
std::unordered_map<unsigned int, unsigned int> Texture::current_bound_texture;
