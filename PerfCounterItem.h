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

	/* Set PerformanceCounter Object name. If not set, nothing is traced */
	bool SetName(const char *name);

	/* Incrmenent performance counter value by increment. Use this if you "add" or call something */
	bool Increment(int incrementvalue);

	/* Set performance counter value by increment. Use this if when report sizes of arrays etc */
	bool SetCounterValue(int newValue);

private:
	int mPerfCounterId;
};


} /* namespace Gain */
#endif /* PERFCOUNTERITEM_H_ */
