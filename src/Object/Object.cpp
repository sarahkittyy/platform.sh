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

ResourceManager& Object::resource()
{
	return *mResource;
}

Object* Object::addObject(Object* object)
{
	return mAddObject(object);
}

void Object::removeObject(Object* object)
{
	mRemoveObject(object);
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
	mUpdatePriorityQueue();
}

void Object::setZIndex(unsigned int zindex)
{
	mZIndex = zindex;
	mUpdateZIndexQueue();
}

const GFX::TiledTilemap& Object::staticTilemap()
{
	return *mStaticTilemap;
}

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
}

}