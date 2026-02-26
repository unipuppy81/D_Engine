#include "Engine/Core/Object.h"

std::atomic<uint64_t> Object::s_idCounter = 1;

Object::Object(ObjectType type)
	: _type(type), _id(s_idCounter.fetch_add(1)), _isDead(false) {

}