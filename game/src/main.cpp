#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include <Application.hpp>
#include <Window.hpp>
#include <Entity.hpp>
#include <Component.hpp>
#include <Shader.hpp>
#include <Buffer.hpp>

#include <vector>


#define QUAD(a,w) a,a+1,a+1+w,a+1+w,a+w,a

#define SIZE 24

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

void APIENTRY glDebugOutput(GLenum source, 
                            GLenum type, 
                            unsigned int id, 
                            GLenum severity, 
                            GLsizei length, 
                            const char *message, 
                            const void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    chengine::Window* w = chengine::Window::reverseLookup(window);
    glViewport(0,0,width,height);
}

template <typename T>
void extend(std::vector<T>& vec, T* values, uint32_t number) {
    // std::cout << "\n,[";
    for (int i = 0; i < number; i++) {
        vec.push_back(values[i]);
        // std::cout << values[i] << ", ";
    }
    // std::cout << "]";
}

template <typename T>
void printArray(T* arr, unsigned int count) {
    std::cout << "{";
    for (unsigned int i = 0 ; i < count ; i++) {
        std::cout << arr[i] << ", ";
    }
    std::cout << "}\n";
}

void char_callback(GLFWwindow* window, unsigned int codepoint) {
    std::cout << "Typed: " << (char)codepoint << "\n";
}

int main() {
    chengine::Window::init();
    chengine::WindowSettings settings;
    
    settings.opengl_debug_context = true;

    chengine::Window* window = new chengine::Window(800,800,"Noises",settings);

    window->makeContextCurrent();
    window->initGLAD();

    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    } 

    std::cout << "GLAD & GLFW Init\n";

    glClearColor(1.0f,1.0f,1.0f,1.0f);

    window->setFramebufferSizeCallback(framebuffer_size_callback);
    window->setCharCallback(char_callback);

    glEnable(GL_DEPTH_TEST);

    VertexBuffer* vboPosition = new VertexBuffer(BufferDataMode::StaticDraw);
    VertexBuffer* vboColor = new VertexBuffer(BufferDataMode::StaticDraw);
    IndexBuffer* ibo = new IndexBuffer(BufferDataMode::StaticDraw);

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
    vboPosition->bind();
    glVertexAttribPointer(0,3,GL_FLOAT,false,0,0);
    glEnableVertexAttribArray(0);
    vboPosition->unbind();
    std::cout << vboPosition->getFullDebugInfo();

    
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
    vboColor->bind();
    glVertexAttribPointer(1,4,GL_FLOAT,false,0,0);
    glEnableVertexAttribArray(1);
    vboColor->unbind();
    std::cout << vboColor->getFullDebugInfo();

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

    std::cout << ibo->getFullDebugInfo();

    glLineWidth(4);

    chengine::ShaderProgram *program = new chengine::ShaderProgram();
    (*program) << "shaders/vsh.glsl" << "shaders/fsh.glsl";

    program->linkProgram();

    std::cout << "Created shaders\n";

    while (!window->shouldWindowClose()) {
        window->pollEvents();

        GLint nTotalMemoryInKB = 0;
        glGetIntegerv( GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &nTotalMemoryInKB );

        GLint nCurAvailMemoryInKB = 0;
        glGetIntegerv( GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &nCurAvailMemoryInKB );

        // std::cout << "Total Memory: " << nTotalMemoryInKB << " kb\nRemaining Memory: " << nCurAvailMemoryInKB << " kb\n";

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        program->use();
        ibo->bind();
        glDrawElements(GL_TRIANGLES,24,GL_UNSIGNED_INT,0);

        window->swapBuffers();
    }

    chengine::Window::terminate();

    return 0;
}