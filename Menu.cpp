/*
 * Menu.cpp
 *
 *  Created on: 21 Oct 2013
 *      Author: w
 */

#include "Menu.h"
#include <gain.h>

namespace Gain {

Menu::Menu()
: pMenuBg(0), pPointerId(-1)
{
	// TODO Auto-generated constructor stub

	CORE->addRenderClient( this , SCENE_DEFAULT_FRONT );
	CORE->addTouchClient( this , SCENE_DEFAULT_FRONT );
}

Menu::~Menu()
{
	// TODO Auto-generated destructor stub
}

void Menu::update(float sec, float deltaSec)
{

}


TouchState Menu::TouchDown(TouchPoint* point)
{
	if(!pMenuBg && abs(point->Xn) > 0.95)
	{
		pMenuBg = new Rect(-0.95f,-1.3f,1.9f,2.6f);
		pMenuBg->setPositionN(0,0,MID_CENTER);
		pMenuBg->setColor(0,0,0.5,0.8);
		addRenderClient( pMenuBg );
		return TOUCH_CONSUMED;
	}
	if(pMenuBg)
	{
		pPointerId = point->ID;
		return TOUCH_CONSUMED;
	}
	return TOUCH_NOT_CONSUMED;
}

TouchState Menu::TouchMove(TouchPoint* point)
{
	if(point->ID == pPointerId)
	{
		return TOUCH_CONSUMED;
	}
	return TOUCH_NOT_CONSUMED;
}

TouchState Menu::TouchUp(TouchPoint* point)
{

	if(pMenuBg && abs(point->Xn) > 0.95)
	{
		removeRenderClient(pMenuBg);
		pMenuBg = 0;
		return TOUCH_CONSUMED;
	}
	if(point->ID == pPointerId)
	{
		pPointerId = -1;
		return TOUCH_CONSUMED;
	}
	return TOUCH_NOT_CONSUMED;
}

} /* namespace Gain */
