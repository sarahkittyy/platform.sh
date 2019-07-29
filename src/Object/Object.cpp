#include "Object/Object.hpp"

namespace Object
{

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

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
}

}