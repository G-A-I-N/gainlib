// Copyright 2014
//   erkki.salonen@tpnet.fi
//   ville.kankainen@gmail.com
// 
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
