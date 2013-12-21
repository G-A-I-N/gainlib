/*
 * EventListener.h
 *
 *  Created on: 18 Dec 2013
 *      Author: w
 */

#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_

namespace Gain {

class Base;

typedef enum _EventType
{
	EVENT_NOTYPE=0,
	EVENT_ANIMATION_FINISHED,
	EVENT_BUTTON
} EventType;

class EventListener {

public:
	EventListener();
	virtual ~EventListener();

	virtual void onEvent(Base* caller, EventType = EVENT_NOTYPE )=0;
};

} /* namespace Gain */


#endif /* EVENTHANDLER_H_ */
