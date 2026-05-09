include_directories(BEFORE ../prj.labs)

add_executable(arrayd_test arrayd_test.cpp)
target_link_libraries(arrayd_test arrayd)

add_executable(arrayt_test arrayt_test.cpp)
target_link_libraries(arrayt_test arrayt)

add_executable(bitsetd_test bitsetd_test.cpp)
target_link_libraries(bitsetd_test bitsetd)

add_executable(complex_test complex_test.cpp)
target_link_libraries(complex_test complex)

add_executable(dio_test dio_test.cpp)
target_link_libraries(dio_test dio)

add_executable(queuea_test queuea_test.cpp)
target_link_libraries(queuea_test queuea)


add_executable(queuel_test queuel_test.cpp)
target_link_libraries(queuel_test queuel)

add_executable(rational_test rational_test.cpp)
target_link_libraries(rational_test rational)

add_executable(stacka_test stacka_test.cpp)
target_link_libraries(stacka_test stacka)

add_executable(stackl_test stackl_test.cpp)
target_link_libraries(stackl_test stackl)


# cmake_minimum_required(VERSION 3.20)

# project(misis2026S-25-04-labudeva-v-i)

# set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin.dbg)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin.rel)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}bin.relwithdbg)

# set(CMAKE_CXX_STANDARD 20)

# include_directories(prj.labs prj.thirdparty)
# add_subdirectory(../prj.labs/BitsetD ${CMAKE_CURRENT_BINARY_DIR}/bitsetd)
# add_executable(test_bitset 
#     BitsetD_test.cpp
# )


# target_link_libraries(test_bitset 
#     PRIVATE 
#     bitsetd_lab
# )


# target_include_directories(test_bitset PRIVATE 
#     ${CMAKE_SOURCE_DIR}/prj.labs
#     ${CMAKE_SOURCE_DIR}/prj.thirdparty
# )

# # add_subdirectory(prj.labs)
# # add_subdirectory(prj.test)

# find_package(Doxygen) # REQUIRED - todo later
# option(BUILD_DOCUMENTATION "Create HTML based documentation (requires Doxygen)" ${DOXYGEN_FOUND})

# if (DOXYGEN_FOUND)
#   message("Doxygen build added.")
#   configure_file(
#       "${CMAKE_CURRENT_SOURCE_DIR}/doxyfile.in"
#       "${CMAKE_CURRENT_BINARY_DIR}/doxyfile"
#       @ONLY
#   )
#   doxygen_add_docs(
#       doc # Name of the custom target
#       ${CMAKE_CURRENT_SOURCE_DIR}/prj.labs # Input directory
#       ${CMAKE_CURRENT_SOURCE_DIR}/prj.test # Input directory
#       ALL # Generate documentation for all files
#   )
#   add_custom_target(doxygen_docs ALL DEPENDS doc)
# endif(DOXYGEN_FOUND)