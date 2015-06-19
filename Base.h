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

#define FLAG_EMPTY             ( 1 << 0)

#define FLAG_DIRTY_ROTATION    ( 1 << 1)
#define FLAG_DIRTY_TRANSLATION ( 1 << 2)
#define FLAG_DIRTY_INVERSE     ( 1 << 3)
#define FLAG_DIRTY_PIVOT       ( 1 << 4)
#define FLAG_DIRTY_VERTICES    ( 1 << 5)
#define FLAG_DIRTY_ZORDER      ( 1 << 6)
#define FLAG_DIRTY_COLOR       ( 1 << 7)
#define FLAG_DIRTY_ANIM        ( 1 << 8)

#define FLAG_GLOBAL_ANIM     ( 1 << 16)

#define FLAG_FEATURE_TOUCH_INTERFACE ( 1 << 24)

namespace Gain {

class Base;

typedef enum _BaseState {
	NOT_INITIALIZED=0,
	INITIALIZED
} BaseState;

typedef enum _PositionIndex
{
	POS_X=0,
	POS_Y,
	POS_SIZE
} PositionIndex;


typedef enum _colorIndex {
	COLOR_RED=0,
	COLOR_GREEN,
	COLOR_BLUE,
	COLOR_APLHA,
	COLOR_SIZE
} ColorIndex;

typedef enum _AnimationType
{
	ANIM_NONE=0,
	ANIM_MOVE,
	ANIM_COLOR,
	ANIM_FADE,
	ANIM_RECT_SIZE
} AnimationType;

typedef struct _AnimationContainer
{
public:
	AnimationType type;

	float elapsedTime, time;

	float startX,startY;
	float targetX, targetY;

	float startColor[COLOR_SIZE];
	float targetColor[COLOR_SIZE];
	//Animation
} AnimationContainer;

class Base : public EventListener, public TouchInterface {
public:
    Base();
	virtual ~Base();

	// needs to call setReady() when ready
	virtual bool setupGraphics() = 0;
	virtual void render() const = 0;
    virtual void updateG(float sec, float deltaSec) ;
    virtual void update(float sec, float deltaSec) {};

    //EventListener
    virtual void onEvent(Base* aSource, EventType aType) {};

	virtual void invalidate() {program=0;pState = NOT_INITIALIZED;};
	inline BaseState getState() const { return pState; } ;

	virtual Base* addEventListener(EventListener* aListener);
	virtual void triggerEvent(EventType type);

    inline virtual Base* setZOrder(int aZOrder) {pZOrder = aZOrder; flags |= FLAG_DIRTY_ZORDER; return this;}
    

	inline virtual TouchState TouchDown(TouchPoint* point) { return TOUCH_NOT_CONSUMED; }
	inline virtual TouchState TouchMove(TouchPoint* point) { return TOUCH_NOT_CONSUMED; }
	inline virtual TouchState TouchUp(TouchPoint* point) { return TOUCH_NOT_CONSUMED; }

	Base* setXN(float x);
	Base* setYN(float y);

	float getXN();
	float getYN();

	Base* setPositionN(float x,float y);
	Base* setRotation(GLfloat angle);
	Base* setPivot(float x, float y);

	Base* setColor(GLfloat color[4]);
	Base* setColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	Base* setAlpha(GLfloat alpha);

	/**
	 * Moves target to given coordinates in given time
	 *
	 * Uses Base's animation framework to move item to specified position in given time.
	 * Is parallel to all other animation that are running.
	 *
	 * Coordinates are given in normalized coordinates.
	 *
	 * \param targetX of target x-coordinate in normalized form.
	 * \param targetY of target y-coordinate in normalized form.
	 * \param sec time (seconds) to spend transition from current pos to new position.
	 */
	virtual Base* toPositionN(float targetX, float targetY, float sec);

	/**
	 * Fades to target alpha in given time
	 *
	 * Uses Base's animation framework to fade the alpha value to given level in given time.
	 * Is parallel to all other animation that are running.
	 *
	 * \param aTargetAlpha to fade to
	 * \param sec time (seconds) to spend fading
	 * \return pointer to this object
	 */
	virtual Base* toAlphaN(float aTargetAplha, float sec);

	/**
	 * Fades to target color in given time
	 *
	 * Uses Base's animation framework to fade the color value to given level in given time.
	 * Is parallel to all other animation that are running.
	 *
	 * \param r red value to fade to
	 * \param g green value to fade to
	 * \param b blue value to fade to
	 * \param aTargetAlpha to fade to
	 * \param sec time to spend fading
	 * \return pointer to this object
	 */
	virtual Base* toColorN(float r, float g, float b, float aTargetAplha, float sec);

	void setGlobalAnim(glm::mat4 &aGlobalAnim);
	void setGlobalColor(float aColor[COLOR_SIZE]);
	void cancelAllAnimations();

protected:
	GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);
	GLuint loadShader(GLenum shaderType, const char* pSource);

	virtual bool initVariables() = 0;
	virtual void enableAttributes() const = 0;
	virtual void disableAttributes() const = 0;

	virtual void setReady() { pState = INITIALIZED; };
	void setTouchable() { flags |= FLAG_FEATURE_TOUCH_INTERFACE; }

	virtual void updateAnimationPart(float currentPosition, AnimationContainer* anim);
	void updateAnimation(float sec, float deltaSec);

protected:
	std::set<EventListener*> pEventListener;



	inline glm::mat4 getInverseMat()
	{
		if(flags & FLAG_DIRTY_INVERSE)
		{
			priInverse = glm::inverse(anim);
			flags ^= FLAG_DIRTY_INVERSE;
		}
		return priInverse;
	}

	inline glm::vec4 getTranslatedPos(float Xn, float Yn)
	{
		glm::vec4 pos = glm::vec4(Xn, -Yn/CORE->reversed_ratio, 0, 1);
		glm::mat4 inverse = getInverseMat();
		glm::vec4 translated_pos = inverse * pos;
		translated_pos.y *=-1.f;
		return translated_pos;
	}


private:
	glm::mat4 priInverse;

public:
	glm::mat4 anim;
	glm::mat4 globalAnim;
	int flags;
	GLfloat color[COLOR_SIZE];
	GLfloat globalColor[COLOR_SIZE];

protected:
	glm::mat4 pAnim;
	GLfloat pColor[COLOR_SIZE];

	float pPivot[POS_SIZE];

	float pPositionX, pPositionY;

	float pAngle;

	int pZOrder;
    
	BaseState pState;
	GLuint program;

	std::set<AnimationContainer*> pAnimationList;

	friend struct BaseCompare;
};
    
struct BaseCompare {
    bool operator()(const Base* aLeft, const Base* aRigth) const {
        return (aLeft->pZOrder < aRigth->pZOrder) ||
               ((aLeft->pZOrder == aRigth->pZOrder) && (aLeft < aRigth));
    }
};

} /* namespace Gain */
#endif /* BASE_H_ */
