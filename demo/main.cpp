#include <GLFWE/window.hpp>
#include <GLFWE/shader.hpp>
#include <GLFWE/shader_program.hpp>
#include <GLFWE/buffer.hpp>
#include <GLFWE/vertex_array.hpp>
#include <GLFWE/texture.hpp>

#include <GLFWE/text/character_set.hpp>

#include <src/core.hpp>

#include <chrono>
#include <ctime>

#include <logger/logger.hpp>
static constexpr Logger logger = Logger("MAIN");

int main() {

    // simulation data
    const FS::Coordinate sim_size = {500, 500};
    auto grid = FS::Simulation(sim_size, 1);
    std::vector<unsigned char> grid_texture_data(sim_size.x * sim_size.y *3);

    // create window
    auto & window = GLFWE::Window::create("Demo", 800, 800);

    // MAIN LOOP
    while(GLFWE::Window::has_instance()) {
        window.clear_color();
        window.swap_buffers();

        window.update();
    }

    return 0;
}

