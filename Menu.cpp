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
: pMenuBg(0), pPointerId(-1),pMenuVisible(false)
{
	// TODO Auto-generated constructor stub

	CORE->addRenderClient( this , SCENE_DEFAULT_FRONT );
	CORE->addTouchClient( this , SCENE_DEFAULT_FRONT );

	pMenuBg = new Rect(-0.95f,-1.3f,1.9f,2.6f);
	pMenuBg->setColor(0,0,0.5,0.8);
	addRenderClient( pMenuBg );
	pMenuBg->addEventListener(this);
	pMenuBg->setPositionN(100,0,MID_CENTER);

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
	if(abs(point->Xn) > 0.90)
	{
		pPointerId = point->ID;

		if(point->Xn > 0.95)
			pXCorrection = 0.9f;
		else
			pXCorrection = -0.9f;


		if(pMenuVisible)
		{
			pXCorrection*=-1.f;
		}
		pMenuVisible = !pMenuVisible;
		pMenuBg->setPositionN(point->Xn + pXCorrection,0,MID_CENTER);

		return TOUCH_CONSUMED;
	}
	if(pMenuVisible)
	{
		pPointerIdMenu = point->ID;
		return TOUCH_CONSUMED;
	}
	return TOUCH_NOT_CONSUMED;
}

TouchState Menu::TouchMove(TouchPoint* point)
{
	if(point->ID == pPointerId && pMenuBg)
	{
		pMenuBg->setPositionN(point->Xn + pXCorrection,0,MID_CENTER);
		return TOUCH_CONSUMED;
	}
	if(point->ID == pPointerIdMenu)
	{
		return TOUCH_CONSUMED;
	}
	return TOUCH_NOT_CONSUMED;
}

TouchState Menu::TouchUp(TouchPoint* point)
{

	if(point->ID == pPointerId)
	{
		if( abs(point->Xn) > 0.5)
		{
			//return to previous
			pMenuVisible = !pMenuVisible;
		}
		if(!pMenuVisible)
		{
			pMenuBg->moveToN(pXCorrection*4.f, 0, 0.3f);
		} else {
			pMenuBg->moveToN(0,0,0.3f);
		}
		pPointerId = -1;
		return TOUCH_CONSUMED;
	}
	return TOUCH_NOT_CONSUMED;
}

void Menu::onEvent(Base* aBase, EventType)
{
	if(pMenuBg == aBase && abs(pMenuBg->getXN())>1.f)
	{
	}

}

} /* namespace Gain */
