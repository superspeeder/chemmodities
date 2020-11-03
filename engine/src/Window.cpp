#include "Window.hpp"
#include "Exceptions.hpp"
#include <string>
#include <cinttypes>
#include <glad/glad.h>

// Window Static
void chengine::Window::init() {
    if (!glfwInit()) {
        throw glfw_error("Initialization Failed");
    }
}

chengine::Window* chengine::Window::reverseLookup(GLFWwindow* window) {
    return s_ReverseLookupMap[window];
}

// Window Constsructors
chengine::Window::Window(uint16_t width, uint16_t height, std::string title, WindowSettings settings) {
    applySettings(settings);
    m_Width = width;
    m_Height = height;
    m_Title = title;
    m_WindowSettings = settings;
    m_Monitor = NULL;
    m_Share = nullptr;

    createWindow();
}

chengine::Window::Window(uint16_t width, uint16_t height, std::string title, WindowSettings settings, GLFWmonitor* monitor) {
    applySettings(settings);
    m_Width = width;
    m_Height = height;
    m_Title = title;
    m_WindowSettings = settings;
    m_Monitor = monitor;
    m_Share = nullptr;
    
    createWindow();
}

chengine::Window::Window(uint16_t width, uint16_t height, std::string title, WindowSettings settings, Window* share) {
    applySettings(settings);
    m_Width = width;
    m_Height = height;
    m_Title = title;
    m_WindowSettings = settings;
    m_Monitor = NULL;
    m_Share = share;

    createWindow();
}

chengine::Window::Window(uint16_t width, uint16_t height, std::string title, WindowSettings settings, GLFWmonitor* monitor, Window* share) {
    applySettings(settings);
    m_Width = width;
    m_Height = height;
    m_Title = title;
    m_WindowSettings = settings;
    m_Monitor = monitor;
    m_Share = share;
    
    createWindow();
}

// Window Operators
chengine::Window::~Window() {
    glfwDestroyWindow(m_Window);
}


// Window setup
void chengine::Window::createWindow() {
    if (m_Share == nullptr) {
        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), m_Monitor, NULL);
    } else {
        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), m_Monitor, m_Share->m_Window);
    }

    s_ReverseLookupMap[m_Window] = this;
}

void chengine::Window::initGLAD() {
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    if(!gladLoadGL()) {
        glfwTerminate();
        std::cerr << "GLAD IS FUCKEROO\n";
    }
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
    glfwWindowHint(GLFW_SAMPLES, settings.samples);
    glfwWindowHint(GLFW_CLIENT_API, settings.client_api);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, settings.context_creation_api);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.context_version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings.context_version_minor);
    glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, settings.context_robustness);
    glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, settings.context_release_behavior);
    glfwWindowHint(GLFW_OPENGL_PROFILE, settings.opengl_profile);
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
        glfwSetWindowMonitor(m_Window, m_Monitor, 0, 0, m_Width, m_Height, m_WindowSettings.refresh_rate);
    }
}

// Basic Drawing
void chengine::Window::swapBuffers() {
    glfwSwapBuffers(m_Window);
}

void chengine::Window::setSwapInterval(int swap_interval) {
    glfwSwapInterval(swap_interval);
}

// Basic Events
void chengine::Window::pollEvents() {
    glfwPollEvents();
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

// Window basic input

void chengine::Window::setInputMode(chengine::Input::Mode mode, int value) {
    glfwSetInputMode(m_Window, static_cast<int>(mode), value);
}


// Window Key Input

void chengine::Window::setKeyCallback(GLFWkeyfun function) {
    glfwSetKeyCallback(m_Window, function);
}

chengine::Input::State chengine::Window::getKeyState(int key) {
    return static_cast<Input::State>(glfwGetKey(m_Window, key));
}

void chengine::Window::enableStickyKeys() {
    setInputMode(chengine::Input::Mode::StickyKeys, GLFW_TRUE);
}

void chengine::Window::disableStickyKeys() {
    setInputMode(chengine::Input::Mode::StickyKeys, GLFW_FALSE);
}

void chengine::Window::readLockKeyMods() {
    setInputMode(chengine::Input::Mode::LockKeyMods, GLFW_TRUE);
}

void chengine::Window::ignoreLockKeyMods() {
    setInputMode(chengine::Input::Mode::LockKeyMods, GLFW_FALSE);
}

void chengine::Window::setCharCallback(GLFWcharfun callback) {
    glfwSetCharCallback(m_Window, callback);
}


// Window Mouse Input

void chengine::Window::setMouseButtonCallback(GLFWmousebuttonfun callback) {
    glfwSetMouseButtonCallback(m_Window, callback);
}

chengine::Input::State chengine::Window::getMouseButton(chengine::Input::MouseButton button) {
    return static_cast<chengine::Input::State>(glfwGetMouseButton(m_Window, static_cast<int>(button)));
}

void chengine::Window::enableStickyMouseButtons() {
    setInputMode(chengine::Input::Mode::StickyMouseButtons, GLFW_TRUE);
}

void chengine::Window::disableStickyMouseButtons() {
    setInputMode(chengine::Input::Mode::StickyMouseButtons, GLFW_FALSE);
}


void chengine::Window::setCursorEnterCallback(GLFWcursorenterfun callback) {
    glfwSetCursorEnterCallback(m_Window, callback);
}


void chengine::Window::setCursorPosCallback(GLFWcursorposfun callback) {
    glfwSetCursorPosCallback(m_Window, callback);
}

void chengine::Window::getCursorPos(double *x, double *y) {
    glfwGetCursorPos(m_Window, x, y);
}


void chengine::Window::setScrollCallback(GLFWscrollfun callback) {
    glfwSetScrollCallback(m_Window, callback);
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

bool chengine::Window::isRawMouseMotionSupported() {
    return glfwRawMouseMotionSupported();
}


void chengine::Window::setCursor(chengine::Cursor* cursor) {
    glfwSetCursor(m_Window, cursor->m_Cursor);
}

// Clipboard
const char* chengine::Window::getClipboardString() {
    return glfwGetClipboardString(m_Window);
}

void chengine::Window::setClipboardString(const char* string) {
    glfwSetClipboardString(m_Window, string);
}

// Basic Window Info

int chengine::Window::getWindowAttrib(chengine::Input::WindowAttrib attrib) {
    return glfwGetWindowAttrib(m_Window, static_cast<int>(attrib));
}

void chengine::Window::setWindowAttrib(Input::WindowAttrib attrib, int value) {
    glfwSetWindowAttrib(m_Window, static_cast<int>(attrib), value);
}

bool chengine::Window::shouldWindowClose() {
    return glfwWindowShouldClose(m_Window);
}

void chengine::Window::closeWindow() {
    setWindowClose(true);
}

void chengine::Window::setWindowClose(bool value) {
    glfwSetWindowShouldClose(m_Window, value);
}

void chengine::Window::setWindowCloseCallback(GLFWwindowclosefun callback) {
    glfwSetWindowCloseCallback(m_Window, callback);
}

void chengine::Window::resizeWindow(int x, int y) {
    glfwSetWindowSize(m_Window, x, y);
}

void chengine::Window::setWindowResizeCallback(GLFWwindowsizefun callback) {
    glfwSetWindowSizeCallback(m_Window, callback);
}

glm::vec2 chengine::Window::getWindowSize() {
    glm::ivec2 s;
    getWindowSize(&(s.x), &(s.y));
    return s;
}

void chengine::Window::getWindowSize(int *x, int *y) {
    glfwGetWindowSize(m_Window,x,y);
}

void chengine::Window::getWindowFrameSize(int *left, int *top, int *right, int *bottom) {
    glfwGetWindowFrameSize(m_Window, left, top, right, bottom);
}

void chengine::Window::setFramebufferSizeCallback(GLFWframebuffersizefun callback) {
    glfwSetFramebufferSizeCallback(m_Window, callback);
}

glm::vec2 chengine::Window::getFramebufferSize() {
    glm::ivec2 s;
    getFramebufferSize(&(s.x),&(s.y));
    return s;
}

void chengine::Window::getFramebufferSize(int *x, int *y) {
    glfwGetFramebufferSize(m_Window,x,y);
}

void chengine::Window::getContentScale(float *x, float *y) {
    glfwGetWindowContentScale(m_Window,x,y);
}

glm::vec2 chengine::Window::getContentScale() {
    glm::vec2 s;
    getContentScale(&(s.x), &(s.y));
    return s;
}

void chengine::Window::setContentScaleCallback(GLFWwindowcontentscalefun callback) {
    glfwSetWindowContentScaleCallback(m_Window, callback);
}

void chengine::Window::setWindowSizeLimits(int x_min, int y_min, int x_max, int y_max) {
    glfwSetWindowSizeLimits(m_Window, x_min,y_min,x_max,y_max);
}

void chengine::Window::setAspectRatio(int x, int y) {
    glfwSetWindowAspectRatio(m_Window, x,y);
}

void chengine::Window::setWindowPos(int x, int y) {
    glfwSetWindowPos(m_Window, x,y);
}

void chengine::Window::getWindowPos(int *x, int *y) {
    glfwGetWindowPos(m_Window, x,y);
}

glm::vec2 chengine::Window::getWindowPos() {
    glm::ivec2 p;
    getWindowPos(&(p.x),&(p.y));
    return p;
}

void chengine::Window::setWindowPosCallback(GLFWwindowposfun callback) {
    glfwSetWindowPosCallback(m_Window, callback);
}

void chengine::Window::setWindowTitle(const char* title) {
    glfwSetWindowTitle(m_Window, title);
}

void chengine::Window::setIcon(GLFWimage* icons, int count) {
    glfwSetWindowIcon(m_Window, count, icons);
}

GLFWmonitor* chengine::Window::getMonitor() {
    return glfwGetWindowMonitor(m_Window);
}

const GLFWvidmode* chengine::Window::getVidMode() {
    return glfwGetVideoMode(getMonitor());
}

void chengine::Window::setWindowMonitor(GLFWmonitor* monitor, int xpos, int ypos, int width, int height, int refreshrate) {
    glfwSetWindowMonitor(m_Window, monitor, xpos,ypos, width,height, refreshrate);
}

void chengine::Window::restoreWindow() {
    glfwRestoreWindow(m_Window);
}

void chengine::Window::iconify() {
    glfwIconifyWindow(m_Window);
}

void chengine::Window::setIconifyCallback(GLFWwindowiconifyfun callback) {
    glfwSetWindowIconifyCallback(m_Window, callback);
}

void chengine::Window::maximize() {
    glfwMaximizeWindow(m_Window);
}

void chengine::Window::setMaximizeCallback(GLFWwindowmaximizefun callback) {
    glfwSetWindowMaximizeCallback(m_Window, callback);
}

void chengine::Window::hide() {
    glfwHideWindow(m_Window);
}

void chengine::Window::show() {
    glfwShowWindow(m_Window);
}

bool chengine::Window::isVisible() {
    return getWindowAttrib(chengine::Input::WindowAttrib::Visible);
}

void chengine::Window::focus() {
    glfwFocusWindow(m_Window);
}

void chengine::Window::setFocusCallback(GLFWwindowfocusfun callback) {
    glfwSetWindowFocusCallback(m_Window,callback);
}

bool chengine::Window::isFocused() {
    return getWindowAttrib(chengine::Input::WindowAttrib::Focused);
}

void chengine::Window::requestAttention() {
    glfwRequestWindowAttention(m_Window);
}

void chengine::Window::setRefreshCallback(GLFWwindowrefreshfun callback) {
    glfwSetWindowRefreshCallback(m_Window, callback);
}

void chengine::Window::setOpacity(float opacity) {
    glfwSetWindowOpacity(m_Window,opacity);
}

float chengine::Window::getOpacity() {
    return glfwGetWindowOpacity(m_Window);
}

void chengine::Window::setUserPointer(void* ptr) {
    glfwSetWindowUserPointer(m_Window, ptr);
}

void* chengine::Window::getUserPointer() {
    return glfwGetWindowUserPointer(m_Window);
}

// Basic windowless input functions

const int chengine::Input::getScancode(int key) {
    return glfwGetKeyScancode(key);
}

const char* chengine::Input::getKeyName(int key, int scancode) {
    return glfwGetKeyName(key, scancode);
}

double chengine::Input::getTime() {
    return glfwGetTime();
}

void chengine::Input::setTime(double newTime) {
    glfwSetTime(newTime);
}

uint64_t chengine::Input::getTimerValue() {
    return glfwGetTimerFrequency();
}

uint64_t chengine::Input::getTimerFrequency() {
    return glfwGetTimerFrequency();
}

// System Clipboard
const char* chengine::Input::getSystemClipboardString() {
    return glfwGetClipboardString(NULL);
}

void chengine::Input::setSystemClipboardString(const char* string) {
    glfwSetClipboardString(NULL, string);
}

        

// Cursor Class

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

// Joystick Class
chengine::Input::Joystick::Joystick(unsigned int id) : m_JoystickId(id) {
}

chengine::Input::JoyAxisValues chengine::Input::Joystick::getAxes() {
    JoyAxisValues values;
    values.values = glfwGetJoystickAxes(m_JoystickId, &(values.count));
    return values;
}

chengine::Input::JoyButtonValues chengine::Input::Joystick::getButtons() {
    JoyButtonValues values;
    values.buttons = glfwGetJoystickButtons(m_JoystickId, &(values.count));
    return values;
}

chengine::Input::JoyHatValues chengine::Input::Joystick::getHats() {
    JoyHatValues values;
    values.hats = glfwGetJoystickHats(m_JoystickId, &(values.count));
    return values;
}


const char* chengine::Input::Joystick::getName() {
    return glfwGetJoystickName(m_JoystickId);
}

void chengine::Input::Joystick::setConfigurationCallback(GLFWjoystickfun callback) {
    glfwSetJoystickCallback(callback);
}

// Gamepad Class

chengine::Input::Gamepad::Gamepad(unsigned int id) : chengine::Input::Joystick::Joystick(id) {
    if (!glfwJoystickIsGamepad(id)) {
        throw glfw_error(("Joystick " + std::to_string(id) + " is not a valid gamepad").c_str());
    }
}

const char* chengine::Input::Gamepad::getName() {
    return glfwGetGamepadName(m_JoystickId);
}

GLFWgamepadstate chengine::Input::Gamepad::getState() {
    GLFWgamepadstate ret;
    glfwGetGamepadState(m_JoystickId, &ret);
    return ret;
}