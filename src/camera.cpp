#include "camera.h"
#include <cmath>

Camera::Camera()
    : position(0.0f, 10.0f, 15.0f)
    , target(0.0f, 0.0f, 0.0f)
    , up(0.0f, 1.0f, 0.0f)
    , projection(1.0f)
    , fov_degrees(45.0f)
    , aspect_ratio(16.0f / 9.0f)
    , near_clip(0.1f)
    , far_clip(100.0f)
{
    setPerspective(fov_degrees, aspect_ratio, near_clip, far_clip);
}

void Camera::setupIsometric(float distance, float height, float angle) {
    // Convert angle to radians
    float angle_rad = glm::radians(angle);
    
    // Calculate camera position for isometric view
    // Camera orbits around the target at the specified angle
    position.x = distance * std::sin(angle_rad);
    position.y = height;
    position.z = distance * std::cos(angle_rad);
    
    // Look at origin by default
    target = glm::vec3(0.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::setupThirdPerson(const glm::vec3& playerPos, float playerAngle, float distance, float height) {
    // Position camera behind player based on their facing direction
    float angle_rad = glm::radians(playerAngle);
    
    position.x = playerPos.x - std::sin(angle_rad) * distance;
    position.y = playerPos.y + height;
    position.z = playerPos.z - std::cos(angle_rad) * distance;
    
    // Look at player position (slightly ahead)
    target = playerPos + glm::vec3(std::sin(angle_rad) * 2.0f, 0.5f, std::cos(angle_rad) * 2.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::setPerspective(float fov, float aspect, float near_plane, float far_plane) {
    fov_degrees = fov;
    aspect_ratio = aspect;
    near_clip = near_plane;
    far_clip = far_plane;
    projection = glm::perspective(glm::radians(fov), aspect, near_plane, far_plane);
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, target, up);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return projection;
}
