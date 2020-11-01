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

        void createWindow(); // implemented

        static std::unordered_map<GLFWwindow*,Window*> s_ReverseLookupMap;

    public:
        static void applySettings(WindowSettings settings); // implemented

        static void init(); // implemented

        static Window* reverseLookup(GLFWwindow* window); // implemented

        Window(uint16_t width, uint16_t height, std::string title, WindowSettings settings); // implemented
        Window(uint16_t width, uint16_t height, std::string title, WindowSettings settings, GLFWmonitor* monitor); // implemented
        Window(uint16_t width, uint16_t height, std::string title, WindowSettings settings, Window* share); // implemented
        Window(uint16_t width, uint16_t height, std::string title, WindowSettings settings, GLFWmonitor* monitor, Window* share); // implemented

        ~Window(); // implemented


        void pollEvents(); // implemented
        void waitEvents(); // implemented
        void waitEventsTimeout(double timeout); // implemented
        void postEmptyEvent(); // implemented

        void setKeyCallback(GLFWkeyfun function); // implemented
        Input::State getKeyState(int key); // implemented

        void setCharCallback(GLFWcharfun callback); // implemented

        void setCursorPosCallback(GLFWcursorposfun callback); // implemented
        void setCursorEnterCallback(GLFWcursorenterfun callback); // implemented
        void getCursorPos(double *x, double *y); // implemented
        int getWindowAttrib(Input::WindowAttrib attrib); // implemented

        void setMouseButtonCallback(GLFWmousebuttonfun callback); // implemented 
        Input::State getMouseButton(Input::MouseButton button);




        void setInputMode(Input::Mode mode, int value); // implemented
        
        void enableStickyKeys(); // implemented
        void disableStickyKeys(); // implemented

        void enableStickyMouseButtons(); // implemented
        void disableStickyMouseButtons(); // implemented

        void readLockKeyMods(); // implemented
        void ignoreLockKeyMods(); // implemented

        void setCursorMode(Input::CursorMode mode); // implemented

        void enableRawMouseMotion(); // implemented
        void disableRawMouseMotion(); // implemented

        void swapBuffers(); // implemented
        void setSwapInterval(int swap_interval); // implemented

        void makeWindowedFullscreen(); // implemented
        void makeWindowlessFullscreen(); // implemented

        void setCursor(Cursor* cursor); // implemented

        bool isRawMouseMotionSupported();

    };

    namespace Input {
        
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

        
        const int getScancode(int key);
        const char* getKeyName(int key, int scancode);
    }


    class Cursor {
    private:
        GLFWcursor* m_Cursor;

        friend void Window::setCursor(Cursor* cursor);
    public:
        Cursor(unsigned char* pixels, int width, int height);
        Cursor(int standard_type);

        ~Cursor();
    };
}