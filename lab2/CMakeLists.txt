cmake_minimum_required(VERSION 4.1)

project("lab2" CXX)

add_executable("lab2")

target_compile_features("lab2" PUBLIC cxx_std_23)

target_sources("lab2" PRIVATE "main.cpp")