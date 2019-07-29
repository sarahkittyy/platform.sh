#include "Object/Player.hpp"

namespace Object
{

Player::Player(sf::Vector2i startPos)
	: mStartPos(startPos)
{
	// Always should be updated first.
	priority = 0;
	zindex   = 0;
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
	// Keep the level camera centered on the player.
	setCameraPosition(mPlayer.getPosition());
}

void Player::reset()
{
	mPlayer.setPosition(getRealPosition(mStartPos));
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
		-(int)tileSize.x / 2,
		-(int)tileSize.y
	};

	// Sum up the player and tile offsets.
	sf::Vector2i offset = playerOffset + tileOffset;

	// Set the player position.
	return sf::Vector2f(mStartPos.x * tileSize.x + offset.x, mStartPos.y * tileSize.y + offset.y);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mPlayer, states);
}

}