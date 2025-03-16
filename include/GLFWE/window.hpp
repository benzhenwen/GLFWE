#pragma once

#include <stdexcept>
#include <forward_list>
#include <list>
#include <functional>
#include <memory>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <logger/logger.hpp>

namespace GLFWE {
class Window {
protected:
    static constexpr Logger logger = Logger("Window");

// -------------------- GLOBAL STUFF -------------------

// callbacks
static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "glfw error: %s\n", description);
}

// other stuff
private:
    static u_int16_t window_id_increment;
protected:
    static std::unordered_map<u_int16_t, std::unique_ptr<Window>> window_instances;

public:
    static inline bool has_instance() {
        return !window_instances.empty();
    }
    static inline bool has_instance(u_int32_t id) {
        return window_instances.find(id) != window_instances.end();
    }

    static inline bool has_terminated() {
        return !has_instance();
    }

// -------------------- EVENT HANDLING --------------------



// -------------------- WINDOW CLASS UPDATE CYCLE --------------------

public:
    void update() {
        glfwPollEvents();
        if (glfwWindowShouldClose(glfw_window)) {
            destroy();
        }
    }

// -------------------- WINDOW CLASS RENDERING --------------------

public:
    void clear_color(glm::vec3 color = {0.8, 0.8, 0.8}) {
        make_context_current();
        glClearColor(color.x, color.y, color.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void swap_buffers() {
        glfwSwapBuffers(glfw_window);
    }

// -------------------- WINDOW CLASS CONSTRUCTION AND DECONSTRUCTION --------------------
protected:
    u_int16_t window_id;
    GLFWwindow * glfw_window;

    Window(std::string name, glm::vec2 size):
    window_id(window_id_increment++),
    glfw_window(glfwCreateWindow(size.x, size.y, name.data(), NULL, NULL)) {
        if (!glfw_window) {
            logger.log(Logger::CRITICAL) << "glfw window creation failed";
            exit(-1);
        }

        logger << "Window " << get_id() << " successfully created!";  
    }

public:
    static Window & create(std::string name = "window", glm::vec2 size = {640, 640}) {
        bool first_window_flag = !has_instance();
        if (first_window_flag) {
            if (!glfwInit()) { // init if glfw is currently closed
                logger.log(Logger::CRITICAL) << "glfw failed to initialize";
                exit(-1);
            } 
            glfwSetErrorCallback(glfw_error_callback);

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            logger << "GLFW initiated";
        }

        auto new_window = new Window(name, size);
        window_instances.emplace(new_window->get_id(), std::unique_ptr<Window>(new_window));
        new_window->make_context_current();

        if (first_window_flag) {
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                logger.log(Logger::CRITICAL) << "Failed to initialize GLAD";
                exit(-1);
            }
            
            // blend mode
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

            logger << "GLAD initiated";
        }

        return * window_instances[new_window->get_id()];
    }

    void destroy() {
        if(!window_instances.erase(get_id())) {
            logger.log(Logger::WARNING) << "Attempted to destroy window " << get_id() << " but could not find instance";
        }
        terminate();
    }

    ~Window() {
        glfwDestroyWindow(glfw_window);
        glfw_window = nullptr;
        logger << "Window " << get_id() << " destroyed";
    }

    inline u_int32_t get_id() {
        return window_id;
    }

    Window(Window & other) = delete;
    Window(Window && other) = delete;

// -------------------- GLFW HANDLING --------------------

protected:
    static void terminate() {
        if (window_instances.empty()) {
            glfwTerminate();
            logger << "All windows closed, GLFW quit";
        }
    }

private:
    static u_int16_t current_window_context;
public:
    void make_context_current() {
        if (get_id() != current_window_context) {
            glfwMakeContextCurrent(glfw_window);
            current_window_context = get_id();
        }
    }
    static u_int16_t get_current_context() {
        return current_window_context;
    }

};
}