#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * Camera class for isometric 3D view.
 * Provides projection and view matrices for rendering.
 */
class Camera {
public:
    Camera();
    
    // Setup isometric camera for Pac-Man view
    void setupIsometric(float distance = 15.0f, float height = 10.0f, float angle = 45.0f);
    
    // Set perspective projection
    void setPerspective(float fov, float aspect, float near_plane, float far_plane);
    
    // Get matrices
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    
    // Camera properties
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    
private:
    glm::mat4 projection;
    float fov_degrees;
    float aspect_ratio;
    float near_clip;
    float far_clip;
};

#endif // CAMERA_H
