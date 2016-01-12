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
	flags(0),
	pAngle(0),
	pPositionX(0),
	pPositionY(0),
    pProgram(0),
    pState(NOT_INITIALIZED),
    pZOrder(0)
{
	for(int n=0;n<COLOR_SIZE;++n)
	{
		pColor[n] = 1.f;
		color[n] = 1.f;
		globalColor[n] = 1.f;
	}
}

Base::~Base()
{
}

void Base::updateG(float /*time*/, float /*deltaTime*/)
{
	if(flags & FLAG_DIRTY_ANIM)
	{
		flags ^= FLAG_DIRTY_ANIM;
		if(flags & FLAG_GLOBAL_ANIM)
		{
			anim = globalAnim * pAnim;
		} else	{
			anim = pAnim;
		}
	}

	if(flags & FLAG_DIRTY_COLOR)
	{
		flags ^= FLAG_DIRTY_COLOR;
		color[COLOR_RED]   = pColor[COLOR_RED] * globalColor[COLOR_RED];
		color[COLOR_GREEN] = pColor[COLOR_GREEN] * globalColor[COLOR_GREEN];
		color[COLOR_BLUE]  = pColor[COLOR_BLUE] * globalColor[COLOR_BLUE];
		color[COLOR_APLHA] = pColor[COLOR_APLHA] * globalColor[COLOR_APLHA];
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
	pEventListener.insert(aListener);
	return this;
}

void Base::triggerEvent(EventType aEventType)
{
	std::set<EventListener*>::iterator it;
	for(it = pEventListener.begin();it != pEventListener.end();it++)
	{
		(*it)->onEvent(this, aEventType);
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
	flags |= FLAG_DIRTY_COLOR;
	memcpy(pColor,aColor, sizeof(color));
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
	flags |= FLAG_DIRTY_COLOR;
	pColor[COLOR_APLHA] = alpha;
	return this;
}

void Base::setGlobalAnim(glm::mat4 &aGlobalAnim)
{
	flags |= (FLAG_GLOBAL_ANIM | FLAG_DIRTY_ANIM);
	globalAnim = aGlobalAnim;
}

void Base::setGlobalColor(float aColor[COLOR_SIZE])
{
	flags |= FLAG_DIRTY_COLOR;
	memcpy(globalColor,aColor, sizeof(globalColor));
}

Base* Base::toPositionN(float aTargetX, float aTargetY, float sec)
{

	AnimationContainer* anim = new AnimationContainer();
	anim->targetX = aTargetX;
	anim->targetY = aTargetY;
	anim->startX = getXN();
	anim->startY = getYN();
	anim->type = ANIM_MOVE;

	anim->time = sec;

	pAnimationList.insert(anim);
	return this;
}


Base* Base::toAlphaN(float aTargetAplha, float sec)
{

	AnimationContainer* anim = new AnimationContainer();
	memcpy(anim->startColor,pColor,sizeof(pColor));
	memcpy(anim->targetColor,pColor,sizeof(pColor));
	anim->targetColor[COLOR_APLHA] = aTargetAplha;
	anim->type = ANIM_FADE;

	anim->time = sec;

	pAnimationList.insert(anim);
	return this;
}

Base* Base::toColorN(float aRed, float aGreen, float aBlue, float aTargetAplha, float sec)
{
	AnimationContainer* anim = new AnimationContainer();
	memcpy(anim->startColor,pColor,sizeof(pColor));
	anim->targetColor[COLOR_APLHA] = aTargetAplha;
	anim->targetColor[COLOR_RED] = aRed;
	anim->targetColor[COLOR_GREEN] = aGreen;
	anim->targetColor[COLOR_BLUE] = aBlue;
	anim->type = ANIM_COLOR;

	anim->time = sec;

	pAnimationList.insert(anim);
	return this;
}

void Base::updateAnimationPart(float currentPosition, AnimationContainer* anim) {
    switch(anim->type) {
		case ANIM_MOVE:
			{
				if(anim->startX != anim->targetX){
					setXN(anim->startX + (anim->targetX - anim->startX)*currentPosition);
				}
				if(anim->startY != anim->targetY){
					setYN(anim->startY + (anim->targetY - anim->startY)*currentPosition);
				}
				break;
			}
		case ANIM_COLOR:
			{
				float color[COLOR_SIZE];
				for(unsigned int color_i=0;color_i<COLOR_SIZE;++color_i)
				{
					color[color_i] = anim->startColor[color_i] + (anim->targetColor[color_i] - anim->startColor[color_i])*currentPosition;
				}
				setColor(color);
				break;
			}
		case ANIM_FADE:
			{
				float alpha = anim->startColor[COLOR_APLHA] + (anim->targetColor[COLOR_APLHA] - anim->startColor[COLOR_APLHA])*currentPosition;
				setAlpha(alpha);
				break;
			}
		// no implementation here
		case ANIM_NONE:
		case ANIM_RECT_SIZE:
			break;
    }
}

void Base::updateAnimation(float /*sec*/, float deltaSec)
{
	std::set<AnimationContainer*>::iterator it = pAnimationList.begin();
	while(it!=pAnimationList.end())
	{
		AnimationContainer* anim = *it;
	    std::set<AnimationContainer*>::iterator delete_it = it;
	    it++;

	    anim->elapsedTime += deltaSec;

	    float currentPosition = std::min(1.f, anim->elapsedTime / anim->time);

	    updateAnimationPart(currentPosition, anim);

	    if(currentPosition == 1.f)
	    {
		    triggerEvent(EVENT_ANIMATION_FINISHED);
		    pAnimationList.erase(delete_it);
	    	delete anim;
	    	anim=0;
	    }
	}
}

void Base::cancelAllAnimations()
{
	std::set<AnimationContainer*>::iterator it = pAnimationList.begin();
	while(it!=pAnimationList.end())
	{
		AnimationContainer* anim = *it;
		it++;
	    triggerEvent(EVENT_ANIMATION_FINISHED_BY_CANCEL);
    	delete anim;
    	anim=0;
    }
	pAnimationList.clear();
}

} /* namespace Gain */
