/*
 * Bitmap.cpp
 *
 *  Created on: Feb 2, 2013
 *      Author: villekankainen
 */

#include "Bitmap.h"

namespace Gain {

static const char gVertexShader[] =
		"attribute vec2 coord2d;\n"
		"attribute vec2 texcoord;"
		"varying vec2 f_texcoord;"
		"uniform mat4 anim;"
		"void main() {\n"
		"  gl_Position = anim * vec4(coord2d.x, -coord2d.y, 0.0, 1.0);"
		"  f_texcoord = texcoord;"
		"}\n";

static const char gFragmentShader[] =
		"precision mediump float;\n"
		"varying vec2 f_texcoord;"
		"uniform sampler2D mytexture;"
		"uniform vec4 color;"

		"void main(void) {"
		"  vec2 flipped_texcoord = vec2(f_texcoord.x, f_texcoord.y);"
		"  gl_FragColor = texture2D(mytexture, flipped_texcoord)*color;"
		"}"
		;


Bitmap::Bitmap(float x,float y, float width, float height, const char* aVertexShader,const char* aFragmentShader) :
		super(x,y,width,height,
		aVertexShader ? aVertexShader:gVertexShader,
		aFragmentShader ? aFragmentShader:gFragmentShader)
{
	initVariables();
	// TODO Auto-generated constructor stub

}

Bitmap::Bitmap(int x,int y, int width, int height, const char* aVertexShader,const char* aFragmentShader) :
		super(x,y,width,height,
		aVertexShader ? aVertexShader:gVertexShader,
		aFragmentShader ? aFragmentShader:gFragmentShader)
{
	initVariables();
	// TODO Auto-generated constructor stub

}

Bitmap::~Bitmap() {
	// TODO Auto-generated destructor stub
}

bool Bitmap::initVariables() {
	if(!super::initVariables()) {
		return false;
	}

	const char* attribute_name;
	const char* uniform_name;

	attribute_name = "texcoord";
    attribute_texcoord = GL_EXT_FUNC glGetAttribLocation(program, attribute_name);
	if (attribute_texcoord == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}

	uniform_name = "mytexture";
    uniform_mytexture = GL_EXT_FUNC glGetUniformLocation(program, uniform_name);
	if (uniform_mytexture == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}


	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
	        GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			GL_CLAMP_TO_EDGE);

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, /*GL10.GL_REPLACE*/ GL_MODULATE);


	static uint32_t onepixelWhiteBuffer = 0xffffffff;
	glTexImage2D(GL_TEXTURE_2D, // target
			0,  // level, 0 = base, no minimap,
			GL_RGB, // internalformat
			1,  // width
			1,  // height
			0,  // border, always 0 in OpenGL ES
			GL_RGB,  // format
			GL_UNSIGNED_BYTE, // type
			&onepixelWhiteBuffer);

	GLfloat square_texcoords[] = {
			// front
			0.0, 0.0,
			1.0, 0.0,
			1.0, 1.0,
			0.0, 1.0,
	};
    GL_EXT_FUNC glGenBuffers(1, &vbo_square_texcoords);
	checkGlError("glGenBuffers");

    GL_EXT_FUNC glBindBuffer(GL_ARRAY_BUFFER, vbo_square_texcoords);
	checkGlError("glBindBuffer");

    GL_EXT_FUNC glBufferData(GL_ARRAY_BUFFER, sizeof(square_texcoords), square_texcoords, GL_STATIC_DRAW);
	checkGlError("glBufferData");

	return true;
}



void Bitmap::updateG(float time, float timeDelta) {
	super::updateG( time,  timeDelta);
}


void Bitmap::enableAttributes() {
	super::enableAttributes();

    GL_EXT_FUNC glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
    GL_EXT_FUNC glUniform1i(uniform_mytexture, 0);

    GL_EXT_FUNC glEnableVertexAttribArray(attribute_texcoord);
    GL_EXT_FUNC glBindBuffer(GL_ARRAY_BUFFER, vbo_square_texcoords);
    GL_EXT_FUNC glVertexAttribPointer(
			attribute_texcoord, // attribute
			2,                  // number of elements per vertex, here (x,y)
			GL_FLOAT,           // the type of each element
			GL_FALSE,           // take our values as-is
			0,                  // no extra data between each position
			0                   // offset of first element
	);

}

void Bitmap::disableAttributes() {
    GL_EXT_FUNC glDisableVertexAttribArray(attribute_texcoord);
    super::disableAttributes();
}


} /* namespace Gain */
