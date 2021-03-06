// Copyright 2014
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

#ifndef TOUCHINTERFACE_H_
#define TOUCHINTERFACE_H_

namespace Gain {

enum TouchState {
	TOUCH_CONSUMED=0,
	TOUCH_NOT_CONSUMED
};


typedef struct _TouchPoint{
	float X;
	float Y;
	//normalised
	float Xn;
	//normalised
	float Yn;
	int ID;

} TouchPoint;

typedef enum _TouchType{
	TOUCH_DOWN=1,
	TOUCH_MOVE,
	TOUCH_UP
} TouchType;

class TouchInterface {
public:
	TouchInterface();
	virtual ~TouchInterface();

	virtual TouchState TouchDown(TouchPoint* point)=0;
	virtual TouchState TouchMove(TouchPoint* point)=0;
	virtual TouchState TouchUp(TouchPoint* point)=0;

	inline virtual TouchState offerTouch(TouchPoint* aTouchPoint, TouchType aType)
	{
		switch (aType) {
			case TOUCH_DOWN:
				return this->TouchDown(aTouchPoint);
			case TOUCH_MOVE:
				return TouchMove(aTouchPoint);
			case TOUCH_UP:
				return TouchUp(aTouchPoint);
		}
		return TOUCH_NOT_CONSUMED;
	}
};

} /* namespace Gain */
#endif /* TOUCHINTERFACE_H_ */
