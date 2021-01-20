#include "ModelApplication.h"
#include "core/MDLParser.h"
#include "pch.h"

ModelApplication* ModelApplication::s_App_Instanse = nullptr;

    
ModelApplication::ModelApplication(const std::string& filePath)
    :m_Window(new Window({"test", 1080, 720})), m_OpenglLayer(new OpenglLayer())/*, m_ImguiLayer(new ImguiLayer())*/ 
{
    m_Window->setCursorPosCallback(cursor_position_callback);
    m_Window->setWindowSizeCallback(window_resize_callback);
    m_Window->setScrollCallback(scroll_callback);
    s_App_Instanse = this;
    m_Window->setVSync(true);
    ParseInformation info = MDLParser::parseFile(filePath);
    
    m_RoadRegistry.reset(info.roadRegistry);
    m_CarGenerators.resize(info.CarGenerators.size());
    m_TrafficLights.resize(info.TrafficLights.size());
    for (unsigned int i = 0; i < info.CarGenerators.size(); ++i) {
        m_CarGenerators[i].reset(info.CarGenerators[i]);
    }
    for (unsigned int i = 0; i < info.TrafficLights.size(); ++i) {
        m_TrafficLights[i].reset(info.TrafficLights[i]);
    }
    m_CarRegistry = std::make_unique<CarRegistry>();
    m_OpenglLayer->onAttach();
    //m_ImguiLayer->onAttach();
}

ModelApplication::~ModelApplication() 
{  
    //m_ImguiLayer->onDetach();
    m_OpenglLayer->onDetach();
 
    delete m_Window;
}

const ModelApplication* ModelApplication::getApp() 
{
    return s_App_Instanse;
}

const Window* ModelApplication::getWindow() const
{
    return m_Window;
}

void ModelApplication::run() 
{
    while(!m_Window->shouldClose()) {
        //CarGenerator.setRate((float)genRate / time);

        float time = glfwGetTime();
        float deltaTime = time - m_LastFrameTime;
        m_LastFrameTime = time;
        // std::cout << deltaTime << '\n';
        for (unsigned int i = 0; i < m_CarGenerators.size(); ++i)
            m_CarGenerators[i]->update(*m_CarRegistry);
        for (unsigned int i = 0; i < m_TrafficLights.size(); ++i)
            m_TrafficLights[i]->update();

        std::vector<const TrafficLight*> lights;
        for (unsigned int i = 0; i < m_TrafficLights.size(); ++i) 
            lights.push_back(m_TrafficLights[i].get());
        m_CarRegistry->update(*m_RoadRegistry, lights, deltaTime);

        ModelInformation info;
        info.cars = m_CarRegistry->getCars();
        info.roads = m_RoadRegistry->getRoads();
        info.lights = lights;
        m_OpenglLayer->onUpdate(info);
        //m_ImguiLayer->begin();
        //m_ImguiLayer->onImguiRender();
        //m_ImguiLayer->end();
        m_Window->onUpdate();
    }
  
}



static float ratio = 1.0f;
void cursor_position_callback(Window* window, double xpos, double ypos)
{
    static double CursorX = xpos;
    static double CursorY = ypos;
    OrtographicCamera* camera =
        ModelApplication::getApp()->getOpenglLayer()->getCamera();
    if (window->getMouseButton(MOUSE_BUTTON_LEFT) == PRESS) {
        //std::cout << xpos << ' ' << ypos << '\n';
        double deltaX =  CursorX - xpos;
        double deltaY =  CursorY - ypos;
        //glfwSetCursorPos(window->m_Window, CursorX, CursorY);
        camera->setPosition(camera->getPosition() + glm::vec3(ratio * deltaX, ratio * deltaY, 0.0f));
    }
    CursorX = xpos;
    CursorY = ypos;
    
}

void window_resize_callback(Window* window, int width, int height)
{
    static int prevWidth = width;
    static int prevHeight = height;
    //camera.setPosition(camera.getPosition() + glm::vec3((float)(width - prevWidth), (float)(height - prevHeight), 0.0f));
    ModelApplication::getApp()->
        getOpenglLayer()->
            getCamera()->
                setProjectionMatrix(glm::ortho(0.0f,
                                              ratio * (float)width,
                                              ratio * (float)height,
                                               0.0f, -1.0f, 1.0f));
    Renderer::viewport(0, 0, width, height);
    prevWidth = width;
    prevHeight = height;
}


void scroll_callback(Window* window, double xOffset, double yOffset)
{

    OrtographicCamera* camera =
        ModelApplication::getApp()->getOpenglLayer()->getCamera();
    float scale = 1.25f;
    if (yOffset == 0)
        return;
    if (yOffset < 0) {
        camera->setPosition(camera->getPosition() + glm::vec3((scale - 1)
                                * (float)window->getWidth()
                                * ratio / (2 * scale),
                            (scale - 1)
                                * (float)window->getHeight()
                                *  ratio / (2 * scale),
                            0.0f));
        ratio /= -1 * scale * yOffset;
    }
    else {
        ratio *= scale * yOffset;
        camera->setPosition(camera->getPosition() - glm::vec3((scale - 1) 
                                * (float)window->getWidth()
                                *  ratio / (2 * scale),
                            (scale - 1)
                                * (float)window->getHeight()
                                *  ratio / (2 * scale), 
                            0.0f));
        
        //camera.setPosition(camera.getPosition() + glm::vec3((scale - 1) *(float)window->getWidth() * ratio / scale, (scale - 1) * (float)window->getHeight() * ratio / scale, 0.0f));
    }
    glm::vec4 p = 
        camera->getViewProjectionMatrix() * glm::vec4(540.0f, 0.0f, 0.0f, 1.0f);
    //std::cout << p.x << ' ' << p.y << ' ';
    
    camera->setProjectionMatrix(glm::ortho(0.0f,
                                            (float)window->getWidth() * ratio,
                                            (float)window->getHeight() * ratio, 
                                            0.0f, -1.0f, 1.0f));
    glm::vec4 z = camera->getViewProjectionMatrix() 
                  * glm::vec4(540.0f, 0.0f, 0.0f, 1.0f);
    //std::cout << z.x << ' ' << z.y << ' ' <<'\n';
}

