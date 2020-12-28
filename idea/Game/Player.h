#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Types/EMovement.h"
#include "Renderer/Camera.h"
#include "Game/Entity.h"

class Player : public Entity
{
public:
    glm::vec3 position_;
    glm::vec3 world_up_;
    glm::vec3 front_;
    glm::vec3 right_;
    glm::vec3 up_;
    float mouse_sensitivity_;
    float movement_speed_;
    float movement_speed_fast_;
    float yaw_;
    float pitch_;
    
    Player(glm::vec3 starting_position = glm::vec3(0.0f, 0.0f, 0.0f),
        TerrainElement terrel = TerrainElement(
                Model("Resources/Models/player/player.obj", true),
                Shader("Resources/Shaders/Model/lowPolyPlayer.vert", "Resources/Shaders/Model/lowPolyPlayer.frag")),
        glm::mat4 world_transform = glm::mat4(1.0f));

    glm::vec3 GetPosition();
    void UpdateBoundingBox();
    void HandleMouse(Camera& camera, float x_offset, float y_offset);
    void Draw();

    uint32_t GetScore();
    void SetScore(uint32_t score);
    void UpdateScore();
    void SetTimeLimit(double seconds);
    void UpdateTimeRemaining(double delta_time);
    std::string GetTimeRemainingPretty();
    std::string GetScorePretty();

private:
    uint32_t score_;
    double time_remaining_;

    static const float _YAW_;
    static const float _PITCH_;
    static const float _SPEED_;
    static const float _SPEED_FAST_;
    static const float _SENSITIVITY_;
    static const double _TIME_LIMIT_;

    void updatePlayerVectors();
};
