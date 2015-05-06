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

#include "Menu.h"
#include <gain.h>

namespace Gain {

Menu::Menu()
: pMenuBg(0), pPointerId(-1),pMenuVisible(false)
{
	// TODO Auto-generated constructor stub

	CORE->addRenderClient( this , SCENE_DEFAULT_FRONT );
	setTouchable();

	pMenuBg = new Rect(-0.95f,-1.3f,1.9f,2.6f);
	pMenuBg->setColor(0,0,0.5,0.8);
	addRenderClient( pMenuBg );
	pMenuBg->addEventListener(this);
	pMenuBg->Base::setPositionN(100,0);

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
		pMenuBg->Base::setPositionN(point->Xn + pXCorrection,0.f);

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
		pMenuBg->Base::setPositionN(point->Xn + pXCorrection,0.f);
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
			pMenuBg->toPositionN(pXCorrection*4.f, 0, 0.3f);
		} else {
			pMenuBg->toPositionN(0,0,0.3f);
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
