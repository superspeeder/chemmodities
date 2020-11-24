#pragma once

#include <glad/glad.h>
#include "Renderer/Texture.hpp"
#include "Utils/Utils.hpp"
#include "Renderer/Buffer.hpp"

namespace che {

    enum TextureAttachment {
        Color0 = GL_COLOR_ATTACHMENT0,
        Color1 = GL_COLOR_ATTACHMENT1,
        Color2 = GL_COLOR_ATTACHMENT2,
        Color3 = GL_COLOR_ATTACHMENT3,
        Color4 = GL_COLOR_ATTACHMENT4,
        Color5 = GL_COLOR_ATTACHMENT5,
        Color6 = GL_COLOR_ATTACHMENT6,
        Color7 = GL_COLOR_ATTACHMENT7,
        Color8 = GL_COLOR_ATTACHMENT8,
        Color9 = GL_COLOR_ATTACHMENT9,
        Color10 = GL_COLOR_ATTACHMENT10,
        Color11 = GL_COLOR_ATTACHMENT11,
        Color12 = GL_COLOR_ATTACHMENT12,
        Color13 = GL_COLOR_ATTACHMENT13,
        Color14 = GL_COLOR_ATTACHMENT14,
        Color15 = GL_COLOR_ATTACHMENT15,
        Color16 = GL_COLOR_ATTACHMENT16,
        Color17 = GL_COLOR_ATTACHMENT17,
        Color18 = GL_COLOR_ATTACHMENT18,
        Color19 = GL_COLOR_ATTACHMENT19,
        Color20 = GL_COLOR_ATTACHMENT20,
        Color21 = GL_COLOR_ATTACHMENT21,
        Color22 = GL_COLOR_ATTACHMENT22,
        Color23 = GL_COLOR_ATTACHMENT23,
        Color24 = GL_COLOR_ATTACHMENT24,
        Color25 = GL_COLOR_ATTACHMENT25,
        Color26 = GL_COLOR_ATTACHMENT26,
        Color27 = GL_COLOR_ATTACHMENT27,
        Color28 = GL_COLOR_ATTACHMENT28,
        Color29 = GL_COLOR_ATTACHMENT29,
        Depth = GL_DEPTH_ATTACHMENT,
        Stencil = GL_STENCIL_ATTACHMENT,
        DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT
    };

    class RenderBuffer;

    class Framebuffer : public che::Disposable {
    private:
        unsigned int m_FrambufferID;
    public:
        Framebuffer();
        ~Framebuffer() override;

        void bind();
        void unbind();

        bool isComplete();

        void bindTexture(che::Texture* texture, TextureAttachment attach);

        void bindRenderBuffer(RenderBuffer* rb, TextureAttachment attach);
    };

    class RenderBuffer : public che::Disposable {
    private:
        unsigned int m_RenderBufferID;

        unsigned int m_Width, m_Height;
    public:

        RenderBuffer(che::InternalTextureFormat format, unsigned int width, unsigned int height);
        ~RenderBuffer() override;

        void bind();
        void unbind();

        inline unsigned int getHandle() { return m_RenderBufferID; };

    };

    class RenderableFrameBuffer {
    private:
        
        che::VertexBuffer *vboFBPos, *vboFBColor, *vboFBUV;
        che::IndexBuffer *iboFB;
        che::VertexArray *vaoFB;

        che::VertexAttribute fb_posAttr;
        che::VertexAttribute fb_uvAttr;
        che::VertexAttribute fb_colorAttr;

        che::Texture* fbTex;

    public:

        RenderableFrameBuffer(che::Texture* fbTex);

        void draw();

    };
}