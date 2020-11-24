#pragma once

#include "Renderer/Buffer.hpp"
#include "Renderer/Framebuffer.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Texture.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

#include <vector>

namespace che {

    enum DrawMode {
        Triangles = GL_TRIANGLES
    };

    struct TextureBinding {
        unsigned int unit;
        Texture *tex;
    };

    class Renderer2d {
    public:

        // Renderer2d();

        // void drawElements(DrawMode mode, IndexBuffer* ibo);
    };
}