#include "Application.hpp"
#include "Renderer/Texture.hpp"
#include "Renderer/SpriteBatch.hpp"

#include <iostream>

#include <ctime>
#include <ratio>
#include <chrono>
#include <algorithm>

che::Application::Application() {

}

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

    spdlog::warn("---------------");
    spdlog::warn("Debug message ({}): {}", id, message);

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             spdlog::warn("Source: API"); break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   spdlog::warn("Source: Window System"); break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: spdlog::warn("Source: Shader Compiler"); break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     spdlog::warn("Source: Third Party"); break;
        case GL_DEBUG_SOURCE_APPLICATION:     spdlog::warn("Source: Application"); break;
        case GL_DEBUG_SOURCE_OTHER:           spdlog::warn("Source: Other"); break;
    }

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               spdlog::warn("Type: Error"); break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: spdlog::warn("Type: Deprecated Behaviour"); break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  spdlog::warn("Type: Undefined Behaviour"); break; 
        case GL_DEBUG_TYPE_PORTABILITY:         spdlog::warn("Type: Portability"); break;
        case GL_DEBUG_TYPE_PERFORMANCE:         spdlog::warn("Type: Performance"); break;
        case GL_DEBUG_TYPE_MARKER:              spdlog::warn("Type: Marker"); break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          spdlog::warn("Type: Push Group"); break;
        case GL_DEBUG_TYPE_POP_GROUP:           spdlog::warn("Type: Pop Group"); break;
        case GL_DEBUG_TYPE_OTHER:               spdlog::warn("Type: Other"); break;
    }
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         spdlog::warn("Severity: high"); break;
        case GL_DEBUG_SEVERITY_MEDIUM:       spdlog::warn("Severity: medium"); break;
        case GL_DEBUG_SEVERITY_LOW:          spdlog::warn("Severity: low"); break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: spdlog::warn("Severity: notification"); break;
    }
    spdlog::warn("---------------");
}

int che::Application::launch() {
    spdlog::info("Launching Application");

    using namespace std::chrono;

    che::Window::init();
    che::Texture::init();

    m_Window = new che::Window(m_WindowSettings);

    m_Window->makeContextCurrent();
    m_Window->initGLAD();

    if (m_WindowSettings.opengl_debug_context) {
        m_Window->enableDebugOutput(glDebugOutput);
    }

    che::SpriteBatch::init();

    spdlog::info("Starting Application Lifecycle");


    create();
    spdlog::info("Finished Application create()");

    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    high_resolution_clock::time_point t2;
    duration<double> time_span;
    while (!m_Window->shouldWindowClose()) {
        
        t2 = high_resolution_clock::now();
        time_span = duration_cast<duration<double>>(t2 - t1);
        render(time_span.count());
        
        m_Window->swapBuffers();
        m_Window->pollEvents();
        t1 = t2;
    }
    spdlog::debug(time_span.count());

    spdlog::info("Finished Application Loop");

    destroy();
    spdlog::info("Finished Application destroy()");

    disposeAll();
    spdlog::info("Disposed all registered disposable objects");


    che::Window::terminate();
    return 0;
}

void che::Application::disposeAll() {
    for (che::Disposable* obj : m_Disposables) {
        if (obj != nullptr) {
            delete obj;
        } else {
            spdlog::warn("Encountered null disposable object");
        }
    }
}

void che::Application::removeDisposable(che::Disposable* disposable) {
    m_Disposables.erase(std::remove(m_Disposables.begin(), m_Disposables.end(), disposable), m_Disposables.end());
}