project('GLFWE', 'cpp',
  version: '0.1.0'
)

glfw_dep = dependency('glfw3')

subdir('glad')
subdir('stb')

logger_dep = subproject('logger').get_variable('logger_dep')

glfwe_dep = declare_dependency(
    include_directories: 'include/',
    dependencies: [glad_dep, glfw_dep, logger_dep, stb_dep],
    sources: 'include/GLFWE/statics.cpp'
)