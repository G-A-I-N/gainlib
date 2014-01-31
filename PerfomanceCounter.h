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

	/**
	 * Send all performance counters to logging API and resets counter values to 0.
	 *
	 * Call this less than 1Hz to avoid logging overhead to your application.
	 */
	void DumpCounters();

	/**
	 * Toggles visualisation mode of counters on/off.
	 *
	 * When user visualisation is turned on, performance counter framework creates canvas
	 * to top of all GFX and start displaying current values of performance counters. When
	 * visualisation is turned off, canvas is removed.
	 *
	 * \param SetOn Visualisation mode. True sets visualisation on.
	 */
	bool VisualizeCounters(bool SetOn);

	/**
	 * Toggles visualisation mode of counters.
	 *
	 * \return visualisation status after toggle.
	 */
	bool ToggleVisualizationOfCounters();

	/**
	 * Create new performance counter.
	 *
	 * Adds new counter into tables.
	 *
	 * \param name Performancecounter name.
	 * \return id of counter item. If return value is -1, adding failed. If there is already item with that name
	 * you will get existing counter id.
	 */
	int AddPerformanceCounter(const char *name);

	/**
	 * Get performance counter id by name
	 *
	 * For fast update of performance counter, it is better to use id instead of
	 * name lookup. You can use this function for querying id by name.
	 *
	 * \param performanceCounterName
	 * \return id of queried name. If no match, -1 is returned.
	 */
	int GetPerformanceCounterId(const char *performanceCounterName);

	/**
	 * Increment performance counter value
	 *
	 * Name lookup is slow, but usage is easier.
	 *
	 * \param performanceCounterName the name of counter item.
	 * \param increment The increment of counter value.
	 * \return status of operation. True is ok.
	 */
	bool IncrementPerformanceCounter(const char *performanceCounterName,int increment);

	/**
	 * Increment performance counter value by using counter id. This is faster than name lookup.
	 *
	 * \param PerformanceCounterId the id of counter item.
	 * \param increment The increment of counter value.
	 * \return status of operation. True is ok.
	 */
	bool IncrementPerformanceCounter(int PerformanceCounterId,int increment);

	/**
	 * Set performance counter value
	 *
	 * Name lookup is slow, but usage is easier.
	 *
	 * \param performanceCounterName the name of counter item.
	 * \param newValue The new value of counter.
	 * \return status of operation. True is ok.
	 */
	bool SetPerformanceCounter(const char *performanceCounterName,int newValue);

	/**
	 * Set performance counter value
	 *
	 * Name lookup is slow, but usage is easier.
	 *
	 * \param PerformanceCounterId the id of counter item.
	 * \param newValue The new value of counter.
	 * \return status of operation. True is ok.
	 */
	bool SetPerformanceCounter(int PerformanceCounterId,int newValue);

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
