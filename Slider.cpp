// Copyright 2015
//   ville.kankainen@gmail.com
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

#include "Slider.h"
#include "Core.h"
#include "helpers.h"

namespace Gain {

Slider::Slider(float x, float y, float width, float height) :
	super(x,y,width,height),
	pPointerId(-1),
	pCurrentPosition(0.5f),
	pMin(0),
	pMax(0),
	pSlider(new Gain::Rect(0.f,0.f,height*0.5f,height*0.5f)),
	pSliderBackground(new Gain::Rect(x,y,width,height))
{
	pSliderBackground->setColor(0,0,0,0.4f);
	setTouchable();
}

Slider::Slider() :
	super(),
	pPointerId(-1),
	pCurrentPosition(0.5f),
	pMin(0),
	pMax(0),
	pSlider(NULL),
	pSliderBackground(NULL)
{
	setTouchable();
}




Slider::~Slider()
{}
void Slider::render() const
{
	if (pSlider && pSlider->getState() == INITIALIZED) {
		pSlider->render();
	}
	if (pSliderBackground && pSliderBackground->getState() == INITIALIZED) {
		pSliderBackground->render();
	}
}

void Slider::updateG(float time, float deltaTime)
{
	super::updateG(time,deltaTime);

	if (pSlider) {
		if( pSlider->getState() != INITIALIZED) {
			pSlider->setupGraphics();
		}
		pSlider->updateG(time,deltaTime);
	}
	if (pSliderBackground) {
		if(pSliderBackground->getState() != INITIALIZED) {
			pSliderBackground->setupGraphics();
		}
		pSliderBackground->updateG(time,deltaTime);
		pSliderBackground->anim = anim;
	}

}

bool Slider::setupGraphics()
{
	setReady();
	return true;
}
bool Slider::initVariables()
{
	return true;
}
void Slider::enableAttributes() const
{}
void Slider::disableAttributes() const
{}



TouchState Slider::TouchDown(TouchPoint* aPoint)
{
	if(isWithin(aPoint->Xn, aPoint->Yn) && pPointerId == -1)
	{
		pPointerId = aPoint->ID;
		glm::vec4 translated_pos = getTranslatedPos(aPoint->Xn, aPoint->Yn);
		pCurrentPosition = (translated_pos.x - pSquareVertices[SV_TL_X])/pWidth;
		pCurrentPosition = CLAMP(0.f,pCurrentPosition,1.f);
		pSlider->setPositionN(aPoint->Xn, aPoint->Yn, MID_CENTER);

		return TOUCH_CONSUMED;
	}
	return TOUCH_NOT_CONSUMED;
}

TouchState Slider::TouchMove(TouchPoint* aPoint)
{
	if(aPoint->ID == pPointerId)
	{
		glm::vec4 translated_pos = getTranslatedPos(aPoint->Xn, aPoint->Yn);
		pCurrentPosition = (translated_pos.x - pSquareVertices[SV_TL_X])/pWidth;
		pCurrentPosition = CLAMP(0.f,pCurrentPosition,1.f);
		pSlider->setPositionN(aPoint->Xn, aPoint->Yn, MID_CENTER);
		triggerEvent(EVENT_SLIDER_UPDATE_MOVE);
		return TOUCH_CONSUMED;
	}
	return TOUCH_NOT_CONSUMED;
}

TouchState Slider::TouchUp(TouchPoint* aPoint)
{
	if(aPoint->ID == pPointerId)
	{
		pPointerId = -1;

		glm::vec4 translated_pos = getTranslatedPos(aPoint->Xn, aPoint->Yn);
		pCurrentPosition = (translated_pos.x - pSquareVertices[SV_TL_X])/pWidth;
		pCurrentPosition = CLAMP(0.f,pCurrentPosition,1.f);
		triggerEvent(EVENT_SLIDER_UPDATE_END);
		pSlider->setPositionN(aPoint->Xn, aPoint->Yn, MID_CENTER);
		return TOUCH_CONSUMED;
    }
	return TOUCH_NOT_CONSUMED;
}

void Slider::setMin(float min)
{
	pMin = min;
}

void Slider::setMax(float max)
{
	pMax = max;
}

float Slider::getValue()
{
	return pMin + pCurrentPosition*(pMax-pMin);
}


} /* namespace Gain */
