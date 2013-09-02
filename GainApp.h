/*
 * App.h
 *
 *  Created on: 22 Apr 2013
 *      Author: w
 */

#ifndef GAINAPP_H_
#define GAINAPP_H_

#include "TouchInterface.h"

namespace Gain {

class GainApp : public TouchInterface {
public:
	GainApp();
	virtual ~GainApp();

	virtual void Initialize();

	virtual TouchState TouchDown(TouchPoint* point);
	virtual TouchState TouchMove(TouchPoint* point);
	virtual TouchState TouchUp(TouchPoint* point);

	virtual void Suspend();
	virtual void Resume();
	virtual void AboutToExit();

	virtual void Update(float sec, float deltaSec);
};

} /* namespace Gain */
#endif /* GAINAPP_H_ */
