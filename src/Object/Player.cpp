#include "Object/Player.hpp"

namespace Object
{

Player::Player(sf::Vector2i startPos)
	: mStartPos(startPos)
{
}

void Player::init()
{
	// The player should always should be updated first.
	setPriority(0);
	setZIndex(0);

	// Initialize the player sprite.
	mPlayer.init(resource().texture("assets/sprites/player.png"), { 15, 30 });
	mPlayer.addAnimation("default", { .frames = { 0 }, .speed = sf::seconds(-1) });
	mPlayer.setAnimation("default");
	mPlayer.start();

	reset();
}

void Player::update()
{
	mPlayer.update();

	// Keep the level camera centered on the player.
	setCameraPosition(mPlayer.getPosition() +
					  sf::Vector2f(mPlayer.getSize().x / 2.f,
								   mPlayer.getSize().y / 2.f));
}

void Player::updateTick()
{
	// Jumping
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		if (!mJustJumped)
		{
			if (jump())
				mJustJumped = true;
		}
		else
		{
			mJustJumped = false;
		}
	}
	// L/R movement.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		moveRight();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		moveLeft();
	}

	postMove();   // Post-move processing.
}

bool Player::jump()
{
	sf::Vector2i tilePos = getPosition();

	// If there's something above us, don't jump.
	if (isCollisionAt({ tilePos.x, tilePos.y - 1 }))
		return false;
	// If there's nothing below us, don't jump.
	if (!isCollisionAt({ tilePos.x, tilePos.y + 1 }))
		return false;
	// Jump.
	setPosition({ tilePos.x, tilePos.y - 1 });
	return true;
}

void Player::moveRight()
{
	sf::Vector2i tilePos = getPosition();

	// If there's something to the right of us, don't move.
	if (isCollisionAt({ tilePos.x + 1, tilePos.y }))
		return;

	setPosition({ tilePos.x + 1, tilePos.y });
}

void Player::moveLeft()
{
	sf::Vector2i tilePos = getPosition();

	// If there's something to the left of us, don't move.
	if (isCollisionAt({ tilePos.x - 1, tilePos.y }))
		return;

	setPosition({ tilePos.x - 1, tilePos.y });
}

void Player::postMove()
{
	sf::Vector2i tilePos = getPosition();

	// Fall if there's nothing under the player,
	// and we didn't jump that frame.
	if (!isCollisionAt({ tilePos.x, tilePos.y + 1 }))
	{
		if (!mJustJumped)
			setPosition({ tilePos.x, tilePos.y + 1 });
		else
			mJustJumped = false;
	}
}

void Player::setPosition(sf::Vector2i tilePos)
{
	mPlayer.setPosition(getActualPosition(sf::Vector2f(tilePos)) +
						sf::Vector2f(tileSize().x / 2.f, tileSize().y) -   //tile offset
						sf::Vector2f(mPlayer.getSize().x / 2.f,
									 mPlayer.getSize().y));   // player offset
}

sf::Vector2i Player::getPosition()
{
	sf::Vector2i playerCenter = {
		(int)mPlayer.getPosition().x + (int)mPlayer.getSize().x / 2,
		(int)mPlayer.getPosition().y + (int)mPlayer.getSize().y / 2
	};

	return (sf::Vector2i)getGridPosition((sf::Vector2f)playerCenter);
}

void Player::reset()
{
	setPosition(mStartPos);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mPlayer, states);
}

}