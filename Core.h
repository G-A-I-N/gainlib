// Copyright 2014
//   erkki.salonen@tpnet.fi
//   ville.kankainen@gmail.com
//   ville.kankainen@kangain.com
// 
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef CORE_H_
#define CORE_H_

#include <sys/time.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <deque>
#include <queue>
#include <set>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "mappings.h"
#include "PerfCounterItem.h"
#include "TouchInterface.h"

#define CORE GetCore()

namespace Gain {

class Base;

enum Scene {
	SCENE_DEFAULT_BACK=0, //rendered each round first
	SCENE_DEFAULT_FRONT, //rendered each round last
	SCENE_FIRST_INDEX,
	SCENE_LAST_INDEX=99, //98 scenes, should be enough for everyone ;)
	SCENE_ACTIVE //points to currently set scene
};

typedef struct _TouchContainer{
	TouchInterface* touchInterface;
} TouchContainer;

typedef struct _BaseQueueContainer
{
    Base* base;
    unsigned int scene;
} BaseQueueContainer;

class Core {
public:
	Core();
	virtual ~Core();

	bool setupGraphics(int w, int h);
	void updateG(float time, float timeDelta);
    
    void backEndUpdateG();
    
	void update(float time, float timeDelta);
	void renderFrame() const;

    void addRenderClient(Base* aBase, unsigned int aScene=SCENE_ACTIVE) ;

    /* Removes render client item from scene. For performance reasons, using
     * aScene, deletion is bit faster. If aScene == -1, then base is looked up
     * from all layers.
     */
    void removeRenderClient(Base* base, unsigned int aScene=SCENE_ACTIVE) ;

    void addTouchClient(TouchInterface* aInterface, unsigned int aScene=SCENE_ACTIVE) ;
    void removeTouchClient(TouchInterface* aInterface, unsigned int aScene=SCENE_ACTIVE) ;

    void setScene(unsigned int aScene) { pScene = aScene;}

	void invalidateAllRenderers(bool fullReset=false);

	void offerTouchDown(TouchPoint* aTouchPoint);
	void offerTouchMove(TouchPoint* aTouchPoint);
	void offerTouchUp(TouchPoint* aTouchPoint);

	void initiatePurchase(std::string purchase);
	bool ownsPurchase(std::string purchase);

	std::string backEndGetPurchaseToBeDone();
	void backEndSetPurchase(std::string purchase);

	float screen_width, screen_height;
	float scale_width, scale_height;
	float ratio, reversed_ratio;
	float normalised_screen_height;
private:
    unsigned int pScene;
    std::vector< std::deque<Base*> > renderClients;
    std::vector< std::vector<TouchContainer*> > touchClients;

    std::queue< BaseQueueContainer > addClientsMultimap;
    std::queue< BaseQueueContainer > removeClientsMultimap;

    std::set< std::string > pPurchasesOwned;
    std::set< std::string > pPurchasesToBeDone;

    LOCK renderClientsLock;
    LOCK touchClientsLock;

	int pFps;

    //timers
    
    struct timeval gStartTime,gNewTime,gOldTime;
    
    float gSecG, gDeltaSecG, gSecU, gDeltaSecU;
    
    int start_msec, old_msec;
	
    /* PerformanceCounters. Initialized when name is set. */
	Gain::PerfCounterItem myCoreUpdateLoopCallCounter;
	Gain::PerfCounterItem myAppUpdateLoopCallCounter;
	Gain::PerfCounterItem myRenderLoopCallCounter;
	Gain::PerfCounterItem myRenderClientsCountCounter;


	/* Performance counters dump timer  */
	float mlastPerforManceCounterUpdateTimeSec;

};

Core* GetCore();



} /* namespace Gain */
#endif /* CORE_H_ */
