#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include <Application.hpp>
#include <Window.hpp>
#include <Entity.hpp>
#include <Component.hpp>
#include <Shader.hpp>

#include <vector>


#define QUAD(a,w) a,a+1,a+1+w,a+1+w,a+w,a

#define SIZE 24


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

void checkIndicies(std::vector<float> vert, unsigned int* ind, unsigned int count) {
    std::cout << "\n{\n";
    for (int i = 0; i < count; i++) {
        if (i % 3 == 0 && i != 0) {
            std::cout << "\n";
        }
        std::cout << ind[i] << " ";
    }
    std::cout << "\n} -- [ \n";

    for (int i = 0; i < count; i++) {
        std::cout << ind[i] << ": " << "(" << vert.at(ind[i]*3) << ", " << vert.at(ind[i]*3+1) << ");\n";
    }
    std::cout << "]\n";
}

template <typename T>
void printArray(T* arr, unsigned int count) {
    std::cout << "{";
    for (unsigned int i = 0 ; i < count ; i++) {
        std::cout << arr[i] << ", ";
    }
    std::cout << "}\n";
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

    glClearColor(1.0f,1.0f,1.0f,1.0f);

    window->setFramebufferSizeCallback(framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);

    std::vector<float> vertices;
    std::vector<float> colors;
    std::vector<float> uvs;
    std::vector<unsigned int> indicies;

    GLuint vbo_positions, vbo_colors, vbo_uvs, ibo,vao;

    glCreateVertexArrays(1,&vao);
    glBindVertexArray(vao);

    glGenBuffers(1,&vbo_positions);
    glGenBuffers(1,&vbo_colors);
    glGenBuffers(1,&vbo_uvs);
    glGenBuffers(1,&ibo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), (const void*)(vertices.data()), GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT,false,2 * sizeof(float),nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colors.size(), (const void*)(colors.data()), GL_STATIC_DRAW);
    glVertexAttribPointer(1,4,GL_FLOAT,false,4 * sizeof(float),nullptr);
    glEnableVertexAttribArray(1);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicies.size(), (const void*)(indicies.data()), GL_STATIC_DRAW);
    glBindVertexArray(0);

    glLineWidth(4);

    chengine::ShaderProgram *program = new chengine::ShaderProgram();
    (*program) << "shaders/vsh.glsl" << "shaders/fsh.glsl" << "shaders/gsh.glsl";

    program->linkProgram();

    while (!window->shouldWindowClose()) {
        window->pollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        program->use();
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES,indicies.size(),GL_UNSIGNED_INT,nullptr);
        glBindVertexArray(0);

        window->swapBuffers();
    }

    chengine::Window::terminate();

    return 0;
}