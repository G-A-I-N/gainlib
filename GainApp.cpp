// Copyright 2014
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


#include "TouchInterface.h"
#include "GainApp.h"
#include "gain.h"
#include "Text.h"

namespace Gain {



GainApp::GainApp()
: EventListener(), TouchInterface()
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
