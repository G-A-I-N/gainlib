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

#include "Base.h"

namespace Gain {

Base::Base() :
    pState(NOT_INITIALIZED),
    program(0),
    pZOrder(0),
	pAngle(0),
	pPositionX(0),
	pPositionY(0),
	flags(0)
{
}

Base::~Base()
{
}

void Base::updateG(float /*time*/, float /*deltaTime*/)
{
	if(flags & FLAG_FEATURE_GLOBAL_ANIM)
	{
		anim = globalAnim * pAnim;
	} else
	{
		anim = pAnim;
	}
}


GLuint Base::createProgram(const char* pVertexSource, const char* pFragmentSource) {
	static const char* lastLoaded_vertex;
	static const char* lastLoaded_fragment;
	static GLuint lastLoaded_program;

	if (pVertexSource == lastLoaded_vertex &&
		pFragmentSource == lastLoaded_fragment)
	{

		return lastLoaded_program;
	}



	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
	if (!vertexShader) {
		return 0;
	}

	GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
	if (!pixelShader) {
		return 0;
	}

    GLuint program = GL_EXT_FUNC glCreateProgram();
	if (program) {
        GL_EXT_FUNC glAttachShader(program, vertexShader);
		checkGlError("glAttachVShader");
        GL_EXT_FUNC glAttachShader(program, pixelShader);
		checkGlError("glAttachFShader");
        GL_EXT_FUNC glLinkProgram(program);
		GLint linkStatus = GL_FALSE;
        GL_EXT_FUNC glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE) {
			GLint bufLength = 0;
            GL_EXT_FUNC glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
			if (bufLength) {
				char* buf = (char*) malloc(bufLength);
				if (buf) {
                    GL_EXT_FUNC glGetProgramInfoLog(program, bufLength, NULL, buf);
					LOGE("Could not link program:\n%s\n", buf);
					free(buf);
				}
			}
            GL_EXT_FUNC glDeleteProgram(program);
			program = 0;
		}
	}
	lastLoaded_vertex = pVertexSource;
	lastLoaded_fragment = pFragmentSource;

	lastLoaded_program = program;
	return program;
}


GLuint Base::loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = GL_EXT_FUNC glCreateShader(shaderType);
	if (shader) {
        GL_EXT_FUNC glShaderSource(shader, 1, &pSource, NULL);
        GL_EXT_FUNC glCompileShader(shader);
		GLint compiled = 0;
        GL_EXT_FUNC glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			GLint infoLen = 0;
            GL_EXT_FUNC glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen) {
				char* buf = (char*) malloc(infoLen);
				if (buf) {
                    GL_EXT_FUNC glGetShaderInfoLog(shader, infoLen, NULL, buf);
					LOGE("Could not compile shader %d:\n%s\n",
							shaderType, buf);
					free(buf);
				}
                GL_EXT_FUNC glDeleteShader(shader);
				shader = 0;
			}
		}
	}
	return shader;
}

Base* Base::addEventListener(EventListener* aListener)
{
	pEventListener.insert(std::pair<EventListener*,EventListener*>(aListener,aListener));
	return this;
}

void Base::triggerEvent(EventType aEventType)
{
	std::map<EventListener*,EventListener*>::iterator it;
	for(it = pEventListener.begin();it != pEventListener.end();it++)
	{
		it->first->onEvent(this, aEventType);
	}
}

Base* Base::setXN(float x)
{
	pPositionX = x;
	flags |= FLAG_DIRTY_TRANSLATION;
	return this;
}

Base* Base::setYN(float y)
{
	pPositionY = -y*CORE->ratio;
	flags |= FLAG_DIRTY_TRANSLATION;
	return this;
}

float Base::getXN()
{
	return pPositionX;
}

float Base::getYN()
{
	return -pPositionY*CORE->reversed_ratio;
}

Base* Base::setPositionN(float x, float y)
{
	setXN(x);
	setYN(y);
	return this;
}

Base* Base::setRotation(float aAngle)
{
	pAngle = aAngle;
	flags |= FLAG_DIRTY_ROTATION;
	return this;
}

Base* Base::setPivot(float x, float y)
{
	pPivot[POS_X] = x;
	pPivot[POS_Y] = y;
	flags |= FLAG_DIRTY_PIVOT;
	return this;
}

Base* Base::setColor(GLfloat aColor[COLOR_SIZE])
{
	memcpy(color,aColor, sizeof(color));
	return this;
}

Base* Base::setColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	GLfloat color[COLOR_SIZE] = {red, green, blue, alpha};
	setColor(color);
	return this;
}

Base* Base::setAlpha(GLfloat alpha)
{
	color[COLOR_APLHA] = alpha;
	return this;
}

void Base::setGlobalAnim(glm::mat4 &aGlobalAnim)
{
	flags |= FLAG_FEATURE_GLOBAL_ANIM;
	globalAnim = aGlobalAnim;
}

} /* namespace Gain */
