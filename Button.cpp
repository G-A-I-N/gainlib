/*
 * Button.cpp
 *
 *  Created on: 12 Dec 2013
 *      Author: w
 */

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

Button::~Button()
{}


void Button::addButtonState(Gain::Rect* aRect, ButtonState aState, ButtonIndex aIndex)
{
	LOCK_ACQUIRE(pButtonStateLock);
	unsigned int size =  pButtonFaces.size();
	while (pButtonFaces.size() <= aIndex) {
		pButtonFaces.push_back(std::vector<Rect*>());
	}
	if(pButtonFaces[aIndex].size() < 2)
	{
		pButtonFaces[aIndex].push_back(NULL);
		pButtonFaces[aIndex].push_back(NULL);
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
		if (active->getState() == NOT_INITIALIZED) {
			active->setupGraphics();
		}

		memcpy(&active->square_vertices, &square_vertices, sizeof(square_vertices));
		active->anim=anim;
//		active->updateG(time,deltaTime);
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
			std::map<EventListener*,EventListener*>::iterator it;
			for(it = pEventListener.begin();it != pEventListener.end();it++)
			{
				it->first->onEvent(this, EVENT_BUTTON);
			}
			return TOUCH_CONSUMED;
		}
    }
	return TOUCH_NOT_CONSUMED;
}


} /* namespace Gain */
