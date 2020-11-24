project(Game LANGUAGES CXX)

set(SOURCES
    src/Main.cpp
)

add_executable(Game ${SOURCES})

target_include_directories(Game
    PRIVATE

    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(Game
    chemmodities::engine
)

add_custom_command(TARGET Game POST_BUILD 
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND cmd /c ${CMAKE_SOURCE_DIR}/scripts/pb.bat)

add_custom_target(run COMMAND Game WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

set_property(TARGET Game PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/output)