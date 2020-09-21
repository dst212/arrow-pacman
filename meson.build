# Meson build file for arrow-pacman

project('arrow-pacman', 'c', version: '0.1a', license: 'gplv3', default_options: 'c_std=c11')

cc = meson.get_compiler('c')

# Compiler flags
#add_global_arguments('-std=c11', language: 'c')

# Warning flags
add_global_arguments('-Wundef', language: 'c')
add_global_arguments('-Winit-self', language: 'c')
add_global_arguments('-Wvla', language: 'c')
add_global_arguments('-Wfloat-equal', language: 'c')
add_global_arguments('-Wlogical-op', language: 'c')
add_global_arguments('-Wduplicated-cond', language: 'c')
add_global_arguments('-Wduplicated-branches', language: 'c')
add_global_arguments('-Wunreachable-code', language: 'c')
add_global_arguments('-Wstrict-aliasing', language: 'c')
add_global_arguments('-Wstrict-overflow', language: 'c')

# Compile for server
if get_option('server') == true
	add_global_arguments('-D_PACMAN_COMPILE_FOR_SERVER', language: 'c')
endif

add_global_arguments('-D_PACMAN_NAME="Arrow Pacman"', language: 'c')
add_global_arguments('-D_PACMAN_AUTHOR="dst212"', language: 'c')
add_global_arguments('-D_PACMAN_VERSION="' + meson.project_version() + '"', language: 'c')

# C-Stuff dependency
cstuff_proj = subproject('cstuff')
cstuff_dep = cstuff_proj.get_variable('cstuff_dep')

# Dependencies
deps = [
	cc.find_library('m'),
	cc.find_library('ncurses'),
	cstuff_dep
]

executable('arrow-pacman', 'src/main.c', dependencies: deps)
