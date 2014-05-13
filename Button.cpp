// Copyright 2014
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

#include "Button.h"

namespace Gain {

Button::Button(float x, float y, float width, float height) :
	pActiveState(ButtonUp),
	pActiveIndex(-1),
	pPointerId(-1),
	Rect(x,y,width,height)
{
	LOCK_INIT(pButtonStateLock);
	CORE->addTouchClient(this);
}

Button::Button() :
	pActiveState(ButtonUp),
	pActiveIndex(-1),
	pPointerId(-1),
	Rect()
{
	LOCK_INIT(pButtonStateLock);
	CORE->addTouchClient(this);
}


Button::~Button()
{}


void Button::addButtonState(Gain::Rect* aRect, ButtonState aState, ButtonIndex aIndex)
{
	LOCK_ACQUIRE(pButtonStateLock);
	while (pButtonFaces.size() <= aIndex) {
		pButtonFaces.resize(aIndex+1);
	}
	if(pButtonFaces[aIndex].size() < 2)
	{
		pButtonFaces[aIndex].resize(2);
	}

	pButtonFaces[aIndex][aState] = aRect;
	if(pActiveIndex < 0) pActiveIndex = aIndex;
	LOCK_RELEASE(pButtonStateLock);
}


void Button::render() const
{
	if(pActiveIndex >= 0 && pActiveIndex < pButtonFaces.size())
	{
		Rect* active = 	pButtonFaces[pActiveIndex][pActiveState];
		if (active && active->getState() == INITIALIZED) {
			active->render();
		}
	}
}

void Button::updateG(float time, float deltaTime)
{
	if(pActiveIndex >= 0 && pActiveIndex < pButtonFaces.size())
	{
		super::updateG(time,deltaTime);

		Rect* active = 	pButtonFaces[pActiveIndex][pActiveState];

		if(!active) {
			return;
		}

		if (active->getState() == NOT_INITIALIZED) {

			active->setupGraphics();
		}

		active->updateG(time,deltaTime);
		memcpy(active->square_vertices, square_vertices, sizeof(square_vertices));
		memcpy(&active->anim,&anim,sizeof(anim));
		memcpy(active->color,color,sizeof(color));
	}
}

bool Button::setupGraphics()
{
	setReady();
	return true;
}
bool Button::initVariables()
{
	return true;
}
void Button::enableAttributes() const
{}
void Button::disableAttributes() const
{}



TouchState Button::TouchDown(TouchPoint* aPoint)
{
	if(isWithin(aPoint->Xn, aPoint->Yn) && pPointerId == -1)
	{
		pActiveState = ButtonDown;
		pPointerId = aPoint->ID;
		return TOUCH_CONSUMED;
	}
	return TOUCH_NOT_CONSUMED;
}

TouchState Button::TouchMove(TouchPoint* aPoint)
{
	if(aPoint->ID == pPointerId)
	{
		if(isWithin(aPoint->Xn, aPoint->Yn))
		{
			return TOUCH_CONSUMED;
		}
		pPointerId = -1;
		pActiveState = ButtonUp;
	}
	return TOUCH_NOT_CONSUMED;
}

TouchState Button::TouchUp(TouchPoint* aPoint)
{
	if(aPoint->ID == pPointerId)
	{
		pPointerId = -1;
		pActiveState = ButtonUp;
	    if(isWithin(aPoint->Xn, aPoint->Yn))
		{
	    	triggerEvent(EVENT_BUTTON);
	    	return TOUCH_CONSUMED;
		}
    }
	return TOUCH_NOT_CONSUMED;
}


Button* Button::setIndex(ButtonIndex aIndex)
{
	pActiveIndex = aIndex;
	return this;
}

Button* Button::nextIndex()
{
	++pActiveIndex;
	if(pActiveIndex >= pButtonFaces.size())
	{
		pActiveIndex=0;
	}
	return this;
}
ButtonIndex Button::getIndex()
{
	return pActiveIndex;
}


} /* namespace Gain */
