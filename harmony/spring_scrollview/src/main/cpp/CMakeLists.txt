cmake_minimum_required(VERSION 3.13)
set(CMAKE_VERBOSE_MAKEFILE on)

file(GLOB rnoh_spring_scrollview_SRC CONFIGURE_DEPENDS *.cpp EventBus/src/*.cpp)
add_library(rnoh_spring_scrollview SHARED ${rnoh_spring_scrollview_SRC})
target_include_directories(rnoh_spring_scrollview PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(rnoh_spring_scrollview PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/EventBus/include)
target_link_libraries(rnoh_spring_scrollview PUBLIC rnoh)