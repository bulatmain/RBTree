cmake_minimum_required(VERSION 3.14)

project(rbtree LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(RBTREE_TESTING "Включить модульное тестирование" OFF)

if((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    add_compile_options(
        -g
        # -Wall
        # -Wextra
        # -Wpedantic

        # -Wcast-align
        # -Wcast-qual
        # -Wconversion
        # -Wctor-dtor-privacy
        # -Wenum-compare
        # -Wfloat-equal
        # -Wnon-virtual-dtor
        # -Wold-style-cast
        # -Woverloaded-virtual
        # -Wredundant-decls
        # -Wsign-conversion
        # -Wsign-promo
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/W4 /WX)
endif()

if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()


# Lib include/
add_library(rbtree_headers INTERFACE)
target_include_directories(rbtree_headers INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_compile_features(rbtree_headers INTERFACE cxx_std_20)
add_library(rbtree::headers ALIAS rbtree_headers)

# Tests
if(NOT RBTREE_TESTING)
    message(STATUS "Тестирование выключено")
else()
    add_subdirectory(test)
endif()

