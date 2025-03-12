project('GLFWE', ['c', 'cpp'],
  version: '0.1.0'
)

glfw_dep = dependency('glfw3')


freetype_dep = dependency('freetype', fallback: ['freetype', 'freetype_dep'])
glm_dep = dependency('glm', fallback: ['glm', 'glm_dep'])
logger_dep = dependency('logger', fallback: ['logger', 'logger_dep'])

glfwe_dep = declare_dependency(
    include_directories: 'include/',
    dependencies: [logger_dep, glfw_dep, freetype_dep, glm_dep],
    sources: 'include/GLFWE/statics.cpp'
)

meson.override_dependency('glfwe', glfwe_dep)