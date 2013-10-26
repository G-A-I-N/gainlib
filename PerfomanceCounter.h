/*
 * PerfomanceCounter.h
 *
 *  Created on: 28.9.2013
 *      Author: Erkki
 */

#ifndef PERFOMANCECOUNTER_H_
#define PERFOMANCECOUNTER_H_

#include <string>
#include <vector>
#include "gain.h"
using namespace std;

namespace Gain {

class PerfomanceCounter {
public:
	PerfomanceCounter();
	virtual ~PerfomanceCounter();

	/* Sends all current performance counters to logging API. Call this less than 1Hz rate to avoid overhead */
	void DumpCounters();

	/* Sets visualization mode of counters. If on, values are shown top of screen */
	bool VisualizeCounters(bool SetOn);

	/* Toggles visualization mode of counters. Returns status of visualization after toggle  */
	bool ToggleVisualizationOfCounters();

	/* Create new Performancecounter. You receive Id for return. If -1 then adding failed. If already exists,
	 * you get current id  */
	int AddPerformanceCounter(const char *name);

	/* Increment performanceCounter by Name. Name lookup is slow, but usage is easier */
	bool IncrementPerformanceCounter(const char *performanceCounterName,int increment);

	/* Increment performance counter by using performance counter id */
	bool IncrementPerformanceCounter(int PerformanceCounterId,int increment);

	/* Set performanceCounter by Name. Name lookup is slow, but usage is easier */
	bool SetPerformanceCounter(const char *performanceCounterName,int newValue);

	/* Set performance counter by using performance counter id */
	bool SetPerformanceCounter(int PerformanceCounterId,int newValue);

	/* Get Performance counter Id */
	int GetPerformanceCounterId(const char *performanceCounterName);
private:

	int FindPerformanceCounter(const char *name);

	void UpdateCounterValuesToCanvas();

	struct performanceCounterItem {
		string PerformanceCounterName;
		int PerformanceCounterValue;
		Gain::Text *VisualizeTextValueRect;
		Gain::Text *VisualizeTextNameRect;
	};

	vector<performanceCounterItem>mPerformanceCounters;

	Gain::Text *mPerfCounterInfoText;
	bool mbVisualizationIsOn;

};

PerfomanceCounter* GetPerformanceCounter();



} /* namespace Gain */
#endif /* PERFOMANCECOUNTER_H_ */
