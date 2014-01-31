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

	/**
	 * Initialize event for app.
	 *
	 * Gain framework calls Initialize first.
	 *
	 */
	virtual void Initialize();

	/**
	 * Touch event for when touch is pressed (down)
	 *
	 * When touch interface detects hits, it first sends TouchDown event.
	 * Then comes TouchMove if touch position is moved.
	 * When touch is released, TouchUp event is received.
	 *
	 * \param point that indicates position of touch event.
	 * \return TouchState that indicates if touch event was consumed. If yes, it is not propagated further.
	 */
	virtual TouchState TouchDown(TouchPoint* point);
	/**
	 * Touch event for when touch is moved. Before this there has been
	 * Touch Down event.
	 *
	 *
	 * \param point that indicates position of touch event.
	 * \return TouchState that indicates if touch event was consumed. If yes, it is not propagated further.
	 */
	virtual TouchState TouchMove(TouchPoint* point);
	/**
	 * Touch event for when touch is release. Before this there might have been
	 * TouchMove event. TouchDown is always before  TouchUp.
	 *
	 * \param point that indicates position of touch event.
	 * \return TouchState that indicates if touch event was consumed. If yes, it is not propagated further.
	 */
	virtual TouchState TouchUp(TouchPoint* point);

	virtual void onEvent(Base* caller, EventType type) {};

	virtual void Suspend();
	virtual void Resume();
	virtual void AboutToExit();

	virtual void Update(float sec, float deltaSec);
};

} /* namespace Gain */
#endif /* GAINAPP_H_ */
