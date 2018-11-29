cmake_minimum_required(VERSION 3.12)
project(hellowindow)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/local/include src/)

if (APPLE)
    include_directories(/System/Library/Frameworks)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    find_library(IOKit_LIBRARY IOKit)
    find_library(CoreVideo_LIBRARY CoreVideo)
    find_library(soil_library SOIL)
    mark_as_advanced(${COCOA_LIBRARY} ${OpenGL_LIBRARY})
    set(EXTRA_LIBS ${COCOA_LIBRARY} ${OpenGL_LIBRARY} ${IOKit_LIBRARY} ${CoreVideo_LIBRARY})
    set(EXTRA_LIBS ${EXTRA_LIBS} /usr/local/lib/libGLEW.a /usr/local/lib/libglfw3.a)
endif ()

set(RUN_DIR ${PROJECT_SOURCE_DIR})

add_executable(test main.cpp)

add_executable(hellowindow src/hellowindow/hellowindow.cpp)
target_link_libraries(hellowindow ${EXTRA_LIBS})

add_executable(hellotriangle src/hellotriangle/hellotriangle.cpp)
target_link_libraries(hellotriangle ${EXTRA_LIBS})

add_executable(helloshader src/helloshader/helloshader.cpp)
target_link_libraries(helloshader ${EXTRA_LIBS})

add_executable(test-texture src/test_texture/test_texture.cpp)
target_link_libraries(test-texture ${EXTRA_LIBS} /usr/local/lib/libSOIL.a)

add_executable(test-trans src/trans/test_trans.cpp)
target_link_libraries(test-trans ${EXTRA_LIBS} ${soil_library})

set(extra_libs ${EXTRA_LIBS} ${soil_library})
add_executable(test-coordinate src/coordinate/test_coordinate.cpp)
target_link_libraries(test-coordinate ${extra_libs})

add_executable(test-camera src/testcamera/testcamera.cpp)
target_link_libraries(test-camera ${extra_libs})
