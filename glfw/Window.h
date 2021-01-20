#ifndef _WINDOW_H_
#define _WINDOW_H_
#include <string>

struct GLFWwindow; 

class Window;

struct WindowData
{
    std::string Title;
    unsigned int Width, Height;
    unsigned int CursorPosX, CursorPosY;
    Window* WindowPointer;
    //TODO: SET PROPER EVENT SYSTEM
    void (*WindowSizeCallback)(Window* window,
                               int width, int height) = nullptr;
    void (*KeyCallback)(Window* window, 
                        int key, int scancode, int action, int mods) = nullptr;
    void (*CharCallback)(Window* window, unsigned int keycode) = nullptr;
    void (*MouseButtonCallback)(Window* window,
                               int button, int action, int mods) = nullptr;
    void (*ScrollCallback)(Window* window,
                           double xOffset, double yOffset) = nullptr;
    void (*CursorPosCallback)(Window* window,
                              double xPos, double yPos) = nullptr;
};
struct WindowProps
{
    std::string Title;
    unsigned int Width, Height;
    WindowProps(const std::string& title = "Model",
                const unsigned int width = 1080,
                const unsigned int height = 720)
                : Title(title), Width(width), Height(height)
    {
    }
};

class Window
{
public:
    Window(const WindowProps& props = WindowProps());
    ~Window();
    void onUpdate();
    inline void 
    setWindowSizeCallback(void (*cb)(Window* window, int width, int height))
        {m_data.WindowSizeCallback = cb; }
	inline void setKeyCallback(void (*cb)(Window* window,
                                          int key, int scancode,
                                          int action, int mods))
        {m_data.KeyCallback = cb; }
	inline void setCharCallback(void (*cb)(Window* window,
                                           unsigned int keycode))
        {m_data.CharCallback = cb; }
	inline void setMouseButtonCallback(void (*cb)(Window* window,
                                                  int button, int action,
                                                   int mods))
        {m_data.MouseButtonCallback = cb; }
	inline void setScrollCallback(void (*cb)(Window* window,
                                             double xOffset, double yOffset))
        {m_data.ScrollCallback = cb; }
	inline void setCursorPosCallback(void (*cb)(Window* window,
                                                double xPos, double yPos))
        {m_data.CursorPosCallback = cb; }
    unsigned int getMouseButton(unsigned int button) const;
    inline unsigned int getWidth() const { return m_data.Width; }
    inline unsigned int getHeight() const { return m_data.Height; }
    bool shouldClose() const;
    void setVSync(bool mode);
private:
    inline void shutDown();
private:
    WindowData m_data;
    GLFWwindow* m_Window;
};

static void WindowSizeCallback(GLFWwindow* window, int width, int height);
static void KeyCallback(GLFWwindow* window,
                        int key, int scancode, int action, int mods);
static void CharCallback(GLFWwindow* window, unsigned int keycode);
static void MouseButtonCallback(GLFWwindow* window,
                                int button, int action, int mods);
static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
static void CursorPosCallback(GLFWwindow* window, double xPos, double yPos);

#endif