project('GLFWE', ['c', 'cpp'],
  version: '0.1.0'
)

glfw_dep = dependency('glfw3')

subdir('glad')
subdir('stb')

freetype_dep = dependency('freetype', fallback: ['freetype', 'freetype_dep'])
glm_dep = dependency('glm', fallback: ['glm', 'glm_dep'])
logger_dep = dependency('logger')

glfwe_dep = declare_dependency(
    include_directories: 'include/',
    dependencies: [logger_dep, glad_dep, glfw_dep, freetype_dep, glm_dep, stb_dep],
    sources: 'include/GLFWE/statics.cpp'
)

executable('GLFWE_demo', 'demo/main.cpp',
    win_subsystem: 'windows',
    dependencies: [logger_dep, glfwe_dep])