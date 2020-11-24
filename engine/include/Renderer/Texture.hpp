#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include "Utils/Utils.hpp"

namespace che {
    
    enum InternalTextureFormat {
        IF_DepthComponent = GL_DEPTH_COMPONENT,
        IF_DepthStencil = GL_DEPTH_STENCIL,
        IF_Red = GL_RED,
        IF_RG = GL_RG,
        IF_RGB = GL_RGB,
        IF_RGBA = GL_RGBA,

        IF_Red8 = GL_R8,
        IF_Red8SNorm = GL_R8_SNORM,
        IF_Red16 = GL_R16,
        IF_Red16SNorm = GL_R16_SNORM,

        IF_RG8 = GL_RG8,
        IF_RG8SNorm = GL_RG8_SNORM,
        IF_RG16 = GL_RG16,
        IF_RG16SNorm = GL_RG16_SNORM,
        
        IF_R3G3B2 = GL_R3_G3_B2,
        IF_RGB4 = GL_RGB4,
        IF_RGB5 = GL_RGB5,
        IF_RGB8 = GL_RGB8,
        IF_RGB8SNorm = GL_RGB8_SNORM,
        IF_RGB10 = GL_RGB10,
        IF_RGB12 = GL_RGB12,
        IF_RGB16SNorm = GL_RGB16_SNORM,

        IF_RGBA2 = GL_RGBA2,
        IF_RGBA4 = GL_RGBA4,
        IF_RGB5A1 = GL_RGB5_A1,
        IF_RGBA8 = GL_RGBA8,
        IF_RGBA8Snorm = GL_RGBA8_SNORM,
        IF_RGB10A2 = GL_RGB10_A2,
        IF_RGB10A2UI = GL_RGB10_A2UI,
        IF_RGBA12 = GL_RGBA12,
        IF_RGBA16 = GL_RGBA16,
        
        IF_SRGB8 = GL_SRGB8,
        IF_SRGB8A8 = GL_SRGB8_ALPHA8,
        
        IF_Red16F = GL_R16F,
        IF_RG16F = GL_RG16F,
        IF_RGB16F = GL_RGB16F,
        IF_RGBA16F = GL_RGBA16F,

        IF_Red32F = GL_R32F,
        IF_RG32F = GL_RG32F,
        IF_RGB32F = GL_RGB32F,
        IF_RGBA32F = GL_RGBA32F,

        IF_R11FG11FB10F = GL_R11F_G11F_B10F,

        IF_RGB9E5 = GL_RGB9_E5,

        IF_Red8I = GL_R8I,
        IF_Red8UI = GL_R8UI,
        IF_Red16I = GL_R16I,
        IF_Red16UI = GL_R16UI,
        IF_Red32I = GL_R32I,
        IF_Red32UI = GL_R32UI,
        
        IF_RG8I = GL_RG8I,
        IF_RG8UI = GL_RG8UI,
        IF_RG16I = GL_RG16I,
        IF_RG16UI = GL_RG16UI,
        IF_RG32I = GL_RG32I,
        IF_RG32UI = GL_RG32UI,
        
        IF_RGB8I = GL_RGB8I,
        IF_RGB8UI = GL_RGB8UI,
        IF_RGB16I = GL_RGB16I,
        IF_RGB16UI = GL_RGB16UI,
        IF_RGB32I = GL_RGB32I,
        IF_RGB32UI = GL_RGB32UI,
        
        IF_RGBA8I = GL_RGBA8I,
        IF_RGBA8UI = GL_RGBA8UI,
        IF_RGBA16I = GL_RGBA16I,
        IF_RGBA16UI = GL_RGBA16UI,
        IF_RGBA32I = GL_RGBA32I,
        IF_RGBA32UI = GL_RGBA32UI,
        
        IF_CompressedRed = GL_COMPRESSED_RED,
        IF_CompressedRG = GL_COMPRESSED_RG,
        IF_CompressedRGB = GL_COMPRESSED_RGB,
        IF_CompressedRGBA = GL_COMPRESSED_RGBA,
        IF_CompressedSRGB = GL_COMPRESSED_SRGB,
        IF_CompressedSRGBA = GL_COMPRESSED_SRGB_ALPHA,
        IF_CompressedRedRGTC1 = GL_COMPRESSED_RED_RGTC1,
        IF_CompressedSignedRedRGTC1 = GL_COMPRESSED_SIGNED_RED_RGTC1,
        IF_CompressedRGRGTC2 = GL_COMPRESSED_RG_RGTC2,
        IF_CompressedSignedRGRGTC2 = GL_COMPRESSED_SIGNED_RG_RGTC2,
        IF_CompressedRGBABPTCUNorm = GL_COMPRESSED_RGBA_BPTC_UNORM,
        IF_CompressedSRGBABPTCUnorm = GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM,
        IF_CompressedRGBBPTCSignedFloat = GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT,
        IF_CompressedRGBBPTCUnsignedFloat = GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT
    };

    enum GenericPixelFormat {
        PF_Red = GL_RED,
        PF_RG = GL_RG,
        PF_RGB = GL_RGB,
        PF_BGR = GL_BGR,
        PF_RGBA = GL_RGBA,
        PF_BGRA = GL_BGRA,
        PF_RedInt = GL_RED_INTEGER,
        PF_RGInt = GL_RG_INTEGER,
        PF_RGBInt = GL_RGB_INTEGER,
        PF_BGRInt = GL_BGR_INTEGER,
        PF_RGBAInt = GL_RGBA_INTEGER,
        PF_BGRAInt = GL_BGRA_INTEGER,
        PF_StencilIndex = GL_STENCIL_INDEX,
        PF_DepthComponent = GL_DEPTH_COMPONENT,
        PF_DepthStencil = GL_DEPTH_STENCIL
    };

    enum TextureResizeMode {
        Nearest = GL_NEAREST,
        Linear = GL_LINEAR,

        LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
        LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
        NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
        NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST
    };

    enum WrapMode {
        Repeat = GL_REPEAT,
        MirroredRepeat = GL_MIRRORED_REPEAT,
        ClampToEdge = GL_CLAMP_TO_EDGE,
        ClampToBorder = GL_CLAMP_TO_BORDER
    };

    struct Color {

        float r,g,b,a;

        inline Color& operator=(glm::vec3 cvec) {
            r = cvec.r;
            g = cvec.g;
            b = cvec.b;
            a = 0;
        };

        inline Color& operator=(glm::vec4 cvec) {
            r = cvec.r;
            g = cvec.g;
            b = cvec.b;
            a = cvec.a;
        };
    };


    struct TextureSettings {

        TextureResizeMode Mag = TextureResizeMode::Nearest, Min = TextureResizeMode::Linear;
        Color Border = {0,0,0,1};
        int BorderWidth = 0;
        bool CreateMipmaps = false;
        InternalTextureFormat InternalFormat = InternalTextureFormat::IF_RGBA;
        GenericPixelFormat PixelFormat = GenericPixelFormat::PF_RGBA;
        WrapMode WrapS = WrapMode::ClampToBorder, WrapT = WrapMode::ClampToBorder;
        unsigned int ChannelsRequired = 4;
    };

    class Texture : public che::Disposable {
    private:
        unsigned int m_TextureID;

        int m_Width, m_Height, m_Channels;
        TextureSettings m_Settings;
        std::string m_Path;
    public:

        static void init();

        Texture(std::string texturePath, TextureSettings settings);
        Texture(unsigned int width, unsigned int height, TextureSettings settings);

        ~Texture() override;
        
        void bind();
        void unbind();

        void bind(unsigned int unitID);
        void unbind(unsigned int unitID);

        static void setActiveTexture(unsigned int unitID);
        inline unsigned int getHandle() { return m_TextureID; }
    };
}

