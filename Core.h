/*
 * Core.h
 *
 *  Created on: Feb 2, 2013
 *      Author: villekankainen
 */

#ifndef CORE_H_
#define CORE_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mappings.h"
#include "PerfCounterItem.h"
#include "TouchInterface.h"

#define CORE GetCore()

namespace Gain {

enum Scene {
	SCENE_DEFAULT_BACK=0, //rendered each round first
	SCENE_DEFAULT_FRONT, //rendered each round last
	SCENE_LAST_INDEX
};

enum BaseState {
	NOT_INITIALIZED=0,
	INITIALIZED
};

class Base;
typedef struct _BaseContainer{
	Base* base;
	BaseState state;

} BaseContainer;


typedef struct _TouchContainer{
	TouchInterface* touchInterface;
} TouchContainer;

class Core {
public:
	Core();
	virtual ~Core();

	bool setupGraphics(int w, int h);
	void updateG(float time, float timeDelta);
	void update(float time, float timeDelta);
	void renderFrame();

    void addRenderClient(Base* aBase, unsigned int aScene=SCENE_DEFAULT_BACK) ;

    /* Removes render client item from scene. For performance reasons, using
     * aScene, deletion is bit faster. If aScene == -1, then base is looked up
     * from all layers.
     */
    void removeRenderClient(Base* base, int aScene=-1) ;

    void addTouchClient(TouchInterface* aInterface, unsigned int aScene=SCENE_DEFAULT_BACK) ;
    void removeTouchClient(TouchInterface* aInterface, unsigned int aScene=SCENE_DEFAULT_BACK) ;

    void setScene(unsigned int aScene) { pScene = aScene;}

	void invalidateAllRenderers(bool fullReset=false);

	void offerTouchDown(TouchPoint* aTouchPoint);
	void offerTouchMove(TouchPoint* aTouchPoint);
	void offerTouchUp(TouchPoint* aTouchPoint);

	float screen_width, screen_height;
	float scale_width, scale_height;
	float ratio, reversed_ratio;
private:
    unsigned int pScene;
    std::vector< std::vector<BaseContainer*> > renderClients;
    std::vector< std::vector<TouchContainer*> > touchClients;

    LOCK renderClientsLock;
    LOCK touchClientsLock;

	int pFps;

	/* PerformanceCounters. Initialized when name is set */
	Gain::PerfCounterItem myCoreUpdateLoopCallCounter;
	Gain::PerfCounterItem myAppUpdateLoopCallCounter;
	Gain::PerfCounterItem myRenderLoopCallCounter;
	Gain::PerfCounterItem myRenderClientsCountCounter;


	/* Performance counters dump timer */
	float mlastPerforManceCounterUpdateTimeSec;

};

Core* GetCore();



} /* namespace Gain */
#endif /* CORE_H_ */
