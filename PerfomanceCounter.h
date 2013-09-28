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

using namespace std;

namespace Gain {

class PerfomanceCounter {
public:
	PerfomanceCounter();
	virtual ~PerfomanceCounter();

	/* Sends all current performance counters to logging API. Call this less than 1Hz rate to avoid overhead */
	void DumpCounters();

	/* Create new Performancecounter. You receive Id for return. If -1 then adding failed. If already exists,
	 * you get current id  */
	int AddPerformanceCounter(char *name);

	/* Increment performanceCounter by Name. Name lookup is slow, but usage is easier */
	bool IncrementPerformanceCounter(char *performanceCounterName,int increment);

	/* Increment performance counter by using performance counter id */
	bool IncrementPerformanceCounter(int PerformanceCounterId,int increment);

	/* Set performanceCounter by Name. Name lookup is slow, but usage is easier */
	bool SetPerformanceCounter(char *performanceCounterName,int newValue);

	/* Set performance counter by using performance counter id */
	bool SetPerformanceCounter(int PerformanceCounterId,int newValue);

	/* Get Performance counter Id */
	int GetPerformanceCounterId(char *performanceCounterName);
private:

	int FindPerformanceCounter(char *name);

	struct performanceCounterItem {
		string PerformanceCounterName;
		int PerformanceCounterValue;
	};

	vector<performanceCounterItem>mPerformanceCounters;



};

PerfomanceCounter* GetPerformanceCounter();



} /* namespace Gain */
#endif /* PERFOMANCECOUNTER_H_ */
