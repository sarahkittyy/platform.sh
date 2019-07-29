#include "Object/Player.hpp"

namespace Object
{

Player::Player(sf::Vector2f startPos)
	: mStartPos(startPos)
{
	// Always should be updated first.
	priority = 0;
}

void Player::init()
{
	// Initialize the player sprite.
	mPlayer.init(resource().texture("assets/sprites/player.png"), { 15, 30 });
	mPlayer.addAnimation("default", { .frames = { 0 }, .speed = sf::seconds(-1) });
	mPlayer.setAnimation("default");
	mPlayer.start();

	reset();
}

void Player::update()
{
}

void Player::updateTick()
{
}

void Player::reset()
{
	// Reset the player's position.
	mPlayer.setPosition(mStartPos);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mPlayer, states);
}

}