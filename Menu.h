/*
 * Menu.h
 *
 *  Created on: 21 Oct 2013
 *      Author: w
 */

#ifndef MENU_H_
#define MENU_H_

#include <Layer.h>
#include <TouchInterface.h>
#include <Rect.h>

namespace Gain {

class Menu : public Gain::Layer, TouchInterface {

public:
	Menu();
	virtual ~Menu();

	//Touch interface
	virtual TouchState TouchDown(TouchPoint* point);
	virtual TouchState TouchMove(TouchPoint* point);
	virtual TouchState TouchUp(TouchPoint* point);

	virtual void update(float sec, float deltaSec);

	virtual void onEvent(Base*, EventType);
protected:
	int pPointerId,pPointerIdMenu;
	Rect* pMenuBg;
	float pXCorrection;
	bool pMenuVisible;
};

} /* namespace Gain */

#endif /* MENU_H_ */
