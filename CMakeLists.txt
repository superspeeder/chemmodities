cmake_minimum_required(VERSION 3.10)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

project(ChemmoditiesEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)



add_subdirectory(engine)
add_subdirectory(game)


install(TARGETS Game RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/output)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/res DESTINATION ${PROJECT_SOURCE_DIR}/output)



