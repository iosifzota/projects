cmake_minimum_required(VERSION 3.0)

project(sd)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU OR
        ${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
            set(warnings "-Wall -Wextra -g")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
            set(warnings "/W4 /EHsc")
endif()
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${warnings}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

add_executable(arr
        c_array.cpp
        req.cpp
        )

# add_executable(mat
#     matrix.cpp
#     req.cpp)
#
# add_executable(lab3
#     lab3_prb1.cpp
#     req.cpp
#     )

add_executable(a_star
        a_star.cpp
        req.cpp
        get_path.cpp
        )

# add_executable(heap
#     heap.cpp
#     req.cpp
#     )
#
# add_executable(priority_queue
#     priority_queue.cpp
#     req.cpp
#     )
#

add_executable(huff
        huffman.cpp
        freq.cpp
        req.cpp
        get_path.cpp
        )

add_executable(bst
	req.cpp
	req.hh
	BST.cpp
	BST.h
	)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU OR
	${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
	target_link_libraries(a_star m)
endif()
#
# add_executable(vec
#     vec.cpp
#     req.cpp
#     )
#
# add_executable(test
#     req.cpp
#     test_utils.cpp
#     )
