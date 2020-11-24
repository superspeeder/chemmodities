#define CHE_CORE_DEBUG

#include <iostream>

#include <Main.hpp>


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

    m_Window->setFramebufferSizeCallback(framebuffer_size_callback);
    m_Window->setCharCallback(char_callback);

    vboPosition = new che::VertexBuffer(che::BufferDataMode::StaticDraw);
    vboColor = new che::VertexBuffer(che::BufferDataMode::StaticDraw);
    vboUV = new che::VertexBuffer(che::BufferDataMode::StaticDraw);
    iboFB = new che::IndexBuffer(che::BufferDataMode::StaticDraw);
    ibo = new che::IndexBuffer(che::BufferDataMode::StaticDraw);
    vao = new che::VertexArray();

    vaoFB = new che::VertexArray();
    vboFBPos = new che::VertexBuffer(che::BufferDataMode::StaticDraw);
    vboFBColor = new che::VertexBuffer(che::BufferDataMode::StaticDraw);
    vboFBUV = new che::VertexBuffer(che::BufferDataMode::StaticDraw);

    positionAttr.buffer = vboPosition;
    positionAttr.index = 0;
    positionAttr.vertex_size = 3;
    vao->pushAttribute(positionAttr, true);

    colorAttr.buffer = vboColor;
    colorAttr.index = 2;
    colorAttr.vertex_size = 4;
    vao->pushAttribute(colorAttr, true);

    uvAttr.buffer = vboUV;
    uvAttr.index = 1;
    uvAttr.vertex_size = 2;
    vao->pushAttribute(uvAttr, true);
    
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

    vao->bind();
    vboPosition->pushVec3f(-1,1,0); // 0
    vboPosition->pushVec3f(0,1,0); // 1
    vboPosition->pushVec3f(1,1,0); // 2
    vboPosition->pushVec3f(-1,0,0); // 3
    vboPosition->pushVec3f(0,0,0); // 4
    vboPosition->pushVec3f(1,0,0); // 5
    vboPosition->pushVec3f(-1,-1,0); // 6
    vboPosition->pushVec3f(0,-1,0); // 7
    vboPosition->pushVec3f(1,-1,0); // 8
    vboPosition->pushBuffer();

    vboColor->pushVec4f(1,1,1,1);
    vboColor->pushVec4f(1,0,0,1);
    vboColor->pushVec4f(1,1,1,1);

    vboColor->pushVec4f(0,1,0,1);
    vboColor->pushVec4f(0,0,0,1);
    vboColor->pushVec4f(0,0,1,1);

    vboColor->pushVec4f(1,1,1,1);
    vboColor->pushVec4f(1,0,1,1);
    vboColor->pushVec4f(1,1,1,1);
    vboColor->pushBuffer();

    vboUV->pushVec2f(0.0,1.0);
    vboUV->pushVec2f(0.5,1.0);
    vboUV->pushVec2f(1.0,1.0);
    vboUV->pushVec2f(0.0,0.5);
    vboUV->pushVec2f(0.5,0.5);
    vboUV->pushVec2f(1.0,0.5);
    vboUV->pushVec2f(0.0,0.0);
    vboUV->pushVec2f(0.5,0.0);
    vboUV->pushVec2f(1.0,0.0);
    vboUV->pushBuffer();

    vao->unbind();

    vao->attachIndexBuffer(ibo);
    vaoFB->attachIndexBuffer(iboFB);

    ibo->pushValues(new unsigned int[24] {
        0,3,1,
        3,4,1,
        2,1,5,
        4,5,1,
        3,6,7,
        7,4,3,
        4,7,5,
        7,8,5
    }, 24);
    ibo->pushBuffer();

    program = new che::ShaderProgram();
    screenProgram = new che::ShaderProgram();

    (*program) << "shaders/vsh.glsl" << "shaders/fsh.glsl";
    (*screenProgram) << "shaders/vsh.glsl" << "shaders/fshScreen.glsl";

    program->linkProgram();
    screenProgram->linkProgram();

    che::TextureSettings texSettings, fbTexSettings;

    texSettings.Min = che::TextureResizeMode::Linear;
    texSettings.Mag = che::TextureResizeMode::Linear;

    tex = new che::Texture("textures/magik-1.png", texSettings);
    fbTex = new che::Texture(800,800,fbTexSettings);

    fbuffer = new che::Framebuffer();
    fbuffer->bindTexture(fbTex,che::TextureAttachment::Color0);

}

void Game::render(double dt) {
    fbuffer->bind();
    glClear(GL_COLOR_BUFFER_BIT);

    program->use();
    tex->bind();
    vao->bind();
    // ibo->bind();
    glDrawElements(GL_TRIANGLES,24,GL_UNSIGNED_INT,0);
    // ibo->unbind();
    vao->unbind();
    tex->unbind();

    fbuffer->unbind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    screenProgram->use();
    fbTex->bind();
    vaoFB->bind();
    // iboFB->bind();
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
    // iboFB->unbind();
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