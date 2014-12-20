/*
 * Timer.h
 *
 *  Created on: 11 Dec 2014
 *      Author: Ville
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "Base.h"

namespace Gain {

class Timer: public Gain::Base  {
private:
	typedef Gain::Base super;
public:
	Timer();
	virtual ~Timer();
	virtual void updateG(float time, float deltaTime) ;
	virtual bool setupGraphics() ;
	virtual bool initVariables() ;

	virtual void render() const {};
	virtual void enableAttributes() const {};
	virtual void disableAttributes() const {};

	virtual void setIntervalCount(float interval, unsigned int count, bool autoStart=true);
	virtual void setInterval(float interval, bool autoStart=true);
	virtual void setOneShot(float time, bool autoStart=true);
	virtual void stop();
	virtual void start();
protected:
	float pTimerOriginal;
	float pTimerLeft;
	unsigned int pRunning;
	unsigned int pRunningCount;

};

} /* namespace Gain */

#endif /* TIMER_H_ */
