project('GLFWE', ['c', 'cpp'],
  version: '0.1.0'
)

glfw_dep = dependency('glfw3')

subdir('glad')
subdir('stb')

logger_dep = subproject('logger').get_variable('logger_dep')
freetype_dep = subproject('freetype').get_variable('freetype_dep')

glfwe_dep = declare_dependency(
    include_directories: 'include/',
    dependencies: [logger_dep, glad_dep, glfw_dep, stb_dep],
    sources: 'include/GLFWE/statics.cpp'
)

executable('GLFWE_demo', 'demo/main.cpp',
    win_subsystem: 'windows',
    dependencies: [logger_dep, glfwe_dep])