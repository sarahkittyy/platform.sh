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
	mPlatform.start();

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
	// Change the animation if necessary.
	mPlatform.setAnimation(getDirectionAnimation());

	/// Get the current position on the grid.
	sf::Vector2f currentPosition = getGridPosition(mPlatform.getPosition());
	mInitialTickPosition		 = mPlatform.getPosition();   // Save the initial before-tick pos for interpolation.
	//* Position calculations start here
	// Get the x/y offset to move given the direction.
	sf::Vector2f offset = getDirectionOffset();

	// Update "currentPosition" with this offset.
	currentPosition += offset;

	//! Using the new position, find nearby arrowplatformend instances, and rotate as needed.
	updateEndpoints(currentPosition);

	// Update the intended next position, for interpolation.
	mIntendedNextPosition = getActualPosition(currentPosition);
}

void ArrowPlatform::updateEndpoints(sf::Vector2f& currentPosition)
{
	// Get all endpoints.
	auto ends = queryObjects([](const Props& props) -> bool {
		return props.test("/arrowPlatformEnd"_json_pointer, true);
	});

	// Get the next position if we were to continue in this direction.
	sf::Vector2f nextPos = currentPosition + getDirectionOffset();
	// The endpoint next to us, if any.
	Object* nextEndpoint = nullptr;

	for (auto& end : ends)
	{
		if (end->getProps().test("/position/0"_json_pointer, nextPos.x) &&
			end->getProps().test("/position/1"_json_pointer, nextPos.y))
		{
			nextEndpoint = end.get();
			break;
		}
	}

	// If there's no endpoint, return.
	if (!nextEndpoint)
		return;

	// Test for an intended next direction.
	if (nextEndpoint->getProps().exists("/turnTo"_json_pointer))
	{
		// Get the direction string.
		std::string dir = nextEndpoint->getProps().get("/turnTo"_json_pointer).get<std::string>();
		// Convert to a direction, and turn there.
		mDir = getDirection(dir);
	}
	else if (nextEndpoint->getProps().exists("/rotateDegrees"_json_pointer))
	{
		// Get the rotation degrees.
		int degrees = nextEndpoint->getProps().get("/rotateDegrees"_json_pointer).get<int>();
		// Turn that amount of degrees.
		mDir = turnDegrees(degrees);
	}
	else
	{
		//? Shouldn't run.
		throw std::runtime_error("ArrowPlatformEnd initialized with no required valid props.");
	}
}

ArrowPlatform::Direction ArrowPlatform::turnDegrees(int degrees)
{
	// Constrain the degrees values.
	degrees %= 360;
	degrees = std::max(0, degrees);

	// Get the current direction, as an integer.
	int cdir = static_cast<int>(mDir);

	// Test for angle ranges.
	if (degrees < 90)   // 0 rotation here.
	{
		// Nothing..
	}
	else if (degrees < 180)   // 90 rotation here
	{
		cdir += 1;
	}
	else if (degrees < 270)   // 180 rotation here.
	{
		cdir += 2;
	}
	else if (degrees < 360)   // 270 rotation here.
	{
		cdir += 3;
	}
	//* Note: do nothing on 360 degrees, as that's a full cycle.

	// Keep the dir betwen 0 and 3
	cdir %= 3;
	// Return cdir back as a Direction enum object.
	return static_cast<Direction>(cdir);
}

void ArrowPlatform::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mPlatform, states);
}

bool ArrowPlatform::isSolidAt(sf::Vector2i pos)
{
	sf::Vector2i nextPos = static_cast<sf::Vector2i>(getGridPosition(mInitialTickPosition));
	return pos.x == nextPos.x && pos.y == nextPos.y;
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

ArrowPlatform::Direction ArrowPlatform::getDirection(std::string dirStr)
{
	if (dirStr == "left")
		return LEFT;
	else if (dirStr == "right")
		return RIGHT;
	else if (dirStr == "up")
		return UP;
	else
		return DOWN;
}

}