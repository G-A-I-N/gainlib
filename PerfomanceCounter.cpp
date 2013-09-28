/*
 * PerfomanceCounter.cpp
 *
 *  Created on: 28.9.2013
 *      Author: Erkki
 */

#include "gain.h"

#include "PerfomanceCounter.h"

namespace Gain {

PerfomanceCounter* GetPerformanceCounter() {

	static PerfomanceCounter* perf = 0;
	if(!perf) {
		perf = new PerfomanceCounter();
	}
	return perf;
}

PerfomanceCounter::PerfomanceCounter() {
	// TODO Auto-generated constructor stub

}

PerfomanceCounter::~PerfomanceCounter() {
	// TODO Auto-generated destructor stub
}


void PerfomanceCounter::DumpCounters() {

	for (int k=0;k<mPerformanceCounters.size();k++) {

		performanceCounterItem myCounter;

		myCounter = mPerformanceCounters[k];

		/* Use TimeToPic format */
		LOGVALUE(myCounter.PerformanceCounterName.c_str(),myCounter.PerformanceCounterValue);

		/* reset value */
		/* Note that mutex protection is not added. In some cases result may be inaccurate if simultaneuous access
		 * to same item. For performance reasones, mutex is not added
		 */
		mPerformanceCounters[k].PerformanceCounterValue = 0;
	}

}

int PerfomanceCounter::FindPerformanceCounter(char *name) {

	int ret = -1;
	/* Slow linear search */
	for (int k=0;k<mPerformanceCounters.size();k++) {
		if (mPerformanceCounters[k].PerformanceCounterName==name) {
			ret = k;
			break;
		}
	}

	return ret;
}

int PerfomanceCounter::AddPerformanceCounter(char *name) {

	int ret = FindPerformanceCounter(name);

	if (ret==-1) {
		/* Add */
		performanceCounterItem myCounter;
		myCounter.PerformanceCounterName = string(name);
		myCounter.PerformanceCounterValue = 0;

		mPerformanceCounters.push_back (myCounter);
		ret = mPerformanceCounters.size()-1;
	}
	else {
		/* reuse */
	}
	return ret;
}


bool PerfomanceCounter::IncrementPerformanceCounter(char *performanceCounterName,int increment) {
	bool ret = false;

	int PerfCounterId = FindPerformanceCounter(performanceCounterName);
	if (PerfCounterId!=-1) {
		mPerformanceCounters[PerfCounterId].PerformanceCounterValue+= increment;
		ret = true;
	}
	return ret;
}


bool PerfomanceCounter::IncrementPerformanceCounter(int PerformanceCounterId,int increment) {

	bool ret = false;
	if (PerformanceCounterId>=0 && PerformanceCounterId<mPerformanceCounters.size()) {
		mPerformanceCounters[PerformanceCounterId].PerformanceCounterValue+= increment;
		ret = true;
	}

	return ret;
}

bool PerfomanceCounter::SetPerformanceCounter(char *performanceCounterName,int newValue) {
	bool ret = false;

	int PerfCounterId = FindPerformanceCounter(performanceCounterName);
	if (PerfCounterId!=-1) {
		mPerformanceCounters[PerfCounterId].PerformanceCounterValue= newValue;
		ret = true;
	}
	return ret;
}

bool  PerfomanceCounter::SetPerformanceCounter(int PerformanceCounterId,int newValue) {
	bool ret = false;
	if (PerformanceCounterId>=0 && PerformanceCounterId<mPerformanceCounters.size()) {
		mPerformanceCounters[PerformanceCounterId].PerformanceCounterValue= newValue;
		ret = true;
	}
	return ret;
}



int PerfomanceCounter::GetPerformanceCounterId(char *performanceCounterName) {
	int ret = 0;
	ret =  FindPerformanceCounter(performanceCounterName);
	return ret;
}

} /* namespace Gain */
