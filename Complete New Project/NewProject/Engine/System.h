#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>

enum SYSTEM_TYPE {
    GRAPHICS = 0,
    ENGINE
};

class System {
public:
	System(int type);
	virtual ~System();

	virtual void init();
	virtual void update(float dt);
	virtual void destroy();

    inline int GetType() { return mType; }
private:
	int mType;
};
    
class SystemContainer {
public:
    static SystemContainer* instance();
    static void destroy();

    void updateSystems();
private:
    SystemContainer();
    ~SystemContainer();
    void registerSystem(System* pSystem);
    void unregisterSystem(System* pSystem);

    static SystemContainer* mInstance;
    std::vector<System*> mSystems;
    friend class System;
};

#endif
