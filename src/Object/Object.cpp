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

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
}

}