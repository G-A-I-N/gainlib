/*
 * App.h
 *
 *  Created on: 22 Apr 2013
 *      Author: w
 */

#ifndef GAINAPP_H_
#define GAINAPP_H_

#include "TouchInterface.h"
#include "EventListener.h"

namespace Gain {

class GainApp : public TouchInterface, EventListener {
public:
	GainApp();
	virtual ~GainApp();

	virtual void Initialize();

	virtual TouchState TouchDown(TouchPoint* point);
	virtual TouchState TouchMove(TouchPoint* point);
	virtual TouchState TouchUp(TouchPoint* point);

	virtual void onEvent(Base* caller, EventType type) {};

	virtual void Suspend();
	virtual void Resume();
	virtual void AboutToExit();

	virtual void Update(float sec, float deltaSec);
};

} /* namespace Gain */
#endif /* GAINAPP_H_ */
