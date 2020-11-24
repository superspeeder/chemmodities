#include "Renderer/Window.hpp"
#include "Utils/Exceptions.hpp"

#include <string>
#include <cinttypes>

#include <glad/glad.h>

// Window Static
void che::Window::init() {
    if (!glfwInit()) {
        throw glfw_error("Initialization Failed");
    }
}

void che::Window::terminate() {
    glfwTerminate();
}

che::Window* che::Window::reverseLookup(GLFWwindow* window) {
    return s_ReverseLookupMap[window];
}

// Window Constsructors
che::Window::Window(uint16_t width, uint16_t height, std::string title, WindowSettings settings) {
    applySettings(settings);
    m_Width = width;
    m_Height = height;
    m_Title = title;
    m_WindowSettings = settings;
    m_Monitor = NULL;
    m_Share = nullptr;

    createWindow();
}

che::Window::Window(uint16_t width, uint16_t height, std::string title, WindowSettings settings, GLFWmonitor* monitor) {
    applySettings(settings);
    m_Width = width;
    m_Height = height;
    m_Title = title;
    m_WindowSettings = settings;
    m_Monitor = monitor;
    m_Share = nullptr;
    
    createWindow();
}

che::Window::Window(uint16_t width, uint16_t height, std::string title, WindowSettings settings, Window* share) {
    applySettings(settings);
    m_Width = width;
    m_Height = height;
    m_Title = title;
    m_WindowSettings = settings;
    m_Monitor = NULL;
    m_Share = share;

    createWindow();
}

che::Window::Window(uint16_t width, uint16_t height, std::string title, WindowSettings settings, GLFWmonitor* monitor, Window* share) {
    applySettings(settings);
    m_Width = width;
    m_Height = height;
    m_Title = title;
    m_WindowSettings = settings;
    m_Monitor = monitor;
    m_Share = share;
    
    createWindow();
}

che::Window::Window(che::WindowSettings settings) {
    applySettings(settings);
    m_Width = settings.width;
    m_Height = settings.height;
    m_Title = settings.title;
    m_Monitor = settings.monitor;
    m_Share = settings.share;

    createWindow();
}

// Window Operators
che::Window::~Window() {
    glfwDestroyWindow(m_Window);
}


// Window setup
void che::Window::createWindow() {
    if (m_Share == nullptr) {
        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), m_Monitor, NULL);
    } else {
        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), m_Monitor, m_Share->m_Window);
    }

    s_ReverseLookupMap[m_Window] = this;
}

void che::Window::initGLAD() {
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    if(!gladLoadGL()) {
        glfwTerminate();
        std::cerr << "GLAD IS FUCKEROO\n";
    }
}

void che::Window::applySettings(WindowSettings settings) {
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
    glfwWindowHint(GLFW_SAMPLES, settings.samples);
    glfwWindowHint(GLFW_CLIENT_API, settings.client_api);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, settings.context_creation_api);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.context_version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings.context_version_minor);
    glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, settings.context_robustness);
    glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, settings.context_release_behavior);
    glfwWindowHint(GLFW_OPENGL_PROFILE, settings.opengl_profile);
}

void che::Window::makeWindowedFullscreen() {
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

void che::Window::makeWindowlessFullscreen() {
    if (m_Monitor != NULL) {
        return;
    }

    m_Monitor = glfwGetPrimaryMonitor();

    if (m_Window != NULL) {
        glfwSetWindowMonitor(m_Window, m_Monitor, 0, 0, m_Width, m_Height, m_WindowSettings.refresh_rate);
    }
}

// Basic Drawing
void che::Window::swapBuffers() {
    glfwSwapBuffers(m_Window);
}

void che::Window::setSwapInterval(int swap_interval) {
    glfwSwapInterval(swap_interval);
}

// Basic Events
void che::Window::pollEvents() {
    glfwPollEvents();
}

void che::Window::waitEvents() {
    glfwWaitEvents();
}

void che::Window::waitEventsTimeout(double timeout) {
    glfwWaitEventsTimeout(timeout);
}

void che::Window::postEmptyEvent() {
    glfwPostEmptyEvent();
}

// Window basic input

void che::Window::setInputMode(che::Input::Mode mode, int value) {
    glfwSetInputMode(m_Window, static_cast<int>(mode), value);
}


// Window Key Input

void che::Window::setKeyCallback(GLFWkeyfun function) {
    glfwSetKeyCallback(m_Window, function);
}

che::Input::State che::Window::getKeyState(int key) {
    return static_cast<Input::State>(glfwGetKey(m_Window, key));
}

void che::Window::enableStickyKeys() {
    setInputMode(che::Input::Mode::StickyKeys, GLFW_TRUE);
}

void che::Window::disableStickyKeys() {
    setInputMode(che::Input::Mode::StickyKeys, GLFW_FALSE);
}

void che::Window::readLockKeyMods() {
    setInputMode(che::Input::Mode::LockKeyMods, GLFW_TRUE);
}

void che::Window::ignoreLockKeyMods() {
    setInputMode(che::Input::Mode::LockKeyMods, GLFW_FALSE);
}

void che::Window::setCharCallback(GLFWcharfun callback) {
    glfwSetCharCallback(m_Window, callback);
}


// Window Mouse Input

void che::Window::setMouseButtonCallback(GLFWmousebuttonfun callback) {
    glfwSetMouseButtonCallback(m_Window, callback);
}

che::Input::State che::Window::getMouseButton(che::Input::MouseButton button) {
    return static_cast<che::Input::State>(glfwGetMouseButton(m_Window, static_cast<int>(button)));
}

void che::Window::enableStickyMouseButtons() {
    setInputMode(che::Input::Mode::StickyMouseButtons, GLFW_TRUE);
}

void che::Window::disableStickyMouseButtons() {
    setInputMode(che::Input::Mode::StickyMouseButtons, GLFW_FALSE);
}


void che::Window::setCursorEnterCallback(GLFWcursorenterfun callback) {
    glfwSetCursorEnterCallback(m_Window, callback);
}


void che::Window::setCursorPosCallback(GLFWcursorposfun callback) {
    glfwSetCursorPosCallback(m_Window, callback);
}

void che::Window::getCursorPos(double *x, double *y) {
    glfwGetCursorPos(m_Window, x, y);
}


void che::Window::setScrollCallback(GLFWscrollfun callback) {
    glfwSetScrollCallback(m_Window, callback);
}


void che::Window::setCursorMode(che::Input::CursorMode mode) {
    setInputMode(che::Input::Mode::Cursor, static_cast<int>(mode));
}

void che::Window::enableRawMouseMotion() {
    setInputMode(che::Input::Mode::RawMouseMotion, GLFW_TRUE);
}

void che::Window::disableRawMouseMotion() {
    setInputMode(che::Input::Mode::StickyKeys, GLFW_FALSE);
}

bool che::Window::isRawMouseMotionSupported() {
    return glfwRawMouseMotionSupported();
}


void che::Window::setCursor(che::Cursor* cursor) {
    glfwSetCursor(m_Window, cursor->m_Cursor);
}

// Clipboard
const char* che::Window::getClipboardString() {
    return glfwGetClipboardString(m_Window);
}

void che::Window::setClipboardString(const char* string) {
    glfwSetClipboardString(m_Window, string);
}

// Basic Window Info

int che::Window::getWindowAttrib(che::Input::WindowAttrib attrib) {
    return glfwGetWindowAttrib(m_Window, static_cast<int>(attrib));
}

void che::Window::setWindowAttrib(Input::WindowAttrib attrib, int value) {
    glfwSetWindowAttrib(m_Window, static_cast<int>(attrib), value);
}

bool che::Window::shouldWindowClose() {
    return glfwWindowShouldClose(m_Window);
}

void che::Window::closeWindow() {
    setWindowClose(true);
}

void che::Window::setWindowClose(bool value) {
    glfwSetWindowShouldClose(m_Window, value);
}

void che::Window::setWindowCloseCallback(GLFWwindowclosefun callback) {
    glfwSetWindowCloseCallback(m_Window, callback);
}

void che::Window::resizeWindow(int x, int y) {
    glfwSetWindowSize(m_Window, x, y);
}

void che::Window::setWindowResizeCallback(GLFWwindowsizefun callback) {
    glfwSetWindowSizeCallback(m_Window, callback);
}

glm::vec2 che::Window::getWindowSize() {
    glm::ivec2 s;
    getWindowSize(&(s.x), &(s.y));
    return s;
}

void che::Window::getWindowSize(int *x, int *y) {
    glfwGetWindowSize(m_Window,x,y);
}

void che::Window::getWindowFrameSize(int *left, int *top, int *right, int *bottom) {
    glfwGetWindowFrameSize(m_Window, left, top, right, bottom);
}

void che::Window::setFramebufferSizeCallback(GLFWframebuffersizefun callback) {
    glfwSetFramebufferSizeCallback(m_Window, callback);
}

glm::vec2 che::Window::getFramebufferSize() {
    glm::ivec2 s;
    getFramebufferSize(&(s.x),&(s.y));
    return s;
}

void che::Window::getFramebufferSize(int *x, int *y) {
    glfwGetFramebufferSize(m_Window,x,y);
}

void che::Window::getContentScale(float *x, float *y) {
    glfwGetWindowContentScale(m_Window,x,y);
}

glm::vec2 che::Window::getContentScale() {
    glm::vec2 s;
    getContentScale(&(s.x), &(s.y));
    return s;
}

void che::Window::setContentScaleCallback(GLFWwindowcontentscalefun callback) {
    glfwSetWindowContentScaleCallback(m_Window, callback);
}

void che::Window::setWindowSizeLimits(int x_min, int y_min, int x_max, int y_max) {
    glfwSetWindowSizeLimits(m_Window, x_min,y_min,x_max,y_max);
}

void che::Window::setAspectRatio(int x, int y) {
    glfwSetWindowAspectRatio(m_Window, x,y);
}

void che::Window::setWindowPos(int x, int y) {
    glfwSetWindowPos(m_Window, x,y);
}

void che::Window::getWindowPos(int *x, int *y) {
    glfwGetWindowPos(m_Window, x,y);
}

glm::vec2 che::Window::getWindowPos() {
    glm::ivec2 p;
    getWindowPos(&(p.x),&(p.y));
    return p;
}

void che::Window::setWindowPosCallback(GLFWwindowposfun callback) {
    glfwSetWindowPosCallback(m_Window, callback);
}

void che::Window::setWindowTitle(const char* title) {
    glfwSetWindowTitle(m_Window, title);
}

void che::Window::setIcon(GLFWimage* icons, int count) {
    glfwSetWindowIcon(m_Window, count, icons);
}

GLFWmonitor* che::Window::getMonitor() {
    return glfwGetWindowMonitor(m_Window);
}

const GLFWvidmode* che::Window::getVidMode() {
    return glfwGetVideoMode(getMonitor());
}

void che::Window::setWindowMonitor(GLFWmonitor* monitor, int xpos, int ypos, int width, int height, int refreshrate) {
    glfwSetWindowMonitor(m_Window, monitor, xpos,ypos, width,height, refreshrate);
}

void che::Window::restoreWindow() {
    glfwRestoreWindow(m_Window);
}

void che::Window::iconify() {
    glfwIconifyWindow(m_Window);
}

void che::Window::setIconifyCallback(GLFWwindowiconifyfun callback) {
    glfwSetWindowIconifyCallback(m_Window, callback);
}

void che::Window::maximize() {
    glfwMaximizeWindow(m_Window);
}

void che::Window::setMaximizeCallback(GLFWwindowmaximizefun callback) {
    glfwSetWindowMaximizeCallback(m_Window, callback);
}

void che::Window::hide() {
    glfwHideWindow(m_Window);
}

void che::Window::show() {
    glfwShowWindow(m_Window);
}

bool che::Window::isVisible() {
    return getWindowAttrib(che::Input::WindowAttrib::Visible);
}

void che::Window::focus() {
    glfwFocusWindow(m_Window);
}

void che::Window::setFocusCallback(GLFWwindowfocusfun callback) {
    glfwSetWindowFocusCallback(m_Window,callback);
}

bool che::Window::isFocused() {
    return getWindowAttrib(che::Input::WindowAttrib::Focused);
}

void che::Window::requestAttention() {
    glfwRequestWindowAttention(m_Window);
}

void che::Window::setRefreshCallback(GLFWwindowrefreshfun callback) {
    glfwSetWindowRefreshCallback(m_Window, callback);
}

void che::Window::setOpacity(float opacity) {
    glfwSetWindowOpacity(m_Window,opacity);
}

float che::Window::getOpacity() {
    return glfwGetWindowOpacity(m_Window);
}

void che::Window::setUserPointer(void* ptr) {
    glfwSetWindowUserPointer(m_Window, ptr);
}

void* che::Window::getUserPointer() {
    return glfwGetWindowUserPointer(m_Window);
}

void che::Window::makeContextCurrent() {
    glfwMakeContextCurrent(m_Window);
}

che::Window* che::Window::getCurrentContext() {
    return reverseLookup(glfwGetCurrentContext());
}

void che::Window::enableDebugOutput(GLDEBUGPROC gldo) {
    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
        glDebugMessageCallback(gldo, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    } 
}

// Basic windowless input functions

const int che::Input::getScancode(int key) {
    return glfwGetKeyScancode(key);
}

const char* che::Input::getKeyName(int key, int scancode) {
    return glfwGetKeyName(key, scancode);
}

double che::Input::getTime() {
    return glfwGetTime();
}

void che::Input::setTime(double newTime) {
    glfwSetTime(newTime);
}

uint64_t che::Input::getTimerValue() {
    return glfwGetTimerFrequency();
}

uint64_t che::Input::getTimerFrequency() {
    return glfwGetTimerFrequency();
}

// System Clipboard
const char* che::Input::getSystemClipboardString() {
    return glfwGetClipboardString(NULL);
}

void che::Input::setSystemClipboardString(const char* string) {
    glfwSetClipboardString(NULL, string);
}

        

// Cursor Class

che::Cursor::Cursor(unsigned char* pixels, int width, int height, int x_hot, int y_hot) {
    GLFWimage image;
    image.pixels = pixels;
    image.width = width;
    image.height = height;

    m_Cursor = glfwCreateCursor(&image, x_hot, y_hot);
}

che::Cursor::Cursor(int standard_type) {
    m_Cursor = glfwCreateStandardCursor(standard_type);
}

che::Cursor::~Cursor() {
    glfwDestroyCursor(m_Cursor);
}

// Joystick Class
che::Input::Joystick::Joystick(unsigned int id) : m_JoystickId(id) {
}

che::Input::JoyAxisValues che::Input::Joystick::getAxes() {
    JoyAxisValues values;
    values.values = glfwGetJoystickAxes(m_JoystickId, &(values.count));
    return values;
}

che::Input::JoyButtonValues che::Input::Joystick::getButtons() {
    JoyButtonValues values;
    values.buttons = glfwGetJoystickButtons(m_JoystickId, &(values.count));
    return values;
}

che::Input::JoyHatValues che::Input::Joystick::getHats() {
    JoyHatValues values;
    values.hats = glfwGetJoystickHats(m_JoystickId, &(values.count));
    return values;
}


const char* che::Input::Joystick::getName() {
    return glfwGetJoystickName(m_JoystickId);
}

void che::Input::Joystick::setConfigurationCallback(GLFWjoystickfun callback) {
    glfwSetJoystickCallback(callback);
}

// Gamepad Class

che::Input::Gamepad::Gamepad(unsigned int id) : che::Input::Joystick::Joystick(id) {
    if (!glfwJoystickIsGamepad(id)) {
        throw glfw_error(("Joystick " + std::to_string(id) + " is not a valid gamepad").c_str());
    }
}

const char* che::Input::Gamepad::getName() {
    return glfwGetGamepadName(m_JoystickId);
}

GLFWgamepadstate che::Input::Gamepad::getState() {
    GLFWgamepadstate ret;
    glfwGetGamepadState(m_JoystickId, &ret);
    return ret;
}