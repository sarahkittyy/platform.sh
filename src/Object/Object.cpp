#include "Level/Level.hpp"
#include "Object/Object.hpp"

namespace Object
{

Object::Object(Props props)
	: mPriority(10),
	  mZIndex(10),
	  mInitialProps(props)
{
}

Object::~Object()
{
}

const std::string Object::name() const
{
	return "Object";
}

const std::string Object::icon() const
{
	return "assets/icons/default.png";
}

const std::string Object::desc() const
{
	return "No description.";
}

void Object::init()
{
}

void Object::update()
{
}

void Object::updateTick()
{
}

void Object::reset()
{
}

unsigned int Object::getPriority()
{
	return mPriority;
}

unsigned int Object::getZIndex()
{
	return mZIndex;
}

bool Object::isSolidAt(sf::Vector2i pos)
{
	return false;
}

const Props& Object::getProps()
{
	return mProps;
}

ResourceManager& Object::resource()
{
	return *mResource;
}

Props& Object::props()
{
	return mProps;
}

Level::Level& Object::level() const
{
	return *mLevel;
}

float Object::interpolationFactor() const
{
	return level().getCurrentClockTime().asSeconds() / level().getTickSpeed().asSeconds();
}

void Object::setPriority(unsigned int priority)
{
	mPriority = priority;
	level().syncPriorityQueue();
}

void Object::setZIndex(unsigned int zindex)
{
	mZIndex = zindex;
	level().syncZIndexQueue();
}

const Props& Object::initialProps()
{
	return mInitialProps;
}

sf::Vector2f Object::getGridPosition(sf::Vector2f actualPos)
{
	return actualPos / (float)level().tileSize().x;
}

sf::Vector2f Object::getActualPosition(sf::Vector2f tilePos)
{
	return tilePos * (float)level().tileSize().x;
}

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
}
}