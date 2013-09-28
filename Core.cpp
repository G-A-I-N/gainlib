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

#undef CORE_INCLUDE


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

	myCoreUpdateLoopCallCounter.SetName("updateG loop");
	myAppUpdateLoopCallCounter.SetName("update loop");
	myRenderLoopCallCounter.SetName("Render loop");
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


	//setup childs?
/*	for(size_t n =0;n<children.size();n++) {
		BaseContainer* child = children[n];
		if(child->state == NOT_INITIALIZED) {
			child->base->setupGraphics();
			child->state = INITIALIZED;
			LOGI("initialized a child");
		}
	}
*/ //TODO:required?

    glViewport(0, 0, w, h);
    checkGlError("glViewport");

    return true;
}

void Core::updateG(float time, float deltaTime)
{

	myCoreUpdateLoopCallCounter.Increment(1);

	if (pScene > renderClients.size()) {
		return;
	}
    LOCK_ACQUIRE(renderClientsLock);
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


    if (renderClients.size() > SCENE_DEFAULT_BACK) {
		std::vector<BaseContainer*> current = renderClients[SCENE_DEFAULT_BACK];
		for (size_t n = 0; n < current.size(); n++) {
			BaseContainer* child = current[n];
			if (child->state == NOT_INITIALIZED) {
				child->base->setupGraphics();
				child->state = INITIALIZED;
			}
			child->base->updateG(time, deltaTime);
		}

	}

	if (pScene > SCENE_DEFAULT_FRONT && pScene < renderClients.size()) {
		std::vector<BaseContainer*> current = renderClients[pScene];
		for (size_t n = 0; n < current.size(); n++) {
			BaseContainer* child = current[n];
			if (child->state == NOT_INITIALIZED) {
				child->base->setupGraphics();
				child->state = INITIALIZED;
			}
			child->base->updateG(time, deltaTime);
		}

	}

	if (renderClients.size() > SCENE_DEFAULT_FRONT) {
		std::vector<BaseContainer*> current = renderClients[SCENE_DEFAULT_FRONT];
		for (size_t n = 0; n < current.size(); n++) {
			BaseContainer* child = current[n];
			if (child->state == NOT_INITIALIZED) {
				child->base->setupGraphics();
				child->state = INITIALIZED;
			}
			child->base->updateG(time, deltaTime);
		}

	}
    LOCK_RELEASE(renderClientsLock);

}

void Core::update(float time, float deltaTime)
{
	myAppUpdateLoopCallCounter.Increment(1);

	if (pScene > renderClients.size()) {
		return;
	}
    LOCK_ACQUIRE(renderClientsLock);
	if (renderClients.size() > SCENE_DEFAULT_BACK) {
		std::vector<BaseContainer*> current = renderClients[SCENE_DEFAULT_BACK];
		for (size_t n = 0; n < current.size(); n++) {
			BaseContainer* child = current[n];
			if (child->state == INITIALIZED) {
				child->base->update(time, deltaTime);
			}
		}
	}

	if (pScene > SCENE_DEFAULT_FRONT && pScene < renderClients.size()) {
		std::vector<BaseContainer*> current = renderClients[pScene];
		for (size_t n = 0; n < current.size(); n++) {
			BaseContainer* child = current[n];
			if (child->state == INITIALIZED) {
				child->base->update(time, deltaTime);
			}
		}

	}

	if (renderClients.size() > SCENE_DEFAULT_FRONT) {
		std::vector<BaseContainer*> current = renderClients[SCENE_DEFAULT_FRONT];
		for (size_t n = 0; n < current.size(); n++) {
			BaseContainer* child = current[n];
			if (child->state == INITIALIZED) {
				child->base->update(time, deltaTime);
			}
		}

	}

    LOCK_RELEASE(renderClientsLock);
}

void Core::renderFrame() {
	//	LOGI("renderFrame()");
	myRenderLoopCallCounter.Increment(1);
    LOCK_ACQUIRE(renderClientsLock);
	static float grey = 0.001f;
	pFps ++;

	//grey+=0.001f;
	glClearColor(grey, grey, grey, 1.0f);
	checkGlError("glClearColor");
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	checkGlError("glClear");

	if (renderClients.size() > SCENE_DEFAULT_BACK) {
		std::vector<BaseContainer*> current = renderClients[SCENE_DEFAULT_BACK];
		for (size_t n = 0; n < current.size(); n++) {
			BaseContainer* child = current[n];
			if (child->state == INITIALIZED) {
				child->base->render();
			}
		}

	}

	if (pScene > SCENE_DEFAULT_FRONT && pScene < renderClients.size()) {
		std::vector<BaseContainer*> current = renderClients[pScene];
		for (size_t n = 0; n < current.size(); n++) {
			BaseContainer* child = current[n];
			if (child->state == INITIALIZED) {
				child->base->render();
			}
		}

	}

	if (renderClients.size() > SCENE_DEFAULT_FRONT) {
		std::vector<BaseContainer*> current = renderClients[SCENE_DEFAULT_FRONT];
		for (size_t n = 0; n < current.size(); n++) {
			BaseContainer* child = current[n];
			if (child->state == INITIALIZED) {
				child->base->render();
			}
		}

	}

    LOCK_RELEASE(renderClientsLock);
}

void Core::addRenderClient(Base* aBase, unsigned int aScene)
{

	BaseContainer* container = new BaseContainer();
	container->base = aBase;
	container->state = NOT_INITIALIZED;
    LOCK_ACQUIRE(renderClientsLock);
	LOGSCOPE;
	while (renderClients.size() <= aScene) {
		renderClients.push_back(std::vector<BaseContainer*>(0));
	}
	renderClients[aScene].push_back(container);
    LOCK_RELEASE(renderClientsLock);
}

void Core::removeRenderClient(Base* base, unsigned int aScene) {
    LOCK_ACQUIRE(renderClientsLock);
	LOGSCOPE;
	if (aScene < renderClients.size()) {
		for (size_t n = 0; n < renderClients[aScene].size(); n++)
			if (base == renderClients[aScene][n]->base) {
				renderClients[aScene].erase(renderClients[aScene].begin() + n);
                LOCK_RELEASE(renderClientsLock);
				return;
			}
	}

    LOCK_RELEASE(renderClientsLock);
}

void Core::invalidateAllRenderers(bool fullReset) {
    LOCK_ACQUIRE(renderClientsLock);
	LOGSCOPE;
	for (size_t n = 0; n < renderClients.size(); n++) {
		for (size_t w = 0; w < renderClients[n].size(); w++) {
			renderClients[n][w]->state=NOT_INITIALIZED;
			if(fullReset) {
				renderClients[n][w]->base->invalidate();
			}
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
		for (size_t n = 0; n < touchClients[aScene].size(); n++)
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
	if (touchClients.size() > SCENE_DEFAULT_BACK) {
		std::vector<TouchContainer*> current = touchClients[SCENE_DEFAULT_BACK];
		for (size_t n = 0; n < current.size(); n++) {
			TouchContainer* container = current[n];
			container->touchInterface->TouchDown(aTouchPoint);
		}
	}

	if (pScene > SCENE_DEFAULT_FRONT && pScene < touchClients.size()) {
		std::vector<TouchContainer*> current = touchClients[pScene];
		for (size_t n = 0; n < current.size(); n++) {
			TouchContainer* container = current[n];
			container->touchInterface->TouchDown(aTouchPoint);
		}
	}

	if (touchClients.size() > SCENE_DEFAULT_FRONT) {
		std::vector<TouchContainer*> current = touchClients[SCENE_DEFAULT_FRONT];
		for (size_t n = 0; n < current.size(); n++) {
			TouchContainer* container = current[n];
			container->touchInterface->TouchDown(aTouchPoint);
		}
	}

}

void Core::offerTouchMove(TouchPoint* aTouchPoint)
{
	LOGSCOPE;
	if (touchClients.size() > SCENE_DEFAULT_BACK) {
		std::vector<TouchContainer*> current = touchClients[SCENE_DEFAULT_BACK];
		for (size_t n = 0; n < current.size(); n++) {
			TouchContainer* container = current[n];
			container->touchInterface->TouchMove(aTouchPoint);
		}
	}

	if (pScene > SCENE_DEFAULT_FRONT && pScene < touchClients.size()) {
		std::vector<TouchContainer*> current = touchClients[pScene];
		for (size_t n = 0; n < current.size(); n++) {
			TouchContainer* container = current[n];
			container->touchInterface->TouchMove(aTouchPoint);
		}
	}

	if (touchClients.size() > SCENE_DEFAULT_FRONT) {
		std::vector<TouchContainer*> current = touchClients[SCENE_DEFAULT_FRONT];
		for (size_t n = 0; n < current.size(); n++) {
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
		for(size_t n =0; n<current.size() ; n++) {
			TouchContainer* container = current[n];
			container->touchInterface->TouchUp(aTouchPoint);
		}
	}

	if(pScene > SCENE_DEFAULT_FRONT && pScene < touchClients.size()) {
		std::vector< TouchContainer* > current = touchClients[pScene];
		for(size_t n =0; n<current.size() ; n++) {
			TouchContainer* container = current[n];
			container->touchInterface->TouchUp(aTouchPoint);
		}
	}

	if(touchClients.size() > SCENE_DEFAULT_FRONT) {
		std::vector< TouchContainer* > current = touchClients[SCENE_DEFAULT_FRONT];
		for(size_t n =0; n<current.size() ; n++) {
			TouchContainer* container = current[n];
			container->touchInterface->TouchUp(aTouchPoint);
		}
	}
}


} /* namespace Gain */
