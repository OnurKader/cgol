project('cgol', 'cpp',
		license: 'MIT',
		version: '1.0.0',
		default_options: ['buildtype=release', 'warning_level=3', 'cpp_std=c++2a']
		)

inc = include_directories('../include')

srcs = ['Grid.cpp', 'Main.cpp', 'RLE_Parser.cpp']

exe = executable('cgol', srcs,
                     include_directories: inc)

