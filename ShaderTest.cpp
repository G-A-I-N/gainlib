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

#include "ShaderTest.h"

namespace Gain {

static const char gVertexShader[] =
		"attribute vec2 coord2d;\n"
		"uniform mat4 anim;"
		"attribute vec2 texcoord;"
		"varying vec2 f_texcoord;"
		"void main() {\n"
		"  gl_Position = anim * vec4(coord2d.x, -coord2d.y, 0.0, 1.0);"
		"  f_texcoord = texcoord;"
		"}\n";

static const char gFragmentShader[] =
		"precision mediump float;\n"
		"varying vec2 f_texcoord;"
		"uniform float dist;"


		"void main(void) {"
		"  vec2 flipped_texcoord = vec2(f_texcoord.x, f_texcoord.y);"
		"float temp = sqrt(f_texcoord.x*f_texcoord.x + f_texcoord.y*f_texcoord.y);"
		"float wave;"
		"float red=0.0,blue=0.0,green=0.0;"
		"if(temp> dist && temp<(dist+0.2) ) {"
		"	wave = (temp-dist)/( 0.25);"
		"   float sini = sin((-1.0 + (temp-dist)*12.7)+sin((f_texcoord.x+temp*0.1)*47.0));"
		"   if(sini>0.45 && sini < 0.55)"
		"		wave+=sini;"
		"   sini = sin((-1.0 + (temp-dist)*12.7)+sin((f_texcoord.x-temp*0.1)*66.0));"
		"   if(sini>0.45 && sini < 0.55)"
		"		wave+=sini;"
		"   sini = sin((-1.0 + (temp-dist)*12.7)+sin((f_texcoord.x-temp*0.05)*83.0));"
		"   if(sini>0.45 && sini < 0.55)"
		"		wave+=sini;"
		"   red+=1.0;"
		"   blue+=1.0;"
		"   green+=wave;"
		"}"
		"else {"
		"	wave = 0.0;"
		"}"
		"  gl_FragColor[0] = red>1.0?1.0:red;"
		"  gl_FragColor[1] = green>1.0?1.0:green;"
		"  gl_FragColor[2] = blue>1.0?1.0:blue;"
		"  gl_FragColor[3] = wave>1.0?0.8:wave*0.8;"

		"}"
		;


ShaderTest::ShaderTest(int x,int y, int width, int height) :
Rect(x,y,width,height, gVertexShader, gFragmentShader)
{
	initVariables();
	dist = -0.2;
	// TODO Auto-generated constructor stub

}

ShaderTest::~ShaderTest() {
	// TODO Auto-generated destructor stub
}

bool ShaderTest::initVariables() {
	if(!Rect::initVariables()) {
		return false;
	}

	const char* attribute_name;
	const char* uniform_name;

	attribute_name = "texcoord";
    attribute_texcoord = GL_EXT_FUNC glGetAttribLocation(pProgram, attribute_name);
	if (attribute_texcoord == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}

	uniform_name = "dist";
    uniform_dist = GL_EXT_FUNC glGetUniformLocation(pProgram, uniform_name);
	if (uniform_dist == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}


	GLfloat square_texcoords[] = {
			// front
			-0.50, 0.0,
			0.50, 0.0,
			0.50, 2.0,
			-0.50, 2.0,
	};
    GL_EXT_FUNC glGenBuffers(1, &vbo_square_texcoords);
    GL_EXT_FUNC glBindBuffer(GL_ARRAY_BUFFER, vbo_square_texcoords);
    GL_EXT_FUNC glBufferData(GL_ARRAY_BUFFER, sizeof(square_texcoords), square_texcoords, GL_STATIC_DRAW);
	checkGlError("glBufferData");

	return true;
}



void ShaderTest::updateG(float time, float timeDelta) {
	Rect::updateG( time, timeDelta);

	dist += 0.02;
	if(dist>2.1) dist = -0.2f;

}


void ShaderTest::enableAttributes() {
	Rect::enableAttributes();

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

    GL_EXT_FUNC glUniform1f(uniform_dist,dist);
	checkGlError("glUniform1f");

}

void ShaderTest::disableAttributes() {
    GL_EXT_FUNC glDisableVertexAttribArray(attribute_texcoord);
	Rect::disableAttributes();
}



} /* namespace Gain */
