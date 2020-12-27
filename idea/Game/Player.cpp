#include "Player.h"

const float Player::_YAW_ = -90.0f;
const float Player::_PITCH_ = 0.0f;
const float Player::_SPEED_ = 3.5f;
const float Player::_SPEED_FAST_ = 7.0f;
const float Player::_SENSITIVITY_ = 0.08f;


Player::Player(glm::vec3 starting_position, TerrainElement terrel, glm::mat4 world_transform) :
	Entity(terrel, world_transform),
	position_(starting_position),
	world_up_(glm::vec3(0.0f, 1.0f, 0.0f)),
	yaw_(_YAW_),
	pitch_(_PITCH_),
	movement_speed_(_SPEED_),
	movement_speed_fast_(_SPEED_FAST_),
	mouse_sensitivity_(_SENSITIVITY_)
{
	updatePlayerVectors();
}

glm::vec3 Player::GetPosition()
{
    return position_;
}

void Player::HandleMouse(Camera& camera, float x_offset, float y_offset)
{
	yaw_ = camera.yaw_;
	front_ = camera.front_;
	
	updatePlayerVectors();
}

void Player::Draw()
{
    Entity::Draw(position_, yaw_);
}

void Player::updatePlayerVectors()
{
	right_ = glm::normalize(glm::cross(front_, world_up_));
	up_ = glm::normalize(glm::cross(right_, front_));
}
