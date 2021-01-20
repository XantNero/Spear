#ifndef _MODELAPPLICATION_H_
#define _MODELAPPLICATION_H_
#include "OpenglLayer.h"
#include "glfw/Window.h"
//#include "imgui/ImguiLayer.h"

#include "core/Car.h"
#include "core/Road.h"
#include "core/TrafficLight.h"

#include "glfw/KeyKodes.h"

#include <vector>
#include <memory>
#include <string>

//TODO THINK: car* --> make carregistry unique_ptr

class OpenglLayer;

struct ModelInformation
{
    std::vector<Car> cars;
    std::vector<const Road*> roads;
    std::vector<const TrafficLight*> lights;
};

class ModelApplication
{
public:
    ModelApplication(const std::string& filePath);
    ~ModelApplication();
    static const ModelApplication* getApp();
    const Window* getWindow() const;
    inline const OpenglLayer* getOpenglLayer() const { return m_OpenglLayer; }
    void run();
private:
    //ImguiLayer* m_ImguiLayer;
    OpenglLayer* m_OpenglLayer;
    Window* m_Window;
    static ModelApplication *s_App_Instanse;

    std::unique_ptr<RoadRegistry> m_RoadRegistry;
    std::unique_ptr<CarRegistry> m_CarRegistry;
    std::vector<std::unique_ptr<CarGenerator>> m_CarGenerators;
    std::vector<std::unique_ptr<TrafficLight>> m_TrafficLights;

    float m_LastFrameTime = 0.0f;
};

void cursor_position_callback(Window* window, double xpos, double ypos);
void scroll_callback(Window* window, double xOffset, double yOffset);
void window_resize_callback(Window* window, int width, int height);

#endif