/*
 * Base.h
 *
 *  Created on: Feb 2, 2013
 *      Author: villekankainen
 */

#ifndef BASE_H_
#define BASE_H_

#include "Core.h"

namespace Gain {

typedef enum _BaseState {
	NOT_INITIALIZED=0,
	INITIALIZED
} BaseState;

class Base {
public:
    Base();
	virtual ~Base();

	// needs to call etReady() when ready
	virtual bool setupGraphics() = 0;
	virtual void render() const = 0;
    virtual void updateG(float sec, float deltaSec) ;
    virtual void update(float sec, float deltaSec) {};

	virtual void animationFinishedCallback(Base* aListener) {};
	virtual void invalidate() {program=0;pState = NOT_INITIALIZED;};
	virtual BaseState getState() { return pState; };

protected:
	GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);
	GLuint loadShader(GLenum shaderType, const char* pSource);

	virtual bool initVariables() = 0;
	virtual void enableAttributes() const = 0;
	virtual void disableAttributes() const = 0;

	virtual void setReady() { pState = INITIALIZED; };

	BaseState pState;
	GLuint program;
};

} /* namespace Gain */
#endif /* BASE_H_ */
