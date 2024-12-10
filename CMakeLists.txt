# Minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(PixelCipher VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Boost
find_package(Boost REQUIRED COMPONENTS program_options)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Add your source files
set(SOURCES
    main.cpp
    cliParser.cpp
    pixelCipher.cpp
)

# Include directories
include_directories(${Boost_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

# Add the executable
add_executable(pxc ${SOURCES})

# Link libraries
target_link_libraries(pxc ${Boost_LIBRARIES} ${OpenCV_LIBS})

