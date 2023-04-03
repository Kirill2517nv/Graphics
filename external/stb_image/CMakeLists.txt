cmake_minimum_required(VERSION 3.12)

project(stb_image) 

add_library (stb_image STATIC stb_image.h)

target_include_directories(stb_image PUBLIC include)