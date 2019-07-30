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
	sf::Vector2i tilePos = getCurrentTilePosition();

	// If there's something above us, don't jump.
	if (staticTilemap().getTile(tilePos.x, tilePos.y - 1) != 0)
		return false;
	// If there's nothing below us, don't jump.
	if (staticTilemap().getTile(tilePos.x, tilePos.y + 1) == 0)
		return false;

	// Jump.
	setCurrentTilePosition({ tilePos.x, tilePos.y - 1 });
	return true;
}

void Player::moveRight()
{
	sf::Vector2i tilePos = getCurrentTilePosition();

	// If there's something to the right of us..
	if (staticTilemap().getTile(tilePos.x + 1, tilePos.y) != 0)
		return;   // Don't move

	setCurrentTilePosition({ tilePos.x + 1, tilePos.y });
}

void Player::moveLeft()
{
	sf::Vector2i tilePos = getCurrentTilePosition();

	// If there's something to the left of us..
	if (staticTilemap().getTile(tilePos.x - 1, tilePos.y) != 0)
		return;   // Don't move

	setCurrentTilePosition({ tilePos.x - 1, tilePos.y });
}

void Player::postMove()
{
	sf::Vector2i tilePos = getCurrentTilePosition();
	// Fall if there's nothing under the player,
	// and we didn't jump that frame.
	if (staticTilemap().getTile(tilePos.x, tilePos.y + 1) == 0)
	{
		if (!mJustJumped)
			setCurrentTilePosition({ tilePos.x, tilePos.y + 1 });
		else
			mJustJumped = false;
	}
}

void Player::setCurrentTilePosition(sf::Vector2i tilePos)
{
	mPlayer.setPosition(getRealPosition(tilePos));
}

sf::Vector2i Player::getCurrentTilePosition()
{
	sf::Vector2i tileSize	 = staticTilemap().getTileSize();
	sf::Vector2i playerCenter = {
		(int)mPlayer.getPosition().x + (int)mPlayer.getSize().x / 2,
		(int)mPlayer.getPosition().y + (int)mPlayer.getSize().y / 2
	};

	int x, y;
	x = std::floor(playerCenter.x / tileSize.x);
	y = std::floor(playerCenter.y / tileSize.y);
	return { x, y };
}

void Player::reset()
{
	setCurrentTilePosition(mStartPos);
}

sf::Vector2f Player::getRealPosition(sf::Vector2i tilePos)
{
	// Get the tile size
	sf::Vector2i tileSize = staticTilemap().getTileSize();
	// Get the offset to center the player on the bottom-middle.
	sf::Vector2i playerOffset = {
		-(int)mPlayer.getSize().x / 2,
		-(int)mPlayer.getSize().y
	};

	// Center the player's center on the bottom middle of the tile.
	sf::Vector2i tileOffset = {
		(int)tileSize.x / 2,
		(int)tileSize.y
	};

	// Sum up the player and tile offsets.
	sf::Vector2i offset = playerOffset + tileOffset;

	// Set the player position.
	return sf::Vector2f(tilePos.x * tileSize.x + offset.x, tilePos.y * tileSize.y + offset.y);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mPlayer, states);
}

}