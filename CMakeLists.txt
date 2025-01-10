cmake_minimum_required(VERSION 3.10)

project(PixelCipher VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Boost REQUIRED COMPONENTS program_options)
find_package(OpenCV REQUIRED)

set(SOURCES
    src/main.cpp
    src/cliParser.cpp
    src/pixelCipher.cpp
)

include_directories(
  ${Boost_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/include
)

add_executable(pxc ${SOURCES})

target_link_libraries(pxc ${Boost_LIBRARIES} ${OpenCV_LIBS})

