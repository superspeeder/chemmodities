#include "Renderer/SpriteBatch.hpp"
#include "Application.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector> 
#include <string> 
#include <algorithm> 
#include <sstream> 
#include <iterator> 
#include <iostream> 

che::IndexBuffer* che::SpriteBatch::s_StaticEBO = nullptr;

void che::SpriteBatch::init() {
    s_StaticEBO = new che::IndexBuffer(che::BufferDataMode::StaticDraw);

    size_t vbo_start;
    for (size_t quad_i = 0 ; quad_i < SPRITEBATCH_MAX_QUADS; quad_i++) {
        vbo_start = quad_i * VERTICIES_PER_QUAD;

        if (quad_i == 0)
            spdlog::debug(vbo_start);

        s_StaticEBO->pushValue(vbo_start);
        s_StaticEBO->pushValue(vbo_start + 1);
        s_StaticEBO->pushValue(vbo_start + 3);


        s_StaticEBO->pushValue(vbo_start);
        s_StaticEBO->pushValue(vbo_start + 3);
        s_StaticEBO->pushValue(vbo_start + 2);
    }

  
    s_StaticEBO->pushBuffer();
}

che::SpriteBatch::SpriteBatch(Texture* tex) {
    m_Texture = tex;
    m_HasBegun = false;
    
    vao = new che::VertexArray();
    vboPosition = new che::VertexBuffer(che::BufferDataMode::DynamicDraw);
    vboTexture = new che::VertexBuffer(che::BufferDataMode::DynamicDraw);

    che::VertexAttribute pos_attr, tex_attr;

    pos_attr.buffer = vboPosition;
    pos_attr.index = 0;
    pos_attr.vertex_size = 4;

    tex_attr.buffer = vboTexture;
    tex_attr.index = 1;
    tex_attr.vertex_size = 2;

    vao->attachIndexBuffer(s_StaticEBO);
    vao->pushAttribute(pos_attr, true);
    vao->pushAttribute(tex_attr, true);
    
    vboPosition->reserve(SPRITEBATCH_MAX_QUADS * 16);
    vboTexture->reserve(SPRITEBATCH_MAX_QUADS * 8);

    s_StaticEBO->bind();
    void* data = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_WRITE);
    unsigned int* fdat = (unsigned int*)data;
    spdlog::debug("{} {} {} {} {} {}", fdat[0],fdat[1],fdat[2],fdat[3],fdat[4],fdat[5]);

    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

    s_StaticEBO->unbind();

    if (che::Disposable::s_autoDisposable) {
        che::Disposable::s_mainDisposer->registerDisposableObject(this);
    }
}

che::SpriteBatch::~SpriteBatch() {
    spdlog::debug("Deleted SpriteBatch");
}

void che::SpriteBatch::begin() {
    vboPosition->clear();
    vboTexture->clear();
    m_HasBegun = true;
    m_Quads = 0;
}

void che::SpriteBatch::end() {
    flush();
    m_HasBegun = false;
}

void che::SpriteBatch::flush() {
    vboPosition->updateBuffer();
    vboTexture->updateBuffer();
}


void che::SpriteBatch::batchQuad(BatchableQuad quad) {
    glm::mat4 transform = glm::mat4(1);
    transform = glm::translate(transform,glm::vec3(quad.position.x + quad.scale.x * 0.5, quad.position.y + quad.scale.x * 0.5,0));
    transform = glm::rotate(transform,quad.rotation, {0,0,1});
    transform = glm::scale(transform,glm::vec3(quad.scale,1));

    // spdlog::debug(glm::to_string(quad.scale));

    glm::vec4 bl = transform * glm::vec4(-0.5,-0.5,0,1);
    glm::vec4 br = transform * glm::vec4(0.5,-0.5,0,1);
    glm::vec4 tl = transform * glm::vec4(-0.5,0.5,0,1);
    glm::vec4 tr = transform * glm::vec4(0.5,0.5,0,1);

    // spdlog::debug("{}", glm::to_string(transform));

    // spdlog::debug("bl: {} ({}, {})", glm::to_string(bl), quad.uv_bl.x, quad.uv_bl.y);
    // spdlog::debug("br: {} ({}, {})", glm::to_string(br), quad.uv_tr.x, quad.uv_bl.y);
    // spdlog::debug("tl: {} ({}, {})", glm::to_string(tl), quad.uv_bl.x, quad.uv_tr.y);
    // spdlog::debug("tr: {} ({}, {})", glm::to_string(tr), quad.uv_tr.x, quad.uv_tr.y);

    vboPosition->pushVec4f(bl);
    vboPosition->pushVec4f(br);
    vboPosition->pushVec4f(tl);
    vboPosition->pushVec4f(tr);

    vboTexture->pushFloat(quad.uv_bl.x);
    vboTexture->pushFloat(quad.uv_bl.y);

    vboTexture->pushFloat(quad.uv_tr.x);
    vboTexture->pushFloat(quad.uv_bl.y);
    
    vboTexture->pushFloat(quad.uv_bl.x);
    vboTexture->pushFloat(quad.uv_tr.y);
    
    vboTexture->pushFloat(quad.uv_tr.x);
    vboTexture->pushFloat(quad.uv_tr.y);
    m_Quads++;  
}

void che::SpriteBatch::render() {
    m_Texture->bind();
    vao->bind();
    glDrawElements(GL_TRIANGLES, m_Quads * 6, GL_UNSIGNED_INT, 0);
    vao->unbind();
    m_Texture->unbind();

}
