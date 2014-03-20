// Copyright 2014
//   erkki.salonen@tpnet.fi
//   ville.kankainen@gmail.com
//   ville.kankainen@kangain.com
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

#ifndef GAINAPP_H_
#define GAINAPP_H_

#include "TouchInterface.h"
#include "EventListener.h"

namespace Gain {

class GainApp : public TouchInterface, EventListener {
public:
	GainApp();
	virtual ~GainApp();

	/**
	 * Initialize event for app.
	 *
	 * Gain framework calls Initialize first.
	 *
	 */
	virtual void Initialize();

	/**
	 * Touch event for when touch is pressed (down)
	 *
	 * When touch interface detects hits, it first sends TouchDown event.
	 * Then comes TouchMove if touch position is moved.
	 * When touch is released, TouchUp event is received.
	 *
	 * \param point that indicates position of touch event.
	 * \return TouchState that indicates if touch event was consumed. If yes, it is not propagated further.
	 */
	virtual TouchState TouchDown(TouchPoint* point);
	/**
	 * Touch event for when touch is moved. Before this there has been
	 * Touch Down event.
	 *
	 *
	 * \param point that indicates position of touch event.
	 * \return TouchState that indicates if touch event was consumed. If yes, it is not propagated further.
	 */
	virtual TouchState TouchMove(TouchPoint* point);
	/**
	 * Touch event for when touch is release. Before this there might have been
	 * TouchMove event. TouchDown is always before  TouchUp.
	 *
	 * \param point that indicates position of touch event.
	 * \return TouchState that indicates if touch event was consumed. If yes, it is not propagated further.
	 */
	virtual TouchState TouchUp(TouchPoint* point);

	virtual void onEvent(Base* caller, EventType type) {};

	virtual void Suspend();
	virtual void Resume();
	virtual void AboutToExit();

	virtual void Update(float sec, float deltaSec);
};

} /* namespace Gain */
#endif /* GAINAPP_H_ */
