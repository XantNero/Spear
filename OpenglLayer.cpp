#include "GL/glew.h"
#include "OpenglLayer.h"
#include "ModelApplication.h"
#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "glfw/Window.h"
#include "opengl/Texture.h"


static const int MaxCars = 1000;
static const int MaxVertexBufferSize = MaxCars * 16 * sizeof(float);
static const int MaxIndexBufferCount = MaxCars * 6;

OpenglLayer::OpenglLayer() 
{  
}

OpenglLayer::~OpenglLayer() 
{
}

void OpenglLayer::onAttach() 
{
    setCamera();
    setCarVAO();
    setRoadVAO();
    setLightVAO();
    setShader();
    Renderer::enableBlending();
}

void OpenglLayer::onUpdate(const ModelInformation& information) 
{
    Renderer::setClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    Renderer::clear();

    drawRoads(information.roads);
    drawCars(information.cars);
    drawTrafficlights(information.lights);
}

void OpenglLayer::drawRoads(const std::vector<const Road*> &roads) 
{
    m_RoadVAO->bind();
    std::shared_ptr<VertexBuffer> RoadBuffer =
        m_RoadVAO->getVertexBufferArray()[0];
    RoadBuffer->resetData();
    std::shared_ptr<IndexBuffer> RoadIBO = m_RoadVAO->getIndexBuffer();
    RoadIBO->ResetData();
    float roadsVertices[MaxVertexBufferSize];
    unsigned int roadsIndices[MaxIndexBufferCount];
    unsigned int roadsVertivesIndex = 0;
    unsigned int roadsIndicesIndex = 0;
    glm::vec3 roadColor(1.0f, 1.0f, 1.0f);
    for (unsigned int i = 0; i < roads.size(); ++i) {
        const Road* road = roads[i];
        for (unsigned int j = 0; j < road->getRoadSize(); ++j) {
            Vector position = road->getPoint(j);
            roadsVertices[5 * roadsVertivesIndex] = (float)position.getX();
            roadsVertices[5 * roadsVertivesIndex + 1] = (float)position.getY();
            roadsVertices[5 * roadsVertivesIndex + 2] = roadColor.r;
            roadsVertices[5 * roadsVertivesIndex + 3] = roadColor.g;
            roadsVertices[5 * roadsVertivesIndex + 4] = roadColor.b;
         
            roadsIndices[roadsIndicesIndex] = roadsVertivesIndex;
            ++roadsIndicesIndex;
            if (j != 0 && j != road->getRoadSize() - 1) {
                roadsIndices[roadsIndicesIndex] = roadsVertivesIndex;
                ++roadsIndicesIndex;
            }
            ++roadsVertivesIndex;
        }
    }
    RoadBuffer->bind();
    RoadBuffer->addData(5 * roadsVertivesIndex* sizeof(float), roadsVertices);
    RoadIBO->bind();
    RoadIBO->addData(roadsIndicesIndex, roadsIndices);
    // RoadBuffer->bind();
    // RoadBuffer->addData(2 * 5 * sizeof(float), roadVertices);
    // RoadIBO->bind();
    // RoadIBO->addData(2 * 4, roadIndices);

    m_Shader->bind();
    m_Shader->setUnifromMat4f("u_ViewProjectionMatrix",
                               m_Camera->getViewProjectionMatrix());
    Renderer::draw(GL_LINES, *m_RoadVAO, *m_Shader);
     //m_RoadShader->unbind();
}

void OpenglLayer::drawCars(const std::vector<Car> &cars) 
{
    m_CarVAO->bind();
    std::shared_ptr<VertexBuffer> CarBuffer =
        m_CarVAO->getVertexBufferArray()[0];
    CarBuffer->bind();
    CarBuffer->resetData();

    std::shared_ptr<IndexBuffer> CarIBO = m_CarVAO->getIndexBuffer();
    CarIBO->bind();
    CarIBO->ResetData();

    glm::vec3 carColors[] = {
        {0.2f, 0.7f, 0.4f},
        {0.6f, 0.2f, 0.8f},
        {0.1f, 0.2f, 0.8f},
        {0.8f, 0.1f, 0.1f}
    };
    glm::vec3 carColor;
    for (unsigned int i = 0; i < cars.size(); ++i) {
        Car car = cars[i];
        carColor = carColors[(int)car.getMaxPossibleSpeed() % 4];
        Vector pos = car.getPosition();

        Vector vel = car.getVelocity();
        Vector normal(0.0f, -1.0f);
        float angle = vel.getAngle(normal);
        if (scalarDotProduct(vel, normal) > 0)
            angle *= -1;
        glm::mat4 ModelMatrix =
            glm::translate(glm::mat4(1.0f),
                           glm::vec3(pos.getX(), pos.getY(), 0.0f))
            * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f))
            * glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 0.8f, 0.0f));
        
            float carTrig[] = {
            -10.0f, -20.0f, carColor.r, carColor.g, carColor.b,
            -10.0f, 20.0f,  carColor.r, carColor.g, carColor.b,
             10.0f, 20.0f,  carColor.r, carColor.g, carColor.b,
             10.0f, -20.0f, carColor.r, carColor.g, carColor.b,
        };

        for (int j = 0; j < 4; ++j) {
            glm::vec4 position = 
                ModelMatrix 
                * glm::vec4(carTrig[5 * j], carTrig[5 * j + 1], 0.0f, 1.0f);
            carTrig[5 * j] = position.x;
            carTrig[5 * j + 1] = position.y; 
        }


        unsigned int carIn[] = {
            0 + i * 4, 1 + i * 4, 2 + i * 4,
            2 + i * 4, 3 + i * 4, 0 + i * 4
        };

        CarBuffer->addData(4 * 5 * sizeof(float), carTrig);
        CarIBO->addData(6, carIn);        
    }
    m_Shader->bind();
    // m_Shader->setUnifromMat4f("u_ViewProjectionMatrix", m_Camera->getViewProjectionMatrix());
    //m_CarShader->setUniform1i("u_Texture", 0);
    Renderer::draw(GL_TRIANGLES, *m_CarVAO, *m_Shader);
    //m_CarShader->unbind();
}

void OpenglLayer::drawTrafficlights(const std::vector<const TrafficLight*> &lights) 
{
    m_LightsVAO->bind();
    std::shared_ptr<VertexBuffer> lightBuffer =
        m_LightsVAO->getVertexBufferArray()[0];
    lightBuffer->bind();
    lightBuffer->resetData();

    std::shared_ptr<IndexBuffer> lightIBO = m_LightsVAO->getIndexBuffer();
    lightBuffer->bind();
    lightIBO->ResetData();

    for (unsigned int i = 0; i < lights.size(); ++i) {

        Vector pos = lights[i]->getPosition();

        glm::mat4 ModelMatrix =
            glm::translate(glm::mat4(1.0f), glm::vec3(pos.getX(), pos.getY(), 0.0f))
            * glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 0.0f));
        glm::vec3 colors[] = {
            {0.0f, 1.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 0.0f}
        };
        glm::vec3 lightColor;
        if (lights[i]->getState() == TrafficLight::State::Green)
            lightColor = colors[0];
        else if (lights[i]->getState() == TrafficLight::State::Red)
            lightColor = colors[1];
        else if (lights[i]->getState() == TrafficLight::State::Yellow)
            lightColor = colors[2];
        
        float circlePoints[] = {
            0.0f, 0.0f,                lightColor.r, lightColor.g, lightColor.b,
            1.0f, 0.0f,                lightColor.r, lightColor.g, lightColor.b,
            sqrt(3.0f) / 2.0f, 0.5f,   lightColor.r, lightColor.g, lightColor.b,
            0.5f, sqrt(3.0f) / 2.0f,   lightColor.r, lightColor.g, lightColor.b,
            0.0f, 1.0f,                lightColor.r, lightColor.g, lightColor.b,
            -0.5f, sqrt(3.0f) / 2.0f,  lightColor.r, lightColor.g, lightColor.b,
            -sqrt(3.0f) / 2.0f, 0.5f,  lightColor.r, lightColor.g, lightColor.b,
            -1.0f, 0.0f,               lightColor.r, lightColor.g, lightColor.b,
            -sqrt(3.0f) / 2.0f, -0.5f, lightColor.r, lightColor.g, lightColor.b,
            -0.5f, -sqrt(3.0f) / 2.0f, lightColor.r, lightColor.g, lightColor.b,
            0.0f, -1.0f,               lightColor.r, lightColor.g, lightColor.b,
            0.5f, -sqrt(3.0f) / 2.0f,  lightColor.r, lightColor.g, lightColor.b,
            sqrt(3.0f) / 2.0f, -0.5f,  lightColor.r, lightColor.g, lightColor.b
        };

        for (int j = 0; j < sizeof(circlePoints) / (5 * sizeof(float)); ++j) {
            glm::vec4 position =
                ModelMatrix
                * glm::vec4(circlePoints[5 * j],
                            circlePoints[5 * j + 1], 0.0f, 1.0f);
            circlePoints[5 * j] = position.x;
            circlePoints[5 * j + 1] = position.y; 
        }


        unsigned int circleIndex[] = {
            0 + i * 13, 1 + i * 13, 2 + i * 13,
            0 + i * 13, 2 + i * 13, 3 + i * 13,
            0 + i * 13, 3 + i * 13, 4 + i * 13,
            0 + i * 13, 4 + i * 13, 5 + i * 13,
            0 + i * 13, 5 + i * 13, 6 + i * 13,
            0 + i * 13, 6 + i * 13, 7 + i * 13,
            0 + i * 13, 7 + i * 13, 8 + i * 13,
            0 + i * 13, 8 + i * 13, 9 + i * 13,
            0 + i * 13, 9 + i * 13, 10 + i * 13,
            0 + i * 13, 10 + i * 13, 11 + i * 13,
            0 + i * 13, 11 + i * 13, 12 + i * 13,
            0 + i * 13, 12 + i * 13, 1 + i * 13
        };

        lightBuffer->addData(sizeof(circlePoints), circlePoints);
        lightIBO->addData(sizeof(circleIndex) / sizeof(unsigned int),
                          circleIndex);                     
    }
    
    m_Shader->bind();
    // m_RoadShader->setUniform4f("u_Color", color.r, color.g, color.b, 1.0f);
    // m_RoadShader->setUnifromMat4f("u_ViewProjectionMatrix", m_Camera->getViewProjectionMatrix());
    Renderer::draw(GL_TRIANGLES, *m_LightsVAO, *m_Shader);
}

void OpenglLayer::setCamera() 
{
    const Window* window = ModelApplication::getApp()->getWindow();
    m_Camera = 
        std::make_unique<OrtographicCamera>(0.0f,
                                            (float)window->getWidth(),
                                            (float)window->getHeight(),
                                            0.0f);
    m_Camera->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
}

void OpenglLayer::setRoadVAO() 
{
    m_RoadVAO = std::make_unique<VertexArray>();
    m_RoadVAO->bind();
    std::shared_ptr<VertexBuffer> roadBuffer =
        std::make_shared<VertexBuffer>(MaxVertexBufferSize, nullptr);
    VertexBufferLayout layout;
    layout.push(GL_FLOAT, 2, 2 * sizeof(float));
    layout.push(GL_FLOAT, 3, 3 * sizeof(float));
    m_RoadVAO->addVertexBuffer(roadBuffer, layout);
    std::shared_ptr<IndexBuffer> roadIBO = 
        std::make_shared<IndexBuffer>(MaxIndexBufferCount, nullptr);
    m_RoadVAO->setIndexBuffer(roadIBO);
    m_RoadVAO->unbind();
}

void OpenglLayer::setShader() 
{
    m_Shader = std::make_unique<Shader>("res/shaders/main.shader");
}

void OpenglLayer::setCarVAO() 
{
    m_CarVAO = std::make_unique<VertexArray>();
    m_CarVAO->bind();

    std::shared_ptr<VertexBuffer> carBuffer = 
        std::make_shared<VertexBuffer>(MaxVertexBufferSize, nullptr);
    VertexBufferLayout layout;
    layout.push(GL_FLOAT, 2, 2 * sizeof(float));
    layout.push(GL_FLOAT, 3, 3 * sizeof(float));
    m_CarVAO->addVertexBuffer(carBuffer, layout);
    
    std::shared_ptr<IndexBuffer> carIBO = 
        std::make_shared<IndexBuffer>(MaxIndexBufferCount, nullptr);
    m_CarVAO->setIndexBuffer(carIBO);


    m_CarVAO->unbind();
}

void OpenglLayer::setLightVAO() 
{
    
    m_LightsVAO = std::make_unique<VertexArray>();
    m_LightsVAO->bind();
    std::shared_ptr<VertexBuffer> lightBuffer = 
        std::make_shared<VertexBuffer>(MaxVertexBufferSize, nullptr);
    VertexBufferLayout layout;
    layout.push(GL_FLOAT, 2, 2 * sizeof(float));
    layout.push(GL_FLOAT, 3, 3 * sizeof(float));

    m_LightsVAO->addVertexBuffer(lightBuffer, layout);
    std::shared_ptr<IndexBuffer> lightIBO = 
        std::make_shared<IndexBuffer>(MaxIndexBufferCount, nullptr);
    m_LightsVAO->setIndexBuffer(lightIBO);
    m_LightsVAO->unbind();

}

void OpenglLayer::setTexture() 
{
    // m_Texture = std::make_unique<Texture>("res/textures/c.png");
    // m_Texture->bind(0);
    // m_CarShader = std::make_unique<Shader>("res/shaders/car.shader");
    // m_CarShader->bind();

    //m_CarShader->setUniform1i("u_Texture", 0);
    //m_CarShader->unbind();
}
