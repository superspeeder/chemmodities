#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include "Renderer/Buffer.hpp"
#include "Renderer/Texture.hpp"
#include "Renderer/Renderer.hpp"
#include "Utils/Utils.hpp"

#define SPRITEBATCH_MAX_QUADS 50000
#define VERTICIES_PER_QUAD 4
#define INDICIES_PER_QUAD 6
#define VERTEX_SIZE 6
#define SPRITEBATCH_EBO_SIZE SPRITEBATCH_MAX_QUADS * INDICIES_PER_QUAD

/*

QUAD LAYOUT

VERT
bl, br, tl, tr

ELEM
s, s+1, s+3,
s, s+3, s+2

*/


namespace che {

    struct BatchableQuad {
        glm::vec2 position, scale;
        glm::vec2 uv_bl, uv_tr;
        float rotation;
    };

    class SpriteBatch : public che::Disposable {
    private:
        bool m_HasBegun;


    public:
        che::Texture* m_Texture;
        che::VertexBuffer *vboPosition, *vboTexture;
        che::VertexArray *vao;
        size_t m_Quads = 0;


        static che::IndexBuffer* s_StaticEBO;

        static void init(); // fills the ebo;

        SpriteBatch(Texture* tex);
        ~SpriteBatch() override;

        void begin();
        void end();

        void flush();

        void batchQuad(BatchableQuad quad);

        void render();
    };
}