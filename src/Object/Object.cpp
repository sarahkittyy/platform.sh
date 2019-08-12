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

void Object::removeObject(Object* object)
{
	mRemoveObject(object);
}

std::vector<std::shared_ptr<Object>>
Object::queryObjects(std::function<bool(const Props&)> query)
{
	return mQueryObjects(query);
}

bool Object::isCollisionAt(sf::Vector2i pos)
{
	return mIsCollisionAt(pos);
}

void Object::setCameraPosition(sf::Vector2f pos)
{
	mSetCameraPosition(pos);
}

sf::Vector2f Object::getCameraPosition()
{
	return mGetCameraPosition();
}

void Object::setViewportSize(sf::Vector2f size)
{
	mSetViewportSize(size);
}

sf::Vector2f Object::getViewportSize()
{
	return mGetViewportSize();
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

float Object::interpolationFactor() const
{
	return mGetCurrentClockTime().asSeconds() / mGetTickRate().asSeconds();
}

void Object::emit(std::string event, nlohmann::json data)
{
	mEmit(event, data);
}

void Object::on(std::string event,
				std::function<void(const nlohmann::json& data)> callback)
{
	mOnEvent(event, callback);
}

const Props& Object::initialProps()
{
	return mInitialProps;
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