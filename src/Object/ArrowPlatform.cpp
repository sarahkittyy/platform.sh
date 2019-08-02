#include "Object/ArrowPlatform.hpp"

namespace Object
{

ArrowPlatform::ArrowPlatform(sf::Vector2f pos, Direction dir)
	: mInitialPosition(pos),
	  mInitialDirection(dir),
	  mInitialTickPosition(pos),
	  mIntendedNextPosition(pos)
{
}

void ArrowPlatform::init()
{
	// Only needs to be updated before the player.
	setPriority(20);
	setZIndex(20);

	// Set the platform as collideable.
	props().set({ { "collideable", true } });

	// Init the platform sprite.
	mPlatform.init(resource().texture("assets/sprites/arrow_platform.png"), { 32, 32 });
	mPlatform.addAnimation("left", { .frames = { 0 }, .speed = sf::seconds(-1) });
	mPlatform.addAnimation("right", { .frames = { 2 }, .speed = sf::seconds(-1) });
	mPlatform.addAnimation("up", { .frames = { 3 }, .speed = sf::seconds(-1) });
	mPlatform.addAnimation("down", { .frames = { 1 }, .speed = sf::seconds(-1) });

	reset();
}

void ArrowPlatform::reset()
{
	mPlatform.setPosition(getActualPosition(mInitialPosition));
	mDir				  = mInitialDirection;
	mInitialTickPosition  = mPlatform.getPosition();
	mIntendedNextPosition = mPlatform.getPosition();
	mPlatform.setAnimation(getDirectionAnimation());
}

void ArrowPlatform::update()
{
	// Update the animated sprite.
	mPlatform.update();
	// Change the animation if necessary.
	mPlatform.setAnimation(getDirectionAnimation());

	//! Interpolation code.
	// Get the difference between the intended next position and the current position.
	sf::Vector2f diff = mIntendedNextPosition - mInitialTickPosition;
	// Scale the diff by an interpolation factor, sped up to emulate a ticking game, while
	// still offering smooth animations.
	diff *= std::min(interpolationFactor() * 2.f, 1.f);

	// Set the actual sprite position to the initial position plus the interpolated difference.
	mPlatform.setPosition(mInitialTickPosition + diff);
}

void ArrowPlatform::updateTick()
{
	// Assert the platform was moved to the correct position after interpolation.
	mPlatform.setPosition(mIntendedNextPosition);

	/// Get the current position on the grid.
	sf::Vector2f currentPosition = getGridPosition(mPlatform.getPosition());
	mInitialTickPosition		 = mPlatform.getPosition();   // Save the initial before-tick pos for interpolation.
	//* Position calculations start here
	// Get the x/y offset to move given the direction.
	sf::Vector2f offset = getDirectionOffset();

	// Update "currentPosition" with this offset.
	currentPosition += offset;

	// Update the intended next position, for interpolation.
	mIntendedNextPosition = getActualPosition(currentPosition);
}

void ArrowPlatform::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mPlatform, states);
}

bool ArrowPlatform::isSolidAt(sf::Vector2i pos)
{
	return pos.x == mIntendedNextPosition.x && pos.y == mIntendedNextPosition.y;
}

std::string ArrowPlatform::getDirectionAnimation()
{
	switch (mDir)
	{
	case LEFT:
		return "left";
	case RIGHT:
		return "right";
	case UP:
		return "up";
	case DOWN:
		return "down";
	default:
		return "error";   // Shouldn't ever run.
	}
}

sf::Vector2f ArrowPlatform::getDirectionOffset()
{
	switch (mDir)
	{
	case LEFT:
		return { -1.f, 0 };
	case RIGHT:
		return { 1.f, 0 };
	case UP:
		return { 0, -1.f };
	case DOWN:
		return { 0, 1.f };
	default:
		return { 0, 0 };   // Shouldn't ever run.
	}
}

}