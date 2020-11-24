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

    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glLineWidth(4);

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
        1,0,0,1,
        0,1,0,1,
        0,0,1,1,
        1,0,1,1
    },16);
    vboFBColor->pushBuffer();

    iboFB->pushValues(new unsigned int[6] {
        0,1,2,
        0,2,3
    },6);
    iboFB->pushBuffer();

    vaoFB->unbind();

    vaoFB->attachIndexBuffer(iboFB);

    program = new che::ShaderProgram();
    screenProgram = new che::ShaderProgram();

    (*program) << "shaders/vsh.glsl" << "shaders/fsh.glsl";
    (*screenProgram) << "shaders/vsh.glsl" << "shaders/fshScreen.glsl";

    program->linkProgram();
    screenProgram->linkProgram();

    che::TextureSettings texSettings, fbTexSettings;

    texSettings.Min = che::TextureResizeMode::Linear;
    texSettings.Mag = che::TextureResizeMode::Linear;

    tex = new che::Texture("textures/HighAsAKite.png", texSettings);
    fbTex = new che::Texture(800,800,fbTexSettings);

    fbuffer = new che::Framebuffer();
    fbuffer->bindTexture(fbTex,che::TextureAttachment::Color0);

    spriteBatch = new che::SpriteBatch(tex);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
}
int factor = 6;
float i = 0;
float pi = 3.14159;

void Game::render(double dt) {
    i += 0.03f;

    float e = 1.0f / factor;
    // spdlog::debug(e);
    spriteBatch->begin();
    for (float x = -1; x < 1; x += e) {
        for (float y = -1; y < 1; y += e) {
            spriteBatch->batchQuad({{x,y}, {e,e}, {0,0},{1,1}, std::fmodf(i,2 * pi)});
        }
    }
    spriteBatch->end();

    fbuffer->bind();
    glClear(GL_COLOR_BUFFER_BIT);

    program->use();
    spriteBatch->render();
    fbuffer->unbind();


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    screenProgram->use();
    fbTex->bind();
    vaoFB->bind();
    glDrawElements(GL_TRIANGLES,24,GL_UNSIGNED_INT,0);
    vaoFB->unbind();
    fbTex->unbind();

    che::ShaderProgram::unbind();
}

void Game::destroy() {
}

int main() {
    Game* game = new Game();
    return game->launch();
}