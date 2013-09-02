/*
 * TouchInterface.h
 *
 *  Created on: 23 Apr 2013
 *      Author: w
 */

#ifndef TOUCHINTERFACE_H_
#define TOUCHINTERFACE_H_

namespace Gain {

enum TouchState {
	TOUCH_CONSUMED=0,
	TOUCH_NOT_CONSUMED
};


typedef struct _TouchPoint{
	float X;
	float Y;
	//normalised
	float Xn;
	//normalised
	float Yn;
	int ID;

} TouchPoint;

class TouchInterface {
public:
	TouchInterface();
	virtual ~TouchInterface();

	virtual TouchState TouchDown(TouchPoint* point)=0;
	virtual TouchState TouchMove(TouchPoint* point)=0;
	virtual TouchState TouchUp(TouchPoint* point)=0;
};

} /* namespace Gain */
#endif /* TOUCHINTERFACE_H_ */
