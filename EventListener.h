// Copyright 2014
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

#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_

namespace Gain {

class Base;

typedef enum _EventType
{
	EVENT_NOTYPE=0,
	EVENT_ANIMATION_FINISHED,
	EVENT_BUTTON
} EventType;

class EventListener {

public:
	EventListener();
	virtual ~EventListener();

	virtual void onEvent(Base* caller, EventType = EVENT_NOTYPE )=0;
};

} /* namespace Gain */


#endif /* EVENTHANDLER_H_ */
