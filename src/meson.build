project('cgol', 'cpp',
		license: 'MIT',
		version: '1.0.0',
		default_options: ['buildtype=release', 'warning_level=3', 'cpp_std=c++2a']
		)

inc = include_directories('../include')

srcs = ['grid.cpp', 'main.cpp', 'rle_parser.cpp']

exe = executable('cgol', srcs,
                     include_directories: inc)

