#include "Object/Player.hpp"

namespace Object
{

Player::Player()
{
}

Player::Player(Props props)
	: Object(props),
	  mStartPos(Props::toVector<float>(props.get("/startPos"_json_pointer))),
	  mDeathCount(0),
	  mQueuedPush(0, 0),
	  mInitialPosition(mStartPos),
	  mNextPosition(mStartPos)
{
}

Player* Player::create(Props props)
{
	return new Player(props);
}

Player* Player::clone()
{
	return new Player(initialProps());
}

void Player::init()
{
	// The player should always should be updated last, to allow moving platforms to update first.
	setPriority(100);
	// Always draw the player on top.
	setZIndex(0);

	// Init the sound engine.
	mSounds.init(&resource());
	mSounds.setSound("death", "assets/sounds/death.wav");
	mSounds.setSound("jump", "assets/sounds/jump.wav");

	// For objects such as moving platforms to push the player.
	props().set({ { "pushable", true } });

	// Initialize the player sprite.
	mPlayer.init(resource().texture("assets/sprites/player.png"), { 15, 30 });
	mPlayer.addAnimation("default", { .frames = { 0 }, .speed = sf::seconds(-1) });
	mPlayer.setAnimation("default");
	mPlayer.start();

	// Set the origin such that the player's top-left aligns itself perfectly with the tile.
	mPlayer.setOrigin(
		-sf::Vector2f(tileSize().x / 2.f, tileSize().y) +   //tile offset
		sf::Vector2f(mPlayer.getSize().x / 2.f,				//player offset
					 mPlayer.getSize().y));

	reset();
}

void Player::update()
{
	mPlayer.update();

	//! Interpolation algorithm.
	// Get the x/y difference between it and the next position.
	sf::Vector2f diff = sf::Vector2f(mNextPosition - mInitialPosition);
	// Scale the diff by the interpolation factor.
	diff *= std::min(interpolationFactor() * 2.f, 1.f);
	// Set the actual sprite position to the sum of the initial position and the interpolated
	// intended next position.
	setPosition(getActualPosition((sf::Vector2f)mInitialPosition + diff));

	//* Camera centering.
	// Keep the level camera centered on the player.
	setCameraPosition(getPosition() + diff +
					  sf::Vector2f(0, mPlayer.getSize().y / 2.f));
}

void Player::updateTick()
{
	// For fluid interpolation
	setPosition(getActualPosition((sf::Vector2f)mNextPosition));
	mInitialPosition = sf::Vector2i(getGridPosition(getPosition()));

	moveInterpolated(mQueuedPush);

	// Jumping / falling
	if (airborne())
	{
		fall();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && mQueuedPush.y <= 0)
	{
		jump();
	}
	// L/R movement.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && mQueuedPush.x <= 0)
	{
		moveRight();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && mQueuedPush.x >= 0)
	{
		moveLeft();
	}

	// Reset the queue'd push.
	mQueuedPush = sf::Vector2i(0, 0);
}

bool Player::airborne()
{
	sf::Vector2i tilePos = getPositionInterpolated();

	return !isCollisionAt({ tilePos.x, tilePos.y + 1 });
}

bool Player::jump()
{
	sf::Vector2i tilePos = getPositionInterpolated();

	// If there's something above us, don't jump.
	if (isCollisionAt({ tilePos.x, tilePos.y - 1 }))
		return false;
	// If there's nothing below us, don't jump.
	if (!isCollisionAt({ tilePos.x, tilePos.y + 1 }))
		return false;
	// Jump.
	moveInterpolated({ 0, -1 });
	// Play the jump sound.
	mSounds.playSound("jump", 40);

	return true;
}

void Player::fall()
{
	moveInterpolated({ 0, 1 });
}

void Player::moveRight()
{
	sf::Vector2i tilePos = getPositionInterpolated();

	// If there's something to the right of us, don't move.
	if (isCollisionAt({ tilePos.x + 1, tilePos.y }))
		return;

	moveInterpolated({ 1, 0 });
}

void Player::moveLeft()
{
	sf::Vector2i tilePos = getPositionInterpolated();

	// If there's something to the left of us, don't move.
	if (isCollisionAt({ tilePos.x - 1, tilePos.y }))
		return;

	moveInterpolated({ -1, 0 });
}

void Player::pushRight()
{
	sf::Vector2i tilePos = getPositionInterpolated();

	// If there's something to the right of us, we're squished
	if (isCollisionAt({ tilePos.x + 1, tilePos.y }))
		return kill();

	mQueuedPush.x += 1;
}

void Player::pushLeft()
{
	sf::Vector2i tilePos = getPositionInterpolated();

	// If there's something to the left of us, we're squished
	if (isCollisionAt({ tilePos.x - 1, tilePos.y }))
		return kill();

	mQueuedPush.x -= 1;
}

void Player::pushUp()
{
	sf::Vector2i tilePos = getPositionInterpolated();

	// If there's something to the left of us, we're squished
	if (isCollisionAt({ tilePos.x, tilePos.y - 1 }))
		return kill();

	mQueuedPush.y -= 1;
}

void Player::pushDown()
{
	// Squished.
	if (!airborne())
	{
		kill();
	}
}

void Player::kill()
{
	// Increment the death counter.
	mDeathCount++;
	// Play the death sound.
	mSounds.playSound("death");

	// Emit a death event.
	emit("playerKilled", { { "deathCount", mDeathCount } });
	// Reset the player.
	reset();
}

void Player::setPositionInterpolated(sf::Vector2i pos)
{
	mNextPosition = pos;
}

void Player::moveInterpolated(sf::Vector2i offset)
{
	mNextPosition += offset;
}

sf::Vector2i Player::getPositionInterpolated()
{
	return mNextPosition;
}

void Player::reset()
{
	setPosition(getActualPosition(mStartPos));
	mInitialPosition = (sf::Vector2i)mStartPos;
	mNextPosition	= (sf::Vector2i)mStartPos;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mPlayer, states);
}

}