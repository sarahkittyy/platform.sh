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

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
}

}