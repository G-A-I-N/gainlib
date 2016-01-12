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

#include <string>
#include <sstream>
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
	mbVisualizationIsOn = false;
	mPerfCounterInfoText = NULL;

}

PerfomanceCounter::~PerfomanceCounter() {
}

bool PerfomanceCounter::ToggleVisualizationOfCounters() {

	VisualizeCounters(!mbVisualizationIsOn);

	return mbVisualizationIsOn;
}

bool PerfomanceCounter::VisualizeCounters(bool SetOn){

	Core* core = GetCore();
	bool ret = false;

	if (SetOn==true) {

		if (mbVisualizationIsOn==false) {

			int x,y;
			const int textHeight = 24;
			int yOffset = core->screen_height/2-(mPerformanceCounters.size()*textHeight)/2;

			x = core->screen_width*(1/16.0);
			y = -1*textHeight+yOffset;

			assert(mPerfCounterInfoText==NULL);
			mPerfCounterInfoText = new Gain::Text((int)x, (int)y, (int)textHeight,"Perfomance counters");
			mPerfCounterInfoText->setColor(1.f, 0.0f, 0.0f, 1.0f);
			mPerfCounterInfoText->setPlacement(TOP_LEFT);
			Gain::GetCore()->addRenderClient(mPerfCounterInfoText,SCENE_DEFAULT_FRONT);

			for (unsigned int k=0;k<mPerformanceCounters.size();k++) {

				assert( mPerformanceCounters[k].VisualizeTextNameRect==NULL);
				assert( mPerformanceCounters[k].VisualizeTextValueRect==NULL);

				y = k*textHeight+yOffset;

				if (y<core->screen_height) { // Show only items that fits to screen

					x = core->screen_width*(1/16.0);
					Gain::Text *myHeaderText = new Gain::Text((int)x, (int)y, (int)textHeight,mPerformanceCounters[k].PerformanceCounterName.c_str());
					myHeaderText->setColor(1.f, 0.0f, 0.0f, 1.0f);
					myHeaderText->setPlacement(TOP_LEFT);
					Gain::GetCore()->addRenderClient(myHeaderText,SCENE_DEFAULT_FRONT);
					mPerformanceCounters[k].VisualizeTextNameRect = myHeaderText;

					x = core->screen_width*(3/4.0);
					Gain::Text *myValueText = new Gain::Text((int)x, y, textHeight,"0");
					myValueText->setColor(1.f, 0.0f, 0.0f, 1.0f);
					myValueText->setPlacement(TOP_LEFT);
					Gain::GetCore()->addRenderClient(myValueText,SCENE_DEFAULT_FRONT);
					mPerformanceCounters[k].VisualizeTextValueRect = myValueText;
				}
			}
			mbVisualizationIsOn = true;
			ret = true;
		}
	}
	else {

		if (mbVisualizationIsOn==true) {

			if (mPerfCounterInfoText!=NULL) {
				mPerfCounterInfoText->setColor(1,1,1,0);
				core->removeRenderClient(mPerfCounterInfoText);
				mPerfCounterInfoText = NULL; // deleted by render client
			}

			for (unsigned int k=0;k<mPerformanceCounters.size();k++) {

				if(mPerformanceCounters[k].VisualizeTextNameRect!=NULL) {
				mPerformanceCounters[k].VisualizeTextNameRect->setColor(1,1,1,0);
				core->removeRenderClient(mPerformanceCounters[k].VisualizeTextNameRect);
				mPerformanceCounters[k].VisualizeTextNameRect = NULL; // deleted by render client
				}

				if(mPerformanceCounters[k].VisualizeTextValueRect!=NULL) {
				mPerformanceCounters[k].VisualizeTextValueRect->setColor(1,1,1,0);
				core->removeRenderClient(mPerformanceCounters[k].VisualizeTextValueRect);
				mPerformanceCounters[k].VisualizeTextValueRect = NULL; // deleted by render client
				}
			}
			mbVisualizationIsOn = false;
			ret = true;
		}
	}
	return ret;
}


void PerfomanceCounter::DumpCounters() {

	UpdateCounterValuesToCanvas();

	for (unsigned int k=0;k<mPerformanceCounters.size();k++) {

		/* Use TimeToPic format */
		LOGVALUE(mPerformanceCounters[k].PerformanceCounterName.c_str(),mPerformanceCounters[k].PerformanceCounterValue);

		/* reset value */
		/* Note that mutex protection is not added. In some cases result may be inaccurate if simultaneuous access
		 * to same item. For performance reasones, mutex is not added
		 */
		mPerformanceCounters[k].PerformanceCounterValue = 0;
	}

}

void PerfomanceCounter::UpdateCounterValuesToCanvas() {

	for (unsigned int k=0;k<mPerformanceCounters.size();k++) {

		if (mPerformanceCounters[k].VisualizeTextValueRect!=NULL) {

			ostringstream ss;
			ss << mPerformanceCounters[k].PerformanceCounterValue;
			ss.str();
			mPerformanceCounters[k].VisualizeTextValueRect->setText(ss.str().c_str());
		}

	}

}

int PerfomanceCounter::FindPerformanceCounter(const char *name) {

	int ret = -1;
	/* Slow linear search */
	for (unsigned int k=0;k<mPerformanceCounters.size();k++) {
		if (mPerformanceCounters[k].PerformanceCounterName==name) {
			ret = k;
			break;
		}
	}

	return ret;
}

int PerfomanceCounter::AddPerformanceCounter(const char *name) {

	int ret = FindPerformanceCounter(name);

	if (ret==-1) {
		/* Add */
		performanceCounterItem myCounter;
		myCounter.PerformanceCounterName = string(name);
		myCounter.PerformanceCounterValue = 0;
		myCounter.VisualizeTextNameRect = NULL;
		myCounter.VisualizeTextValueRect = NULL;

		mPerformanceCounters.push_back (myCounter);
		ret = mPerformanceCounters.size()-1;
	}
	else {
		/* reuse */
	}

	/* Refresh counter visualization if on */
	if (this->mbVisualizationIsOn == true) {
		VisualizeCounters(false);
		VisualizeCounters(true);
	}

	return ret;
}


bool PerfomanceCounter::IncrementPerformanceCounter(const char *performanceCounterName,int increment) {
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
	unsigned int counterId = PerformanceCounterId;
	if (counterId<mPerformanceCounters.size()) {
		mPerformanceCounters[counterId].PerformanceCounterValue+= increment;
		ret = true;
	}

	return ret;
}

bool PerfomanceCounter::SetPerformanceCounter(const char *performanceCounterName,int newValue) {
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



int PerfomanceCounter::GetPerformanceCounterId(const char *performanceCounterName) {
	int ret = 0;
	ret =  FindPerformanceCounter(performanceCounterName);
	return ret;
}

} /* namespace Gain */
