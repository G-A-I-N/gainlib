/*
 * ShaderTest.h
 *
 *  Created on: Feb 2, 2013
 *      Author: villekankainen
 */

#ifndef ShaderTest_H_
#define ShaderTest_H_

#include "Rect.h"

namespace Gain {

class ShaderTest: public Gain::Rect {
public:
	ShaderTest(int x, int y, int width, int height);
	virtual ~ShaderTest();

	//Base
	virtual void updateG(float time, float timeDelta);

protected:

	virtual bool initVariables();
	virtual void enableAttributes();
	virtual void disableAttributes();


	GLint attribute_texcoord;
	GLint uniform_dist;
	GLuint vbo_square_texcoords;

	float dist;


	GLuint texture_id;
	GLint uniform_mytexture;
};

} /* namespace Gain */
#endif /* ShaderTest_H_ */
