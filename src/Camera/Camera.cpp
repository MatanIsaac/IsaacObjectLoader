#include "Camera.h"

namespace isaacObjectLoader
{
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
: m_Front(glm::vec3(0.0f, 0.0f, -1.0f))
, m_MovementSpeed(DEFAULT_SPEED)
, m_MouseSensitivity(DEFAULT_SENSITIVITY)
, m_Zoom(DEFAULT_ZOOM)
{
    m_Position = position;
    m_WorldUp  = up;
    m_Yaw      = yaw;
    m_Pitch    = pitch;
    updateCameraVectors();
}

Camera::Camera(
    float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
: m_Front(glm::vec3(0.0f, 0.0f, -1.0f))
, m_MovementSpeed(DEFAULT_SPEED)
, m_MouseSensitivity(DEFAULT_SENSITIVITY)
, m_Zoom(DEFAULT_ZOOM)
{
    m_Position = glm::vec3(posX, posY, posZ);
    m_WorldUp  = glm::vec3(upX, upY, upZ);
    m_Yaw      = yaw;
    m_Pitch    = pitch;
    updateCameraVectors();
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = m_MovementSpeed * deltaTime;
    if (direction == FORWARD)
        m_Position += m_Front * velocity;
    if (direction == BACKWARD)
        m_Position -= m_Front * velocity;
    if (direction == LEFT)
        m_Position -= m_Right * velocity;
    if (direction == RIGHT)
        m_Position += m_Right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= m_MouseSensitivity;
    yoffset *= m_MouseSensitivity;

    m_Yaw += xoffset;
    m_Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;
        if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}
void Camera::ProcessMouseScroll(float yoffset)
{
    m_Zoom -= (float) yoffset;
    if (m_Zoom < 1.0f)
        m_Zoom = 1.0f;
    if (m_Zoom > 45.0f)
        m_Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    m_Right = glm::normalize(glm::cross(
        m_Front, m_WorldUp));  // normalize the vectors, because their length gets closer to 0 the
                               // more you look up or down which results in slower movement.
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}
}