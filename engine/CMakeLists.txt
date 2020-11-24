project(Engine LANGUAGES CXX)

set(SOURCES
    src/ECS/Component.cpp
    src/ECS/Entity.cpp
    src/Renderer/Buffer.cpp
    src/Renderer/Shader.cpp
    src/Renderer/Window.cpp
    src/Renderer/Framebuffer.cpp
    src/Renderer/Texture.cpp
    src/Renderer/Renderer.cpp
    src/Renderer/SpriteBatch.cpp
    src/Utils/Exceptions.cpp
    src/Utils/Utils.cpp
    src/Application.cpp
)

set(GLAD_SOURCE
    vendor/glad/src/glad.c
)

add_library(Engine ${SOURCES} ${GLAD_SOURCE})
add_library(chemmodities::engine ALIAS Engine)

target_include_directories(Engine
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/vendor/glad/include
)

add_subdirectory(vendor/glm)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/stb)
add_subdirectory(vendor/spdlog)

target_link_libraries(Engine
    glm::glm
    glfw
    stb::stb
    spdlog::spdlog
)