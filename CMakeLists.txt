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

add_executable(main main.cpp cat.cpp cat.hh dog.cpp dog.hh pet.cpp pet.hh)

#add_executable(arr vec.cpp req.cpp)
#add_executable(btrees basic_btree.hh RB-node.hh rbtree.hh map.hh binary_trees.cpp req.hh req.cpp)
#add_executable(redblack basic_btree.hh RB-node.hh rbtree.hh req.hh req.cpp rb-tree.cpp)

#add_executable(hash hash_table.cpp hash_table.hh req.cpp utility.cpp)
#add_executable(test_hashes test_hashes.cpp hash_table.hh hashes.cpp utility.cpp req.cpp)

# for poo
#add_executable(shop shop.cpp product.cpp customer.cpp utility.cpp hashes.cpp req.cpp products_order.cpp customers_order.cpp)

# add_executable(mat
#     matrix.cpp
#     req.cpp)
#
# add_executable(lab3
#     lab3_prb1.cpp
#     req.cpp
#     )

#add_executable(a_star a_star.cpp req.cpp)

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

#add_executable(huff huffman.cpp freq.cpp req.cpp)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU OR
	${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
    #	target_link_libraries(a_star m)
    #	target_link_libraries(hash m)
endif()

# add_executable(vec
#     vec.cpp
#     req.cpp
#     )
#
# add_executable(test
#     req.cpp
#     test_utils.cpp
#     )
