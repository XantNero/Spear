#ifndef _ORTOGRAPHICCAMERA_H_
#define _ORTOGRAPHICCAMERA_H_
#include "../../vendor/glm/glm.hpp"

class OrtographicCamera
{
public:

    OrtographicCamera(const float left, const float right, const float bottom, const float top);
    OrtographicCamera(const glm::mat4& ProjectionMatrix);
    ~OrtographicCamera();

    void setPosition(glm::vec3 position);
    const glm::vec3& getPosition() const;
    void setRotation(const float rotation);
    float getRotation() const;

    const glm::mat4& getViewProjectionMatrix() const;
    void setProjectionMatrix(const glm::mat4& ProjectionMatrix);
private:
    void recalculateViewMatrix();
private:
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ViewProjectionMatrix;
    glm::vec3 m_Position;
    float m_Rotation;
};

#endif