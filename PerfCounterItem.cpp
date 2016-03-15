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
