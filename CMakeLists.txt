cmake_minimum_required(VERSION 3.22)
project(ProperMDTranslater)

set(CMAKE_CXX_STANDARD 17)

add_executable(ProperMDTranslater Tokenizer.cpp main.cpp)
