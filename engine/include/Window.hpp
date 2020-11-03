#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <string>
#include <cinttypes>
#include <unordered_map>

namespace chengine {
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

        // Basic Drawing
        void swapBuffers();
        void setSwapInterval(int swap_interval);

        // Basic Events
        void pollEvents();
        void waitEvents();
        void waitEventsTimeout(double timeout);
        void postEmptyEvent();

        // Basic Input
        void setInputMode(Input::Mode mode, int value);

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

        void setScrollCallback(GLFWscrollfun callback);

        void setCursorMode(Input::CursorMode mode);
        void setCursor(Cursor* cursor);

        void enableRawMouseMotion();
        void disableRawMouseMotion();
        bool isRawMouseMotionSupported();

        // Basic Window Info
        int getWindowAttrib(Input::WindowAttrib attrib);

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
            Left = GLFW_MOUSE_BUTTON_LEFT,
            Right = GLFW_MOUSE_BUTTON_RIGHT,
            Button3 = GLFW_MOUSE_BUTTON_3,
            Button4 = GLFW_MOUSE_BUTTON_4,
            Button5 = GLFW_MOUSE_BUTTON_5,
            Button6 = GLFW_MOUSE_BUTTON_6,
            Button7 = GLFW_MOUSE_BUTTON_7,
            Button8 = GLFW_MOUSE_BUTTON_8,
        };

        enum HatState {
            Centered = GLFW_HAT_CENTERED,
            Up = GLFW_HAT_UP,
            Right = GLFW_HAT_RIGHT,
            Down = GLFW_HAT_DOWN,
            Left = GLFW_HAT_LEFT,
            RightUp = GLFW_HAT_RIGHT_UP,
            RightDown = GLFW_HAT_RIGHT_DOWN,
            LeftUp = GLFW_HAT_LEFT_UP,
            LeftDown = GLFW_HAT_LEFT_DOWN
        };

        // Joystick Values Structs
        struct JoyAxisValues {
            int count;
            const float* values;

            const float operator[](unsigned int axis) {
                if (axis < count) {
                    return values[axis];
                }
                return 0;
            };
        };

        struct JoyButtonValues {
            int count;
            const unsigned char* buttons;
            
            const State operator[](unsigned int b_id) {
                if (b_id < count) {
                    return static_cast<State>(buttons[b_id]);
                }
                return State::Release;
            };
        };

        struct JoyHatValues {
            int count;
            const unsigned char* hats;

            const HatState operator[](unsigned int hat_id) {
                if (hat_id < count) {
                    return static_cast<HatState>(hats[hat_id]);
                }
                return HatState::Centered;
            };
        };

        // Basic windowless input functions
        const int getScancode(int key);
        const char* getKeyName(int key, int scancode);

        class Joystick {
        private:
            unsigned int m_JoystickId;
        public:
            static bool isJoystickPresent(unsigned int id);

            Joystick(unsigned int id);

            JoyAxisValues getAxes();
            JoyButtonValues getButtons();
            JoyHatValues getHats();
            
            const char* getName();
        };
    }


    // Cursor Class
    class Cursor {
    private:
        GLFWcursor* m_Cursor;

        friend void Window::setCursor(Cursor* cursor);
    public:
        Cursor(unsigned char* pixels, int width, int height, int x_hot, int y_hot);
        Cursor(int standard_type);

        ~Cursor();
    };
}