#include "OrtographicCamera.h"

#include "../../vendor/glm/gtc/matrix_transform.hpp"

OrtographicCamera::OrtographicCamera(const float left, const float right, const float bottom, const float top)
    :m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(glm::mat4(1.0f)),
    m_ViewProjectionMatrix(glm::mat4(1.0f)), m_Position(glm::vec3(0.0f, 0.0f, 0.0f)), m_Rotation(0.0f)
{
    recalculateViewMatrix();
}
OrtographicCamera::OrtographicCamera(const glm::mat4& ProjectionMatrix)
    :m_ProjectionMatrix(ProjectionMatrix), m_ViewMatrix(glm::mat4(1.0f)),
    m_ViewProjectionMatrix(glm::mat4(1.0f)), m_Position(glm::vec3(0.0f, 0.0f, 0.0f)), m_Rotation(0.0f)
{
}
OrtographicCamera::~OrtographicCamera()
{
}

void OrtographicCamera::setPosition(glm::vec3 position)
{
    m_Position = position;
    recalculateViewMatrix();
}
const glm::vec3& OrtographicCamera::getPosition() const
{
    return m_Position;
}
void OrtographicCamera::setRotation(const float rotation)
{
    m_Rotation = rotation;
    recalculateViewMatrix();
}
float OrtographicCamera::getRotation() const
{
    return m_Rotation;
}

const glm::mat4& OrtographicCamera::getViewProjectionMatrix() const
{
    return m_ViewProjectionMatrix;
}

void OrtographicCamera::setProjectionMatrix(const glm::mat4& ProjectionMatrix) 
{
    m_ProjectionMatrix = ProjectionMatrix;
    recalculateViewMatrix();
}

void OrtographicCamera::recalculateViewMatrix()
{
    glm::mat4 transform = glm::translate(glm::mat4(1.0f),  m_Position)
     * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    
    m_ViewMatrix = glm::inverse(transform);
    //m_ViewMatrix = transform;
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}