#include "Player.h"

const float Player::_YAW_ = -90.0f;
const float Player::_PITCH_ = 0.0f;
const float Player::_SPEED_ = 3.5f;
const float Player::_SPEED_FAST_ = 7.0f;

Player::Player(glm::vec3 starting_position, TerrainElement terrel, glm::mat4 world_transform) :
	Entity(terrel, world_transform),
	position_(starting_position),
	world_up_(glm::vec3(0.0f, 1.0f, 0.0f)),
	yaw_(_YAW_),
	pitch_(_PITCH_),
	movement_speed_(_SPEED_),
	movement_speed_fast_(_SPEED_FAST_)
{
	updatePlayerVectors();
}

glm::vec3 Player::GetPosition()
{
    return position_;
}

void Player::Draw()
{
    Entity::Draw(position_);
}

void Player::updatePlayerVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front.y = sin(glm::radians(pitch_));
	front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));

	front_ = glm::normalize(front);
	right_ = glm::normalize(glm::cross(front_, world_up_));
	up_ = glm::normalize(glm::cross(right_, front_));
}
