/*
 * Timer.cpp
 *
 *  Created on: 11 Dec 2014
 *      Author: Ville
 */

#include "Timer.h"
#include "Logger.h"

namespace Gain {

Timer::Timer():
	super(),
	pTimerOriginal(),
	pTimerLeft(0),
	pRunning(0),
	pRunningCount(0)
{
	//add to current scene, and its graphical time
	CORE->addRenderClient(this, 0);
}

Timer::~Timer() {
	// TODO Auto-generated destructor stub
}
void Timer::setIntervalCount(float interval, unsigned int count, bool autoStart)
{
	LOGI("%s %f %u", __FUNCTION__, interval, count);
	pTimerOriginal = interval;
	pRunningCount = count;
	if(autoStart)
	{
		LOGSCOPE;
		start();
	}
}
void Timer::setOneShot(float time, bool autoStart)
{
	setIntervalCount(time, 1, autoStart);
}
void Timer::setInterval(float interval, bool autoStart)
{
	setIntervalCount(interval, 0xffffffff, autoStart);
}

void Timer::start()
{
	pRunning = 1;
	pTimerLeft = pTimerOriginal;
}
void Timer::stop()
{
	pRunning = 0;
}


void Timer::updateG(float /*time*/, float deltaTime)
{
	if(pRunning >0 && pRunningCount>0)
	{
		pTimerLeft-=deltaTime;
		if(pTimerLeft <= 0)
		{
			triggerEvent(EVENT_TIMER);
			pRunningCount--;
			if(pRunningCount>0)
			{
				pTimerLeft += pTimerOriginal;
			}
		}
	}
}
bool Timer::setupGraphics()
{
	setReady();
	return true;
}
bool Timer::initVariables()
{
	return true;
}

} /* namespace Gain */
