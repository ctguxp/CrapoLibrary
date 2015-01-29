# Using CMake 3.1
cmake_minimum_required(VERSION 3.1)

# Our Visual Studio Project
project(CrapoLibrary)

# Corlib project
add_subdirectory(corlib)
add_subdirectory(System)
add_subdirectory(System.Window.Gui)
add_subdirectory(System.ServiceProcess)
