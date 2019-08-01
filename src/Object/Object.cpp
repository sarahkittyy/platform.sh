#include "Object/Object.hpp"

namespace Object
{

Object::Object()
	: mPriority(10),
	  mZIndex(10)
{
}

Object::~Object()
{
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

Object* Object::addObject(Object* object)
{
	return mAddObject(object);
}

void Object::removeObject(Object* object)
{
	mRemoveObject(object);
}

std::vector<std::shared_ptr<Object>>
Object::queryObjects(std::function<bool(const Props&)> query)
{
	return mQueryObjects(query);
}

void Object::setCameraPosition(sf::Vector2f pos)
{
	mSetCameraPosition(pos);
}

sf::Vector2f Object::getCameraPosition()
{
	return mGetCameraPosition();
}

void Object::setViewportScale(float scale)
{
	mSetViewportScale(scale);
}

float Object::getViewportScale()
{
	return mGetViewportScale();
}

void Object::setPriority(unsigned int priority)
{
	mPriority = priority;
	mSyncPriorityQueue();
}

void Object::setZIndex(unsigned int zindex)
{
	mZIndex = zindex;
	mSyncZIndexQueue();
}

const sf::Vector2i& Object::gridSize()
{
	return *mGridSize;
}

const sf::Vector2i& Object::tileSize()
{
	return *mTileSize;
}

sf::Vector2f Object::getGridPosition(sf::Vector2f actualPos)
{
	return actualPos / (float)tileSize().x;
}

sf::Vector2f Object::getActualPosition(sf::Vector2f tilePos)
{
	return tilePos * (float)tileSize().x;
}

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
}
}