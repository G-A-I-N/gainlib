/*
 * Core.cpp
 *
 *  Created on: Feb 2, 2013
 *      Author: villekankainen
 */

#define CORE_INCLUDE

#include "Core.h"
#include "Base.h"
#include "TouchInterface.h"
#include "PerfomanceCounter.h"

#undef CORE_INCLUDE

const float performanceCounterDumpTimeIntervalSec = 1.0;

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

namespace Gain {

//#define CORE_PERF

Core::Core()
:pScene(SCENE_DEFAULT_BACK),pFps(0)
{
    LOCK_INIT(renderClientsLock);
    LOCK_INIT(touchClientsLock);

	myCoreUpdateLoopCallCounter.SetName("updateGLoop rate");
	myAppUpdateLoopCallCounter.SetName("updateLoop rate");
	myRenderLoopCallCounter.SetName("RenderLoop rate");
	myRenderClientsCountCounter.SetName("Render clients count");
	mlastPerforManceCounterUpdateTimeSec = 0;
}

Core::~Core() {
	// TODO Auto-generated destructor stub
}

Core* GetCore() {

	static Core* core = 0;
	if(!core) {
		core = new Core();
	}
	return core;
}

bool Core::setupGraphics(int w, int h) {
	LOGSCOPE;

    INIT_GL;

	printGLString("Version", GL_VERSION);
	printGLString("Vendor", GL_VENDOR);
	printGLString("Renderer", GL_RENDERER);
	printGLString("Extensions", GL_EXTENSIONS);

	screen_width = w;
	screen_height = h;

	scale_width = 2.0f / w;
	scale_height = 2.0f / h;

	ratio = screen_width/screen_height;
	reversed_ratio = screen_height/screen_width;


	LOGI("setupGraphics(%d, %d)", w, h);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glDisable( GL_DEPTH_TEST );
    glEnable(GL_BLEND );

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    checkGlError("glBlendFunc");


    glViewport(0, 0, w, h);
    checkGlError("glViewport");

    return true;
}

void Core::updateG(float time, float deltaTime)
{

	myCoreUpdateLoopCallCounter.Increment(1);

	int renderClientItems = 0;

	if (pScene > renderClients.size()) {
		return;
	}

	//lock add and removal
//    LOCK_ACQUIRE(renderClientsLock);
#if 0
	LOGSCOPE;

    static int lastTime = 0;
    int newTime = time;
    if(newTime != lastTime)
	{
		char buf[32];
		sprintf(buf, "fps(%d)\0",pFps);

		LOGI(buf);
		pFps=0;
	}
    lastTime = time;
#endif

    while(addClientsMultimap.size())
    {
    	BaseQueueContainer container = addClientsMultimap.front();
    	renderClients[container.scene].push_back(container.base);
    	addClientsMultimap.pop();
    }
    while(removeClientsMultimap.size())
    {

    	BaseQueueContainer container = removeClientsMultimap.front();

    	Base* forDelete = container.base;
    	removeClientsMultimap.pop();

    	//remove from all scenes
    	for(size_t n = 0;n<renderClients.size();++n)
    	{
            std::deque<Base*>::iterator it =
            		renderClients[n].begin();
            while(it != renderClients[n].end())
            {
            	if(*it == forDelete)
            	{
            		renderClients[n].erase(it);
            		it = renderClients[n].end();
            	} else {
            		++it;
            	}
            }
    	}
    	delete forDelete;
    	forDelete = 0;

	}
//    LOCK_RELEASE(renderClientsLock);

    if (renderClients.size() > SCENE_DEFAULT_BACK) {
		std::deque<Base*> current = renderClients[SCENE_DEFAULT_BACK];
		std::deque<Base*>::iterator it;
		for (it = current.begin();it != current.end(); ++it) {
			Base* child = *it;
			if (child->getState() == NOT_INITIALIZED) {
				child->setupGraphics();
			}
			child->updateG(time, deltaTime);
			renderClientItems++;
		}

	}

	if (pScene > SCENE_DEFAULT_FRONT && pScene < renderClients.size()) {
		std::deque<Base*> current = renderClients[pScene];
		std::deque<Base*>::iterator it;
		for (it = current.begin();it != current.end(); ++it) {
			Base* child = *it;
			if (child->getState() == NOT_INITIALIZED) {
				child->setupGraphics();
			}
			child->updateG(time, deltaTime);
			renderClientItems++;
		}

	}

	if (renderClients.size() > SCENE_DEFAULT_FRONT) {
		std::deque<Base*> current = renderClients[SCENE_DEFAULT_FRONT];
		std::deque<Base*>::iterator it;
		for (it = current.begin();it != current.end(); ++it) {
			Base* child = *it;
			if (child->getState() == NOT_INITIALIZED) {
				child->setupGraphics();
			}
			child->updateG(time, deltaTime);
			renderClientItems++;
		}

	}

    /* Update renderClients performance counter */
    myRenderClientsCountCounter.SetCounterValue(renderClientItems);

}

void Core::update(float time, float deltaTime)
{
	myAppUpdateLoopCallCounter.Increment(1);

	if (pScene > renderClients.size()) {
		return;
	}
    LOCK_ACQUIRE(renderClientsLock);
	if (renderClients.size() > SCENE_DEFAULT_BACK) {
		std::deque<Base*> current = renderClients[SCENE_DEFAULT_BACK];
		std::deque<Base*>::iterator it;
		for (it = current.begin();it != current.end(); ++it) {
			Base* child = *it;
			if (child->getState() == INITIALIZED) {
				child->update(time, deltaTime);
			}
		}
	}

	if (pScene > SCENE_DEFAULT_FRONT && pScene < renderClients.size()) {
		std::deque<Base*> current = renderClients[pScene];
		std::deque<Base*>::iterator it;
		for (it = current.begin();it != current.end(); ++it) {
			Base* child = *it;
			if (child->getState() == INITIALIZED) {
				child->update(time, deltaTime);
			}
		}

	}

	if (renderClients.size() > SCENE_DEFAULT_FRONT) {
		std::deque<Base*> current = renderClients[SCENE_DEFAULT_FRONT];
		std::deque<Base*>::iterator it;
		for (it = current.begin();it != current.end(); ++it) {
			Base* child = *it;
			if (child->getState() == INITIALIZED) {
				child->update(time, deltaTime);
			}
		}

	}

    LOCK_RELEASE(renderClientsLock);

    /* Performance counters dump position */
    mlastPerforManceCounterUpdateTimeSec+=deltaTime;
	if (mlastPerforManceCounterUpdateTimeSec>performanceCounterDumpTimeIntervalSec) {
		GetPerformanceCounter()->DumpCounters();
		mlastPerforManceCounterUpdateTimeSec=0;
	}
}

void Core::renderFrame() const
{
	//	LOGI("renderFrame()");
	(const_cast<Gain::PerfCounterItem*>(&myRenderLoopCallCounter))->Increment(1);
	(*(const_cast<int*>(&pFps))) ++;

	glClearColor(0.f, 1.f, 1.f, 1.0f);
	checkGlError("glClearColor");
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	checkGlError("glClear");

	if (renderClients.size() > SCENE_DEFAULT_BACK) {
		std::deque<Base*> current = renderClients[SCENE_DEFAULT_BACK];
		std::deque<Base*>::iterator it;
		for (it = current.begin();it != current.end(); ++it) {
			Base* child = *it;
			if (child->getState() == INITIALIZED) {
				child->render();
			}
		}

	}

	if (pScene > SCENE_DEFAULT_FRONT && pScene < renderClients.size()) {
		std::deque<Base*> current = renderClients[pScene];
		std::deque<Base*>::iterator it;
		for (it = current.begin();it != current.end(); ++it) {
			Base* child = *it;
			if (child->getState() == INITIALIZED) {
				child->render();
			}
		}

	}

	if (renderClients.size() > SCENE_DEFAULT_FRONT) {
		std::deque<Base*> current = renderClients[SCENE_DEFAULT_FRONT];
		std::deque<Base*>::iterator it;
		for (it = current.begin();it != current.end(); ++it) {
			Base* child = *it;
			if (child->getState() == INITIALIZED) {
				child->render();
			}
		}

	}
}

void Core::addRenderClient(Base* aBase, unsigned int aScene)
{

	if(aScene >= SCENE_LAST_INDEX)
	{
		aScene = pScene;
	}
    LOCK_ACQUIRE(renderClientsLock);
	LOGSCOPE;
	while (renderClients.size() <= aScene) {
		renderClients.push_back(std::deque<Base*>());
	}
	addClientsMultimap.push((BaseQueueContainer){aBase,aScene});
    LOCK_RELEASE(renderClientsLock);
}

void Core::removeRenderClient(Base* aBase, int aScene) {
	LOCK_ACQUIRE(renderClientsLock);
	LOGSCOPE;
	removeClientsMultimap.push((BaseQueueContainer){aBase,aScene});
	LOCK_RELEASE(renderClientsLock);
}

void Core::invalidateAllRenderers(bool fullReset) {
    LOCK_ACQUIRE(renderClientsLock);
	LOGSCOPE;
	for (size_t n = 0; n < renderClients.size(); ++n) {

		std::deque<Base*> current = renderClients[n];
		std::deque<Base*>::iterator it;
		for (it = current.begin();it != current.end(); ++it) {
			Base* child = *it;
			child->invalidate();
		}
	}
    LOCK_RELEASE(renderClientsLock);
}

void Core::addTouchClient(TouchInterface* aInterface, unsigned int aScene)
{
	TouchContainer* container = new TouchContainer();
	container->touchInterface = aInterface;
    LOCK_ACQUIRE(touchClientsLock);
	LOGSCOPE;
	while (touchClients.size() <= aScene) {
		touchClients.push_back(std::vector<TouchContainer*>(0));
	}
	touchClients[aScene].push_back(container);
    LOCK_RELEASE(touchClientsLock);
}

void Core::removeTouchClient(TouchInterface* aInterface, unsigned int aScene)
{
    LOCK_ACQUIRE(touchClientsLock);
	LOGSCOPE;
	if (aScene < touchClients.size()) {
		for (size_t n = 0; n < touchClients[aScene].size(); ++n)
			if (aInterface == touchClients[aScene][n]->touchInterface) {
				touchClients[aScene].erase(touchClients[aScene].begin() + n);
				return;
			}
	}

    LOCK_RELEASE(touchClientsLock);
}


void Core::offerTouchDown(TouchPoint* aTouchPoint)
{
	LOGSCOPE;
	TouchState touchState = TOUCH_NOT_CONSUMED;

	if (touchClients.size() > SCENE_DEFAULT_FRONT) {
		std::vector<TouchContainer*> current = touchClients[SCENE_DEFAULT_FRONT];
		for (size_t n = 0; n < current.size() && touchState; ++n) {
			TouchContainer* container = current[n];
			touchState = container->touchInterface->TouchDown(aTouchPoint);
		}
	}

	if (pScene > SCENE_DEFAULT_FRONT && pScene < touchClients.size()) {
		std::vector<TouchContainer*> current = touchClients[pScene];
		for (size_t n = 0; n < current.size() && touchState; ++n) {
			TouchContainer* container = current[n];
			touchState = container->touchInterface->TouchDown(aTouchPoint);
		}
	}

	if (touchClients.size() > SCENE_DEFAULT_BACK) {
		std::vector<TouchContainer*> current = touchClients[SCENE_DEFAULT_BACK];
		for (size_t n = 0; n < current.size() && touchState; ++n) {
			TouchContainer* container = current[n];
			touchState = container->touchInterface->TouchDown(aTouchPoint);
		}
	}
}

void Core::offerTouchMove(TouchPoint* aTouchPoint)
{
	LOGSCOPE;
	if (touchClients.size() > SCENE_DEFAULT_BACK) {
		std::vector<TouchContainer*> current = touchClients[SCENE_DEFAULT_BACK];
		for (size_t n = 0; n < current.size(); ++n) {
			TouchContainer* container = current[n];
			container->touchInterface->TouchMove(aTouchPoint);
		}
	}

	if (pScene > SCENE_DEFAULT_FRONT && pScene < touchClients.size()) {
		std::vector<TouchContainer*> current = touchClients[pScene];
		for (size_t n = 0; n < current.size(); ++n) {
			TouchContainer* container = current[n];
			container->touchInterface->TouchMove(aTouchPoint);
		}
	}

	if (touchClients.size() > SCENE_DEFAULT_FRONT) {
		std::vector<TouchContainer*> current = touchClients[SCENE_DEFAULT_FRONT];
		for (size_t n = 0; n < current.size(); ++n) {
			TouchContainer* container = current[n];
			container->touchInterface->TouchMove(aTouchPoint);
		}
	}

}

void Core::offerTouchUp(TouchPoint* aTouchPoint)
{
	LOGSCOPE;
	if(touchClients.size() > SCENE_DEFAULT_BACK) {
		std::vector< TouchContainer* > current = touchClients[SCENE_DEFAULT_BACK];
		for(size_t n =0; n<current.size() ; ++n) {
			TouchContainer* container = current[n];
			container->touchInterface->TouchUp(aTouchPoint);
		}
	}

	if(pScene > SCENE_DEFAULT_FRONT && pScene < touchClients.size()) {
		std::vector< TouchContainer* > current = touchClients[pScene];
		for(size_t n =0; n<current.size() ; ++n) {
			TouchContainer* container = current[n];
			container->touchInterface->TouchUp(aTouchPoint);
		}
	}

	if(touchClients.size() > SCENE_DEFAULT_FRONT) {
		std::vector< TouchContainer* > current = touchClients[SCENE_DEFAULT_FRONT];
		for(size_t n =0; n<current.size() ; ++n) {
			TouchContainer* container = current[n];
			container->touchInterface->TouchUp(aTouchPoint);
		}
	}
}


} /* namespace Gain */
