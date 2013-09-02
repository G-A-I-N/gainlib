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

class Base {
public:
    Base();
	virtual ~Base();

	virtual bool setupGraphics() = 0;
	virtual void render() = 0;
    virtual void updateG(float time, float deltaTime) ;
    virtual void update(float /*time*/, float /*deltaTime*/) {};
//	virtual void mapToGraphics() = 0;

	virtual void invalidate() {program=0;};
protected:
	GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);
	GLuint loadShader(GLenum shaderType, const char* pSource);

	virtual bool initVariables() = 0;
	virtual void enableAttributes() = 0;
	virtual void disableAttributes() = 0;

	GLuint program;
};

} /* namespace Gain */
#endif /* BASE_H_ */
