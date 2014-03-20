// Copyright 2014
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

#ifndef BASE_H_
#define BASE_H_

#include "Core.h"
#include "EventListener.h"
#include <map>

namespace Gain {

class Base;

typedef enum _BaseState {
	NOT_INITIALIZED=0,
	INITIALIZED
} BaseState;

class Base : public EventListener {
public:
    Base();
	virtual ~Base();

	// needs to call etReady() when ready
	virtual bool setupGraphics() = 0;
	virtual void render() const = 0;
    virtual void updateG(float sec, float deltaSec) ;
    virtual void update(float sec, float deltaSec) {};

    //EventListener
    virtual void onEvent(Base* aListener, EventType aType) {};

	virtual void invalidate() {program=0;pState = NOT_INITIALIZED;};
	virtual BaseState getState() { return pState; };

	virtual void addEventListener(EventListener* aListener);
	virtual void triggerEvent(EventType type);

protected:
	GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);
	GLuint loadShader(GLenum shaderType, const char* pSource);

	virtual bool initVariables() = 0;
	virtual void enableAttributes() const = 0;
	virtual void disableAttributes() const = 0;

	virtual void setReady() { pState = INITIALIZED; };

	std::map<EventListener*,EventListener*> pEventListener;

	BaseState pState;
	GLuint program;
};

} /* namespace Gain */
#endif /* BASE_H_ */
