/*
 * PerfCounterItem.h
 *
 *  Created on: 28.9.2013
 *      Author: Erkki
 */

#ifndef PERFCOUNTERITEM_H_
#define PERFCOUNTERITEM_H_

namespace Gain {

class PerfCounterItem {
public:
	PerfCounterItem();
	virtual ~PerfCounterItem();

	/**
	 * Set performance counter object name. If not set, nothing is traced.
	 *
	 * When performance counter name is set, it object will register itself to
	 * performance counter framework. In return it receives information that can
	 * be used for fast update actual counter.
	 *
	 * \param name Performance counter object name.
	 * \return boolean that indicates success of operation. True is ok.
	 */
	bool SetName(const char *name);

	/**
	 * Set performance counter value by incrementing it. Use this for adding 'delta'.
	 *
	 *  \param incrementvalue Increment or decrement (negative) to current value.
	 *  \return boolean that indicates success of operation. True is ok.
	 */
	bool Increment(int incrementvalue);

	/**
	 * Set performance counter value by replacing current value. Use this for when you know current
	 * value.. size() for example.
	 *
	 *  \param newValue new value of performance counter.
	 *  \return boolean that indicates success of operation. True is ok.
	 */
	bool SetCounterValue(int newValue);

private:
	int mPerfCounterId;
};


} /* namespace Gain */
#endif /* PERFCOUNTERITEM_H_ */
