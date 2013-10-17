/*
 * PerfCounterItem.cpp
 *
 *  Created on: 28.9.2013
 *      Author: Erkki
 */

#include "PerfCounterItem.h"
#include "PerfomanceCounter.h"

namespace Gain {

PerfCounterItem::PerfCounterItem() {
	mPerfCounterId = -1;
}

PerfCounterItem::~PerfCounterItem() {
}


bool PerfCounterItem::SetName(const char* name) {
	bool ret = false;

	mPerfCounterId = GetPerformanceCounter()->AddPerformanceCounter(name);
	if (mPerfCounterId!=-1) {
		ret = true;
	}
	return ret;
}

bool PerfCounterItem::Increment(int incrementvalue) {
	bool ret = false;

	if (mPerfCounterId!=-1 ) {
		ret = GetPerformanceCounter()->IncrementPerformanceCounter(mPerfCounterId,incrementvalue);
	}

	return ret;
}

bool PerfCounterItem::SetCounterValue(int newValue) {
	bool ret = false;

	if (mPerfCounterId!=-1 ) {
		ret = GetPerformanceCounter()->SetPerformanceCounter(mPerfCounterId,newValue);
	}

	return ret;
}



 /* namespace Gain */
}
