/*
 * App.cpp
 *
 *  Created on: 22 Apr 2013
 *      Author: w
 */


#include "TouchInterface.h"
#include "GainApp.h"
#include "gain.h"
#include "Text.h"

namespace Gain {



GainApp::GainApp()
{}

GainApp::~GainApp()
{}

void GainApp::Initialize()
{}


TouchState GainApp::TouchDown(TouchPoint* point)
{
	return TOUCH_NOT_CONSUMED;
}

TouchState GainApp::TouchMove(TouchPoint* point)
{
	return TOUCH_NOT_CONSUMED;
}

TouchState GainApp::TouchUp(TouchPoint* point)
{
	return TOUCH_NOT_CONSUMED;
}

void GainApp::Suspend()
{
	LOGEVENTSTART("GainApp:Suspend");
	LOGSTATE("Suspended","GainApp");
	LOGEVENTSTOP("GainApp:Suspend");
}

void GainApp::Resume()
{
	LOGEVENTSTART("GainApp:Resume");
	LOGSTATE("Active","GainApp");
	LOGEVENTSTOP("GainApp:Resume");
}

void GainApp::AboutToExit()
{
	LOGEVENTSTART("GainApp:AboutToExit");
	LOGSTATE("Exiting","GainApp");
	LOGEVENTSTOP("GainApp:AboutToExit");
}

void GainApp::Update(float sec, float deltaSec)
{}

} /* namespace Gain */
