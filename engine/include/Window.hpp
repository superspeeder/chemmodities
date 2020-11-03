#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <string>
#include <cinttypes>
#include <unordered_map>
#include <glm/vec2.hpp>

namespace chengine {
    class Cursor {
        friend class Window;
        private:
            GLFWcursor* m_Cursor;
    
        public:
            Cursor(unsigned char* pixels, int width, int height, int x_hot, int y_hot);
            Cursor(int standard_type);
    
            ~Cursor();
    };
    namespace Input {
        // Input Enums
        enum State {
            Press = GLFW_PRESS,
            Release = GLFW_RELEASE,
            Repeat = GLFW_REPEAT
        };

        enum Mode {
            Cursor = GLFW_CURSOR,
            StickyKeys = GLFW_STICKY_KEYS,
            StickyMouseButtons = GLFW_STICKY_MOUSE_BUTTONS,
            LockKeyMods = GLFW_LOCK_KEY_MODS,
            RawMouseMotion = GLFW_RAW_MOUSE_MOTION
        };

        enum CursorMode {
            Normal = GLFW_CURSOR_NORMAL,
            Hidden = GLFW_CURSOR_HIDDEN,
            Disabled = GLFW_CURSOR_DISABLED
        };

        enum WindowAttrib {
            Focused = GLFW_FOCUSED,
            Iconified = GLFW_ICONIFIED,
            Maximized = GLFW_MAXIMIZED,
            Hovered = GLFW_HOVERED,
            Visible = GLFW_VISIBLE,
            Resizable = GLFW_RESIZABLE,
            Decorated = GLFW_DECORATED,
            AutoIconify = GLFW_AUTO_ICONIFY,
            Floating = GLFW_FLOATING,
            TransparentFramebuffer = GLFW_TRANSPARENT_FRAMEBUFFER,
            FocusOnShow = GLFW_FOCUS_ON_SHOW
        };

        enum MouseButton {
            Button1 = GLFW_MOUSE_BUTTON_1,
            Button2 = GLFW_MOUSE_BUTTON_2,
            Button3 = GLFW_MOUSE_BUTTON_3,
            Button4 = GLFW_MOUSE_BUTTON_4,
            Button5 = GLFW_MOUSE_BUTTON_5,
            Button6 = GLFW_MOUSE_BUTTON_6,
            Button7 = GLFW_MOUSE_BUTTON_7,
            Button8 = GLFW_MOUSE_BUTTON_8,
        };

        enum HatState {
            HatCentered = GLFW_HAT_CENTERED,
            HatUp = GLFW_HAT_UP,
            HatRight = GLFW_HAT_RIGHT,
            HatDown = GLFW_HAT_DOWN,
            HatLeft = GLFW_HAT_LEFT,
            HatRightUp = GLFW_HAT_RIGHT_UP,
            HatRightDown = GLFW_HAT_RIGHT_DOWN,
            HatLeftUp = GLFW_HAT_LEFT_UP,
            HatLeftDown = GLFW_HAT_LEFT_DOWN
        };

        // Joystick Values Structs
        struct JoyAxisValues {
            int count;
            const float* values;

            const float operator[](int axis) {
                if (axis < count) {
                    return values[axis];
                }
                return 0;
            };
        };

        struct JoyButtonValues {
            int count;
            const unsigned char* buttons;
            
            const State operator[](int b_id) {
                if (b_id < count) {
                    return static_cast<State>(buttons[b_id]);
                }
                return State::Release;
            };
        };

        struct JoyHatValues {
            int count;
            const unsigned char* hats;

            const HatState operator[](int hat_id) {
                if (hat_id < count) {
                    return static_cast<HatState>(hats[hat_id]);
                }
                return HatState::HatCentered;
            };
        };

        // Basic windowless input functions
        const int getScancode(int key);
        const char* getKeyName(int key, int scancode);

        double getTime();
        void setTime(double newTime);
        
        uint64_t getTimerValue();
        uint64_t getTimerFrequency();

        const char* getSystemClipboardString(); // proper paste
        void setSystemClipboardString(const char* string); // proper copy/cut


        class Joystick {
        protected:
            unsigned int m_JoystickId;
        public:
            static bool isJoystickPresent(unsigned int id);

            Joystick(unsigned int id);

            JoyAxisValues getAxes();
            JoyButtonValues getButtons();
            JoyHatValues getHats();
            
            const char* getName();

            static void setConfigurationCallback(GLFWjoystickfun callback);
        };

        class Gamepad : public Joystick {
        public:
            Gamepad(unsigned int id);
            const char* getName();

            GLFWgamepadstate getState();
        };
    }

    enum FullScreenMode {
        WINDOWED, WINDOWLESS
    };

    struct WindowSettings {
        bool
            resizable = true, visible = true, decorated = true, focused = true,
            auto_iconify = true, floating = false, maximized = false, center_cursor = true,
            transparent_framebuffer = false, focus_on_show = true, scale_to_monitor = false, 
            stereo = false, srgb_capable = false, doublebuffer = true, 
            opengl_forward_compat = false, opengl_debug_context = false;

        int 
            red_bits = 8, green_bits = 8, blue_bits = 8, alpha_bits = 8, depth_bits = 24, 
            stencil_bits = 8, accum_red_bits = 0, accum_green_bits = 0, accum_blue_bits = 0, 
            accum_alpha_bits = 0, aux_buffers = 0, samples = 0, refresh_rate = GLFW_DONT_CARE, 
            client_api = GLFW_OPENGL_API, context_creation_api = GLFW_NATIVE_CONTEXT_API, 
            context_version_major = 1, context_version_minor = 0, 
            context_robustness = GLFW_NO_ROBUSTNESS, context_release_behavior = GLFW_ANY_RELEASE_BEHAVIOR,
            opengl_profile = GLFW_OPENGL_ANY_PROFILE;

        FullScreenMode fullScreenMode = WINDOWED;
        bool make_fullscreen = false;
    };

    class Window {
    private:
        GLFWwindow* m_Window;
        uint16_t m_Width, m_Height;
        std::string m_Title;
        WindowSettings m_WindowSettings;
        GLFWmonitor* m_Monitor;
        Window* m_Share;

        // Window setup
        void createWindow();
        void applySettings(WindowSettings settings);

        static std::unordered_map<GLFWwindow*,Window*> s_ReverseLookupMap;

    public:
        // Static Functions
        static void init();
        static Window* reverseLookup(GLFWwindow* window);

        // Constructors
        Window(uint16_t width, uint16_t height, std::string title, WindowSettings settings);
        Window(uint16_t width, uint16_t height, std::string title, WindowSettings settings, GLFWmonitor* monitor);
        Window(uint16_t width, uint16_t height, std::string title, WindowSettings settings, Window* share);
        Window(uint16_t width, uint16_t height, std::string title, WindowSettings settings, GLFWmonitor* monitor, Window* share);

        // Special Functions and Operators
        ~Window();

        // Window Setup
        void makeWindowedFullscreen();
        void makeWindowlessFullscreen();
        void initGLAD();

        // Basic Drawing
        void swapBuffers();
        void setSwapInterval(int swap_interval);

        // Basic Events
        void pollEvents();
        void waitEvents();
        void waitEventsTimeout(double timeout);
        void postEmptyEvent();

        // Basic Input
        void setInputMode(chengine::Input::Mode mode, int value);

        // Key Input
        void setKeyCallback(GLFWkeyfun function);
        Input::State getKeyState(int key);

        void setCharCallback(GLFWcharfun callback);

        void enableStickyKeys();
        void disableStickyKeys();

        void readLockKeyMods();
        void ignoreLockKeyMods();

        // Mouse Input
        void setMouseButtonCallback(GLFWmousebuttonfun callback); 
        Input::State getMouseButton(Input::MouseButton button);
        void enableStickyMouseButtons();
        void disableStickyMouseButtons();

        void setCursorEnterCallback(GLFWcursorenterfun callback);

        void setCursorPosCallback(GLFWcursorposfun callback);
        void getCursorPos(double *x, double *y);
        glm::vec2 getCursorPos();

        void setScrollCallback(GLFWscrollfun callback);

        void setCursorMode(Input::CursorMode mode);
        void setCursor(Cursor* cursor);

        void enableRawMouseMotion();
        void disableRawMouseMotion();
        bool isRawMouseMotionSupported();

        // Path Drop Event
        void setPathDropCallback(GLFWdropfun callback);

        // Clipboard
        const char* getClipboardString();
        void setClipboardString(const char* string);
        

        // Basic Window Info
        int getWindowAttrib(Input::WindowAttrib attrib);
        void setWindowAttrib(Input::WindowAttrib attrib, int value);
        
        bool shouldWindowClose();
        void closeWindow();
        void setWindowClose(bool value);
        void setWindowCloseCallback(GLFWwindowclosefun callback);

        void resizeWindow(int x, int y);
        void setWindowResizeCallback(GLFWwindowsizefun callback);
        glm::vec2 getWindowSize();
        void getWindowSize(int *x, int *y);

        void getWindowFrameSize(int *left, int *top, int *right, int *bottom);

        void setFramebufferSizeCallback(GLFWframebuffersizefun callback);
        glm::vec2 getFramebufferSize();
        void getFramebufferSize(int *x, int *y);

        void getContentScale(float *x, float *y);
        glm::vec2 getContentScale();
        void setContentScaleCallback(GLFWwindowcontentscalefun callback);

        void setWindowSizeLimits(int x_min, int y_min, int x_max, int y_max);

        void setAspectRatio(int x, int y);

        void setWindowPos(int x, int y);
        void getWindowPos(int *x, int *y);
        glm::vec2 getWindowPos();
        void setWindowPosCallback(GLFWwindowposfun callback);
         
        void setWindowTitle(const char* title);

        void setIcon(GLFWimage* icons, int count);

        GLFWmonitor* getMonitor();
        const GLFWvidmode* getVidMode();

        void setWindowMonitor(GLFWmonitor* monitor, int xpos, int ypos, int width, int height, int refreshrate);
        
        void restoreWindow();
        
        void iconify();
        void setIconifyCallback(GLFWwindowiconifyfun callback);

        void maximize();
        void setMaximizeCallback(GLFWwindowmaximizefun callback);

        void hide();
        void show();
        bool isVisible();

        void focus();
        void setFocusCallback(GLFWwindowfocusfun callback);
        bool isFocused();

        void requestAttention();

        void setRefreshCallback(GLFWwindowrefreshfun callback);

        void setOpacity(float opacity);
        float getOpacity();

        // User Pointer
        void setUserPointer(void* ptr);
        void* getUserPointer();

        // Context

        void makeContextCurrent();
        static Window* getCurrentContext();
    };
}