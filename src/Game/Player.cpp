#include "Player.h"

const float Player::_YAW_ = -90.0f;
const float Player::_PITCH_ = 0.0f;
const float Player::_SPEED_ = 5.5f;
const float Player::_SPEED_FAST_ = 7.0f;
const float Player::_SENSITIVITY_ = 0.08f;
const double Player::_TIME_LIMIT_ = 300.0;

Player::Player(glm::vec3 starting_position, TerrainElement terrel, glm::mat4 world_transform) : Entity(terrel, world_transform),
                                                                                                position_(starting_position),
                                                                                                world_up_(glm::vec3(0.0f, 1.0f, 0.0f)),
                                                                                                yaw_(_YAW_),
                                                                                                pitch_(_PITCH_),
                                                                                                movement_speed_(_SPEED_),
                                                                                                movement_speed_fast_(_SPEED_FAST_),
                                                                                                mouse_sensitivity_(_SENSITIVITY_),
                                                                                                score_(0),
                                                                                                time_remaining_(300)
{
    updatePlayerVectors();
}

glm::vec3 Player::GetPosition()
{
    return position_;
}

void Player::UpdateBoundingBox()
{
    Entity::SetCenter(position_);
}

void Player::HandleMouse(Camera &camera, float x_offset, float y_offset)
{
    yaw_ = camera.yaw_;
    front_ = camera.front_;

    updatePlayerVectors();
}

void Player::Draw()
{
    Entity::Draw(position_, yaw_);
}

uint32_t Player::GetScore()
{
    return score_;
}

void Player::SetScore(uint32_t score)
{
    score_ = score;
}

void Player::UpdateScore()
{
    score_++;
}

void Player::SetTimeLimit(double seconds)
{
    time_remaining_ = seconds;
}

void Player::updatePlayerVectors()
{
    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_ = glm::normalize(glm::cross(right_, front_));
}

void Player::UpdateTimeRemaining(double delta_time)
{
    time_remaining_ -= delta_time;
    if (time_remaining_ <= 0)
    {
        time_remaining_ = _TIME_LIMIT_;
        score_ = 0;
    }
}

std::string Player::GetTimeRemainingPretty()
{
    uint32_t minutes, seconds;
    minutes = (uint32_t)(time_remaining_ / 60.0);
    seconds = (uint32_t)(time_remaining_ - minutes * 60.0);

    return "TIME: " + std::to_string(minutes) + ":" + ((seconds < 10) ? "0" : "") + std::to_string(seconds);
}

std::string Player::GetScorePretty()
{
    return "SCORE: " + std::to_string(score_);
}
