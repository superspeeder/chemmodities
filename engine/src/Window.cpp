#include "Window.hpp"
#include "Exceptions.hpp"

chengine::Window::Window(uint16_t width, uint16_t height, std::string title, WindowSettings settings) {
    applySettings(settings);
    m_Width = width;
    m_Height = height;
    m_Title = title;
    m_Settings = settings;
    m_Monitor = NULL;
    m_Share = nullptr;

    createWindow();
}

chengine::Window::Window(uint16_t width, uint16_t height, std::string title, WindowSettings settings, GLFWmonitor* monitor) {
    applySettings(settings);
    m_Width = width;
    m_Height = height;
    m_Title = title;
    m_Settings = settings;
    m_Monitor = monitor;
    m_Share = nullptr;
    
    createWindow();
}

chengine::Window::Window(uint16_t width, uint16_t height, std::string title, WindowSettings settings, Window* share) {
    applySettings(settings);
    m_Width = width;
    m_Height = height;
    m_Title = title;
    m_Settings = settings;
    m_Monitor = NULL;
    m_Share = share;

    createWindow();
}

chengine::Window::Window(uint16_t width, uint16_t height, std::string title, WindowSettings settings, GLFWmonitor* monitor, Window* share) {
    applySettings(settings);
    m_Width = width;
    m_Height = height;
    m_Title = title;
    m_Settings = settings;
    m_Monitor = monitor;
    m_Share = share;
    
    createWindow();
}

void chengine::Window::applySettings(WindowSettings settings) {
    glfwDefaultWindowHints();

    if (settings.make_fullscreen) {
        if (settings.fullScreenMode == WINDOWED) {
            makeWindowedFullscreen();
        } else {
            makeWindowlessFullscreen();
        }
    } else {
        glfwWindowHint(GLFW_RED_BITS, settings.red_bits);
        glfwWindowHint(GLFW_GREEN_BITS, settings.green_bits);
        glfwWindowHint(GLFW_BLUE_BITS, settings.blue_bits);
        glfwWindowHint(GLFW_REFRESH_RATE, settings.refresh_rate);
    }

    glfwWindowHint(GLFW_RESIZABLE, settings.resizable);
    glfwWindowHint(GLFW_VISIBLE, settings.visible);
    glfwWindowHint(GLFW_DECORATED,  settings.decorated);
    glfwWindowHint(GLFW_FOCUSED, settings.focused);
    glfwWindowHint(GLFW_AUTO_ICONIFY, settings.auto_iconify);
    glfwWindowHint(GLFW_FLOATING, settings.floating);
    glfwWindowHint(GLFW_MAXIMIZED, settings.maximized);
    glfwWindowHint(GLFW_CENTER_CURSOR, settings.center_cursor);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, settings.transparent_framebuffer);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, settings.focus_on_show);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, settings.scale_to_monitor);
    glfwWindowHint(GLFW_STEREO, settings.stereo);
    glfwWindowHint(GLFW_SRGB_CAPABLE, settings.srgb_capable);
    glfwWindowHint(GLFW_DOUBLEBUFFER, settings.doublebuffer);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, settings.opengl_forward_compat);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, settings.opengl_debug_context);

    glfwWindowHint(GLFW_DEPTH_BITS, settings.depth_bits);
    glfwWindowHint(GLFW_STENCIL_BITS, settings.stencil_bits);
    glfwWindowHint(GLFW_ACCUM_RED_BITS, settings.accum_red_bits);
    glfwWindowHint(GLFW_ACCUM_GREEN_BITS, settings.accum_green_bits);
    glfwWindowHint(GLFW_ACCUM_BLUE_BITS, settings.accum_blue_bits);
    glfwWindowHint(GLFW_ACCUM_ALPHA_BITS, settings.accum_alpha_bits);
    glfwWindowHint(GLFW_AUX_BUFFERS, settings.aux_buffers);
    glfwWindowHints(GLFW_SAMPLES, settings.samples);
    glfwWindowHint(GLFW_CLIENT_API, settings.client_api);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, settings.context_creation_api);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.context_version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings.context_version_minor);
    glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, settings.context_robustness);
    glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, settings.context_release_behavior);
    glfwWindowHint(GLFW_OPENGL_PROFILE, settings.opengl_profile);
}

void chengine::Window::init() {
    if (!glfwInit()) {
        throw glfw_error("Initialization Failed");
    }
}

void chengine::Window::createWindow() {
    if (m_Share == nullptr) {
        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), m_Monitor, NULL);
    } else {
        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), m_Monitor, m_Share->m_Window);
    }

    s_ReverseLookupMap[m_Window] = this;
}

chengine::Window* chengine::Window::reverseLookup(GLFWwindow* window) {
    return s_ReverseLookupMap[window];
} 

void chengine::Window::pollEvents() {
    glfwPollEvents();
}

void chengine::Window::swapBuffers() {
    glfwSwapBuffers(m_Window);
}

void chengine::Window::setSwapInterval(int swap_interval) {
    glfwSwapInterval(swap_interval);
}

void chengine::Window::makeWindowedFullscreen() {
    if (m_Monitor == NULL) {
        m_Monitor = glfwGetPrimaryMonitor();
    }

    if (m_Window == NULL) { // Window doesnt exist yet
        const GLFWvidmode* mode = glfwGetVideoMode(m_Monitor);

        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    } else {
        const GLFWvidmode* mode = glfwGetVideoMode(m_Monitor);

        m_Width = mode->width;
        m_Height = mode->height;

        glfwSetWindowMonitor(m_Window, m_Monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
}

void chengine::Window::makeWindowlessFullscreen() {
    if (m_Monitor != NULL) {
        return;
    }

    m_Monitor = glfwGetPrimaryMonitor();

    if (m_Window != NULL) {
        glfwSetWindowMonitor(m_Window, m_Monitor, 0, 0, m_Width, m_Height, m_WindowSettings.refresh_rate)
    }
}

chengine::Window::~Window() {
    glfwDestroyWindow(m_Window);
}

void chengine::Window::waitEvents() {
    glfwWaitEvents();
}

void chengine::Window::waitEventsTimeout(double timeout) {
    glfwWaitEventsTimeout(timeout);
}

void chengine::Window::postEmptyEvent() {
    glfwPostEmptyEvent();
}

void chengine::Window::setKeyCallback(GLFWkeyfun function) {
    glfwSetKeyCallback(m_Window, function);
}

Input::State chengine::Window::getKeyState(int key) {
    return static_cast<Input::State>(glfwGetKey(m_Window, key));
}

void chengine::Window::setInputMode(chengine::Input::Mode mode, int value) {
    glfwSetInputMode(static_cast<int>(mode), value);
}

void chengine::Window::enableStickyKeys() {
    setInputMode(chengine::Input::Mode::StickyKeys, GLFW_TRUE);
}

void chengine::Window::disableStickyKeys() {
    setInputMode(chengine::Input::Mode::StickyKeys, GLFW_FALSE);
}

void chengine::Window::enableStickyMouseButtons() {
    setInputMode(chengine::Input::Mode::StickyMouseButtons, GLFW_TRUE);
}

void chengine::Window::disableStickyMouseButtons() {
    setInputMode(chengine::Input::Mode::StickyMouseButtons, GLFW_FALSE);
}

void chengine::Window::readLockKeyMods() {
    setInputMode(chengine::Input::Mode::LockKeyMods, GLFW_TRUE);
}

void chengine::Window::ignoreLockKeyMods() {
    setInputMode(chengine::Input::Mode::LockKeyMods, GLFW_FALSE);
}

void chengine::Window::setCursorMode(chengine::Input::CursorMode mode) {
    setInputMode(chengine::Input::Mode::Cursor, static_cast<int>(mode));
}

void chengine::Window::enableRawMouseMotion() {
    setInputMode(chengine::Input::Mode::RawMouseMotion, GLFW_TRUE);
}

void chengine::Window::disableRawMouseMotion() {
    setInputMode(chengine::Input::Mode::StickyKeys, GLFW_FALSE);
}

void chengine::Window::setCursor(chengine::Cursor* cursor) {
    glfwSetCursor(m_Window, cursor->m_Cursor);
}

void chengine::Window::setCharCallback(GLFWcharfun callback) {
    glfwSetCharCallback(m_Window, callback);
}

void chengine::Window::setCursorPosCallback(GLFWcursorposfun callback) {
    glfwSetCursorPosCallback(m_Window, callback);
}

void chengine::Window::setCursorEnterCallback(GLFWcursorenterfun callback) {
    glfwSetCursorEnterCallback(m_Window, callback);
}

void chengine::Window::getCursorPos(double *x, double *y) {
    glfwGetCursorPos(m_Window, x, y);
}

int chengine::Window::getWindowAttrib(chengine::Input::WindowAttrib attrib) {
    return glfwGetWindowAttrib(m_Window, static_cast<int>(attrib));
}

void chengine::Window::setMouseButtonCallback(GLFWmousebuttonfun callback) {
    glfwSetMouseButtonCallback(m_Window, callback);
}

chengine::Input::State chengine::Window::getMouseButton(chengine::Input::MouseButton button) {
    return static_cast<chengine::Input:State>(glfwGetMouseButton(m_Window, static_cast<int>(button)))
}

chengine::Input::getScancode(int key) {
    return glfwGetKeyScancode(key);
}


chengine::Cursor::Cursor(unsigned char* pixels, int width, int height, int x_hot, int y_hot) {
    GLFWimage image;
    image.pixels = pixels;
    image.width = width;
    image.height = height;

    m_Cursor = glfwCreateCursor(&image, x_hot, y_hot);
}

chengine::Cursor::Cursor(int standard_type) {
    m_Cursor = glfwCreateStandardCursor(standard_type);
}

chengine::Cursor::~Cursor() {
    glfwDestroyCursor(m_Cursor);
}

