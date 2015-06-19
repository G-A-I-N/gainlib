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

#define CORE_INCLUDE

#include "Core.h"
#include "Base.h"
#include "TouchInterface.h"
#include "PerfomanceCounter.h"
#include "helpers.h"

#undef CORE_INCLUDE

const float performanceCounterDumpTimeIntervalSec = 1.0;

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

namespace Gain {

//#define CORE_PERF

Core::Core() :
     pScene(SCENE_DEFAULT_BACK)
    ,pFps(0)
    ,screen_height(0)
    ,screen_width(0)
    ,reversed_ratio(1)
    ,ratio(1)
    ,scale_width(1)
    ,scale_height(1)
{
    LOCK_INIT(touchClientsLock);

	myCoreUpdateLoopCallCounter.SetName("updateGLoop rate");
	myAppUpdateLoopCallCounter.SetName("updateLoop rate");
	myRenderLoopCallCounter.SetName("RenderLoop rate");
	myRenderClientsCountCounter.SetName("Render clients count");
	mlastPerforManceCounterUpdateTimeSec = 0;
    
    gettimeofday( &gNewTime , NULL);
    memcpy(&gOldTime, &gNewTime, sizeof(gOldTime));
    memcpy(&gStartTime, &gNewTime, sizeof(gStartTime));
    start_msec = gNewTime.tv_sec*1000 + gNewTime.tv_usec/1000;
    old_msec = start_msec;

    for(int n=0;n<MAX_SCENES;n++)
    {
    	pScenes.push_back(new Layer());
    }
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
	normalised_screen_height = reversed_ratio*2.f;


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
    
void Core::backEndUpdateG()
{
	gettimeofday( &gNewTime , NULL);
    
	int new_msec = (int)(gNewTime.tv_sec*1000 + gNewTime.tv_usec/1000);
    
	gSecG      = 0.001f*(float)(new_msec - start_msec);
	gDeltaSecG = 0.001f*(float)(new_msec - old_msec);
    
	old_msec  = new_msec;
	memcpy(&gOldTime, &gNewTime, sizeof(gOldTime));
    
	updateG(gSecG,gDeltaSecG);
}
    
void Core::updateG(float time, float deltaTime)
{

	myCoreUpdateLoopCallCounter.Increment(1);

	int renderClientItems = 0;

	if (pScene > pScenes.size()) {
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

//    LOCK_RELEASE(renderClientsLock);

    LOCK_ACQUIRE(touchClientsLock);
    if (pScenes.size() > SCENE_DEFAULT_BACK) {
		Gain::Layer* current = pScenes[SCENE_DEFAULT_BACK];
		if (current->getState() == NOT_INITIALIZED) {
			current->setupGraphics();
		}
		current->updateG(time, deltaTime);
	}

	if (pScene > SCENE_DEFAULT_FRONT && pScene < pScenes.size()) {
		Gain::Layer* current = pScenes[pScene];
		if (current->getState() == NOT_INITIALIZED) {
			current->setupGraphics();
		}
		current->updateG(time, deltaTime);
	}

	if (pScenes.size() > SCENE_DEFAULT_FRONT) {
		Gain::Layer* current = pScenes[SCENE_DEFAULT_FRONT];
		if (current->getState() == NOT_INITIALIZED) {
			current->setupGraphics();
		}
		current->updateG(time, deltaTime);
	}
    LOCK_RELEASE(touchClientsLock);

    /* Update renderClients performance counter */
    myRenderClientsCountCounter.SetCounterValue(renderClientItems);

}

void Core::update(float time, float deltaTime)
{
	myAppUpdateLoopCallCounter.Increment(1);

	if (pScene > pScenes.size()) {
		return;
	}

	if (pScenes.size() > SCENE_DEFAULT_BACK) {
		Gain::Layer* current = pScenes[SCENE_DEFAULT_BACK];
		if (current->getState() == INITIALIZED) {
			current->update(time, deltaTime);
		}
	}

	if (pScene > SCENE_DEFAULT_FRONT && pScene < pScenes.size()) {
		Gain::Layer* current = pScenes[pScene];
		if (current->getState() == INITIALIZED) {
			current->update(time, deltaTime);
		}
	}

	if (pScenes.size() > SCENE_DEFAULT_FRONT) {
		Gain::Layer* current = pScenes[SCENE_DEFAULT_FRONT];
		if (current->getState() == INITIALIZED) {
			current->update(time, deltaTime);
		}
	}

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

	glClearColor(0.f, 0.f, 0.f, 1.0f);
	checkGlError("glClearColor");
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	checkGlError("glClear");

	if (pScenes.size() > SCENE_DEFAULT_BACK) {
		Gain::Layer* current = pScenes[SCENE_DEFAULT_BACK];
		if (current->getState() == INITIALIZED) {
			current->render();
		}
	}

	if (pScene > SCENE_DEFAULT_FRONT && pScene < pScenes.size()) {
		Gain::Layer* current = pScenes[pScene];
		if (current->getState() == INITIALIZED) {
			current->render();
		}
	}

	if (pScenes.size() > SCENE_DEFAULT_FRONT) {
		Gain::Layer* current = pScenes[SCENE_DEFAULT_FRONT];
		if (current->getState() == INITIALIZED) {
			current->render();
		}
	}
}

void Core::addRenderClient(Base* aBase, unsigned int aScene)
{
	if(aScene >= SCENE_LAST_INDEX)
	{
		aScene = pScene;
	}
	pScenes[aScene]->addRenderClient(aBase);
}

void Core::removeRenderClient(Base* aBase, unsigned int aScene)
{
    if(aScene >= SCENE_LAST_INDEX)
	{
		aScene = pScene;
	}
    pScenes[aScene]->removeRenderClient(aBase);
}

void Core::invalidateAllRenderers(bool fullReset)
{
	LOGSCOPE;
	for (size_t n = 0; n < pScenes.size(); ++n) {
		Gain::Layer* current = pScenes[n];
		current->invalidate();
	}
}

void Core::addTouchClient(TouchInterface* aInterface)
{
    LOCK_ACQUIRE(touchClientsLock);
	LOGSCOPE;
	pTouchClients.push_back(aInterface);
    LOCK_RELEASE(touchClientsLock);
}

void Core::removeTouchClient(TouchInterface* aInterface)
{
    LOCK_ACQUIRE(touchClientsLock);
	LOGSCOPE;
	std::vector<TouchInterface*>::iterator it = pTouchClients.begin();
	while(it != pTouchClients.end())
	{
		if (aInterface == *it) {
			std::vector<TouchInterface*>::iterator it_rem = it++;
			pTouchClients.erase(it_rem);
			return;
		}
	}

    LOCK_RELEASE(touchClientsLock);
}


void Core::offerTouch(TouchPoint* aTouchPoint, TouchType aType)
{
	TouchState touchState = TOUCH_NOT_CONSUMED;
	TouchInterface* interface;
	LOGSCOPE;
	LOCK_ACQUIRE(touchClientsLock);

	//offer to added touch clients first
	if(touchState == TOUCH_NOT_CONSUMED) {
		std::vector<TouchInterface*>::iterator it;

		for(it = pTouchClients.begin();touchState == TOUCH_NOT_CONSUMED && it != pTouchClients.end();++it)
		{
			TouchInterface* current = *it;
			touchState = current->offerTouch(aTouchPoint,aType);
		}
	}

	//offer to front
	if(touchState == TOUCH_NOT_CONSUMED)
	{
		touchState = pScenes[SCENE_DEFAULT_FRONT]->offerTouch(aTouchPoint, aType);
	}
	//current
	if(touchState == TOUCH_NOT_CONSUMED)
	{
		touchState = pScenes[pScene]->offerTouch(aTouchPoint, aType);
	}
	//back
	if(touchState == TOUCH_NOT_CONSUMED)
	{
		touchState = pScenes[SCENE_DEFAULT_BACK]->offerTouch(aTouchPoint, aType);
	}

    LOCK_RELEASE(touchClientsLock);
}

void Core::offerTouchDown(TouchPoint* aTouchPoint)
{
	LOGSCOPE;
	offerTouch(aTouchPoint, TOUCH_DOWN);
}

void Core::offerTouchMove(TouchPoint* aTouchPoint)
{
	LOGSCOPE;
	offerTouch(aTouchPoint, TOUCH_MOVE);
}

void Core::offerTouchUp(TouchPoint* aTouchPoint)
{
	LOGSCOPE;
	offerTouch(aTouchPoint, TOUCH_UP);
}

void Core::initiatePurchase(std::string purchase)
{
	pPurchasesToBeDone.insert(purchase);
}

bool Core::ownsPurchase(std::string purchase)
{
	bool owns = false;
	std::set<std::string>::iterator it = pPurchasesOwned.begin();
	while(!owns && it != pPurchasesOwned.end())
	{
		owns = (*it == purchase);
		++it;
	}
	return owns;
}

std::string Core::backEndGetPurchaseToBeDone()
{
	std::string newPurchase("empty");
	if( !pPurchasesToBeDone.empty() )
	{
		newPurchase = *(pPurchasesToBeDone.begin());
		pPurchasesToBeDone.erase(pPurchasesToBeDone.begin());
	}
	return newPurchase;
}

void Core::backEndSetPurchase(std::string purchase)
{
	LOGI("purchase acquired: %s", purchase.c_str());
	pPurchasesOwned.insert(purchase);
}


} /* namespace Gain */
