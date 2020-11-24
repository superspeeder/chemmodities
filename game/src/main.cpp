#define CHE_CORE_DEBUG

#include <iostream>

#include <Main.hpp>
#include <math.h>

#define SIZE 0.05

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    che::Window* w = che::Window::reverseLookup(window);
    glViewport(0,0,width,height);
}

void char_callback(GLFWwindow* window, unsigned int codepoint) {

}


Game::Game() {
    m_WindowSettings.opengl_profile = GLFW_OPENGL_CORE_PROFILE;
    m_WindowSettings.context_version_major = 4;    
    m_WindowSettings.context_version_minor = 5;    
    m_WindowSettings.opengl_debug_context = true;
    m_WindowSettings.width = 800;
    m_WindowSettings.height = 800;
    m_WindowSettings.title = "Noises";
    enableAutoDisposer();
}

void Game::create() {

    che::TextureSettings texSettings, fbTexSettings;
    texSettings.Min = che::TextureResizeMode::Linear;
    texSettings.Mag = che::TextureResizeMode::Linear;



    program = new che::ShaderProgram();
    (*program) << "shaders/vsh.glsl" << "shaders/fsh.glsl";
    program->linkProgram();

    screenProgram = new che::ShaderProgram();
    (*screenProgram) << "shaders/vsh.glsl" << "shaders/fshScreen.glsl";
    screenProgram->linkProgram();

    tex = new che::Texture("textures/HighAsAKite.png", texSettings);
    fbTex = new che::Texture(800,800,fbTexSettings);

    fbuffer = new che::Framebuffer();
    fbuffer->bindTexture(fbTex,che::TextureAttachment::Color0);
    rframeb = new che::RenderableFrameBuffer(fbTex);

    spriteBatch = new che::SpriteBatch(tex);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    glClearColor(1.0f,1.0f,1.0f,1.0f);

}

void Game::render(double dt) {
    spriteBatch->begin();
    spriteBatch->batchQuad({{0,0}, {1,1}, {0,0},{1,1}, 0});
    spriteBatch->end();

    fbuffer->bind();
    glClear(GL_COLOR_BUFFER_BIT);

    program->use();
    spriteBatch->render();
    fbuffer->unbind();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    screenProgram->use();
    rframeb->draw();

    che::ShaderProgram::unbind();
}

void Game::destroy() {
}

int main() {
    Game* game = new Game();
    return game->launch();
}