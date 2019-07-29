#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace GFX
{

/**
 * @brief Animateable sprite based around FSMs, with customizable animation states.
 * 
 */
class AnimatedSprite : public sf::Drawable, public sf::Transformable
{
public:
	/// An animation structure.
	struct Animation
	{
		std::vector<int> frames;   // Animation frames
		sf::Time speed;			   // Time per frame. (-1 for unmoving)
	};

	/// Init
	AnimatedSprite();

	/**
	 * @brief Initialize the sprite.
	 * 
	 * @param texture The sprite texture map.
	 * @param size The size of a single animation frame.
	 */
	void init(sf::Texture* texture, sf::Vector2f size);

	/**
	 * @brief Get the size of a single sprite frame.
	 * 
	 * @return sf::Vector2f The size of the rendered sprite.
	 */
	sf::Vector2f getSize();

	/**
	 * @brief Update the animation. Call once per frame.
	 * 
	 */
	void update();

	/**
	 * @brief Add an animation to the sprite.
	 * 
	 * @param name The name of the animation.
	 * @param animation The animation data.
	 */
	void addAnimation(std::string name, Animation animation);

	/**
	 * @brief Remove an animation.
	 * 
	 * @param name The name of the animation.
	 */
	void removeAnimation(std::string name);

	/**
	 * @brief Set the currently running animation.
	 * 
	 * @param name The name of the animation.
	 */
	void setAnimation(std::string name);

	/**
	 * @brief Start the animation.
	 * 
	 */
	void start();

	/**
	 * @brief Stop the animation.
	 * 
	 */
	void stop();

private:
	/// SFML draw() override.
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	/// The sprite itself.
	sf::Sprite mSprite;
	/// The sprite's texture.
	sf::Texture* mTexture;
	/// The size of a single frame.
	sf::Vector2f mFrameSize;
	/// The size of the texture map, in frames.
	sf::Vector2i mGridSize;

	/// Get the current animation's animation rect for the current frame.
	sf::IntRect getAnimationRect();
	/// Get the current animation, or DEFAULT_ANIMATION if it's not set.
	Animation* getCurrentAnimation();

	/// All stored animations.
	std::unordered_map<std::string, Animation> mAnimations;
	/// The current animation.
	Animation* mCurrentAnimation;
	/// The name of the currently running animation.
	std::string mCurrentAnimationName;
	/// The frame of the current animation.
	int mCurrentFrame;
	/// Animation clock.
	sf::Clock mClock;
	/// Whether or not the animation is running.
	bool mRunning = false;


	/// The default animation (first frame, -1 speed).
	Animation DEFAULT_ANIMATION;
};

}