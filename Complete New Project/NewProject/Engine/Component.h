#ifndef COMPONENT_H
#define COMPONENT_H

#include "ComponentTypes.h"
#include <map>
#include <string>

typedef void (*ComponentBuildFunction)(void); // function pointer type
typedef std::map<std::string, ComponentBuildFunction> component_map;

#define REGISTER_COMPONENT(string, function) (std::pair<std::string, ComponentBuildFunction> newPair(string, function); component_map.insert(newPair);)

class Component {
public:
	Component();
	virtual ~Component();
	Component(const Component& copy);
	virtual Component& operator=(Component& rhs);

	unsigned int getID() { return mID; }
	COMPONENT_TYPES getType() { return mCompType; }

	virtual void init() { };
	virtual void update(float time) { };
	virtual void destroy() { };
	virtual void serialize() { };
	virtual void deserialize() { };
private:
	unsigned int mID;
	COMPONENT_TYPES mCompType;
};

#endif
