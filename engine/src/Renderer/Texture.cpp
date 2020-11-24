#include "Renderer/Texture.hpp"
#include "Utils/Utils.hpp"
#include "stb_image.h"

#include "Application.hpp"


void che::Texture::init() {
    stbi_set_flip_vertically_on_load(true);
}

che::Texture::Texture(std::string path, che::TextureSettings settings) {

    spdlog::debug("Loading texture {}", path);
    m_Path = path;
    m_Settings = settings;
    unsigned char *texData = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Channels, settings.ChannelsRequired);

    glGenTextures(1,&m_TextureID);

    bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(settings.WrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(settings.WrapT));
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(settings.Min));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(settings.Mag));

    spdlog::default_logger()->flush();

    if (texData) {
        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(settings.InternalFormat), m_Width, m_Height, settings.BorderWidth, static_cast<GLenum>(settings.PixelFormat), GL_UNSIGNED_BYTE, texData);
        spdlog::debug("Loaded Texture {}", path.c_str());

        if (settings.CreateMipmaps) {
            glGenerateMipmap(GL_TEXTURE_2D);
            spdlog::debug("Generated Mipmaps for texture {}", path.c_str());
        }
    } else {
        spdlog::error("Couldn't load texture {}", path.c_str());
    }

    stbi_image_free(texData);
    unbind();
    
    if (che::Disposable::s_autoDisposable) {
        che::Disposable::s_mainDisposer->registerDisposableObject(this);
    }
}

che::Texture::Texture(unsigned int width, unsigned int height, TextureSettings settings) {
    m_Path = "N/A";
    m_Channels = 0;
    m_Settings = settings;
    m_Width = width;
    m_Height = height;

    glGenTextures(1,&m_TextureID);

    bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(settings.WrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(settings.WrapT));
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(settings.Min));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(settings.Mag));

    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(settings.InternalFormat), m_Width, m_Height, settings.BorderWidth, static_cast<GLenum>(settings.PixelFormat), GL_UNSIGNED_BYTE, NULL);

    if (che::Disposable::s_autoDisposable) {
        che::Disposable::s_mainDisposer->registerDisposableObject(this);
    }
}

che::Texture::~Texture() {
    glDeleteTextures(1, &m_TextureID);
    spdlog::debug("Deleted texture[{}] {}", m_TextureID, m_Path);
}

void che::Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void che::Texture::bind(unsigned int unitID) {
    setActiveTexture(unitID);
    bind();
}

void che::Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void che::Texture::unbind(unsigned int unitID) {
    setActiveTexture(unitID);
    unbind();
}


void che::Texture::setActiveTexture(unsigned int unitID) {
    glActiveTexture(GL_TEXTURE0 + unitID);
}