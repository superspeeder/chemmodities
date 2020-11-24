#include "Renderer/Framebuffer.hpp"
#include <spdlog/spdlog.h>

#include "Application.hpp"

che::Framebuffer::Framebuffer() {
    glGenFramebuffers(1,&m_FrambufferID);
    if (che::Disposable::s_autoDisposable) {
        che::Disposable::s_mainDisposer->registerDisposableObject(this);
    }
}

che::Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(1,&m_FrambufferID);
    spdlog::debug("Deleted framebuffer {}", m_FrambufferID);
}

void che::Framebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrambufferID);
}

void che::Framebuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool che::Framebuffer::isComplete() {
    bind();
    unsigned int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    unbind();
    return status == GL_FRAMEBUFFER_COMPLETE;
}

void che::Framebuffer::bindTexture(che::Texture* texture, che::TextureAttachment attach) {
    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(attach),GL_TEXTURE_2D, texture->getHandle(),0);
    unbind();
}

void che::Framebuffer::bindRenderBuffer(che::RenderBuffer* rb, che::TextureAttachment attach) {
    bind();
    rb->bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, static_cast<GLenum>(attach), GL_RENDERBUFFER, rb->getHandle());
    rb->unbind();
    unbind();
}

che::RenderBuffer::RenderBuffer(che::InternalTextureFormat format, unsigned int width, unsigned int height) {
    glGenRenderbuffers(1,&m_RenderBufferID);
    m_Width = width;
    m_Height = height;

    bind();

    glRenderbufferStorage(GL_RENDERBUFFER, static_cast<GLenum>(format), m_Width, m_Height);
    
    if (che::Disposable::s_autoDisposable) {
        che::Disposable::s_mainDisposer->registerDisposableObject(this);
    }
}

che::RenderBuffer::~RenderBuffer() {
    glDeleteRenderbuffers(1, &m_RenderBufferID);
    spdlog::debug("Deleted framebuffer {}", m_RenderBufferID);

}
void che::RenderBuffer::bind() {
    glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID);
}

void che::RenderBuffer::unbind() {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}


che::RenderableFrameBuffer::RenderableFrameBuffer(che::Texture* fbTex) {
    this->fbTex = fbTex;

    iboFB = new che::IndexBuffer(che::BufferDataMode::StaticDraw);

    vaoFB = new che::VertexArray();
    vboFBPos = new che::VertexBuffer(che::BufferDataMode::StaticDraw);
    vboFBColor = new che::VertexBuffer(che::BufferDataMode::StaticDraw);
    vboFBUV = new che::VertexBuffer(che::BufferDataMode::StaticDraw);
    
    fb_posAttr.buffer = vboFBPos;
    fb_posAttr.index = 0;
    fb_posAttr.vertex_size = 2;
    vaoFB->pushAttribute(fb_posAttr, true);

    fb_uvAttr.buffer = vboFBUV;
    fb_uvAttr.index = 1;
    fb_uvAttr.vertex_size = 2;
    vaoFB->pushAttribute(fb_uvAttr, true);

    fb_colorAttr.buffer = vboFBColor;
    fb_colorAttr.index = 2;
    fb_colorAttr.vertex_size = 4;
    vaoFB->pushAttribute(fb_colorAttr,true);

    vaoFB->bind();
    vboFBPos->pushFloats(new float[8] {
        -1,-1,
         1,-1,
         1, 1,
        -1, 1
    }, 8);
    vboFBPos->pushBuffer();

    vboFBUV->pushFloats(new float[8] {
        0,0,
        1,0,
        1,1,
        0,1
    },8);
    vboFBUV->pushBuffer();

    vboFBColor->pushFloats(new float[16]{
        1,1,1,1,
        1,1,1,1,
        1,1,1,1,
        1,1,1,1
    },16);
    vboFBColor->pushBuffer();

    iboFB->pushValues(new unsigned int[6] {
        0,1,2,
        0,2,3
    },6);
    iboFB->pushBuffer();

    vaoFB->unbind();

    vaoFB->attachIndexBuffer(iboFB);
}

void che::RenderableFrameBuffer::draw() {
    fbTex->bind();
    vaoFB->bind();
    glDrawElements(GL_TRIANGLES,24,GL_UNSIGNED_INT,0);
    vaoFB->unbind();
    fbTex->unbind();

}