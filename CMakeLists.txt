cmake_minimum_required(VERSION 3.10)  # CMake version check
project(evolution_simulator)          # Create project "evolution_simulator"
set(CMAKE_CXX_STANDARD 17)            # Enable c++17 standard

set(SOURCE_FILES main.cpp include/game.hpp include/direction.hpp)

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED COMPONENTS network audio graphics window system)

set(SFML_LIBS sfml-system sfml-window sfml-graphics)


add_executable(evolution_simulator ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE "include")

target_link_libraries(${PROJECT_NAME} ${SFML_LIBS})