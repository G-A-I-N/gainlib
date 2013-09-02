/*
 * mappings_android.h
 *
 *  Created on: Feb 2, 2013
 *      Author: villekankainen
 */

#ifndef MAPPINGS_ANDROID_H_
#define MAPPINGS_ANDROID_H_

#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#define  LOG_TAG    "libfbmjni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define  LOG_TAG_TIMETOPIC    "libfbmjni"

#define  LOGITIMETOPIC(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG_TIMETOPIC,__VA_ARGS__)

#define ALPHA_NOT_TRANSPARENT 0xff

#define LOGISON
#ifdef LOGISON
    #define LOGEVENTSTART(x) timeToPicEventStart(x);
    #define LOGEVENTSTOP(x) timeToPicEventStop(x);
	#define LOGNAMEDEVENTSTART(x,comment) timeToPicNamedEventStart(x,comment);
    #define LOGNAMEDEVENTSTOP(x) timeToPicNamedEventStop(x);
    #define LOGFREETEXT(x) LOGI("%s",x);
    #define LOGSTATE(x,y) timeToPicLogState(x,y);
    #define LOGVALUE(x,y) timeToPicValueAbs(x,y);

    //#define FUNCTIONSCOPE TimeToPicEventObj myFuncScope(__FUNCTION__);
#else
    /* Empty macros when loggin is disabled */
    #define LOGEVENTSTART(x)
    #define LOGEVENTSTOP(x)
	#define LOGNAMEDEVENTSTART(x,comment)
    #define LOGNAMEDEVENTSTOP(x)
    #define LOGFREETEXT(x)
    #define LOGSTATE(x,y)
    #define LOGVALUE(x,y)
    #define FUNCTIONSCOPE
#endif

#ifdef LOGISON
static void timeToPicLogState(const char *newState,const char *machineName)
{
	/* 1.5;state;State #67;STATE_example[<CR>]<LF> */
	LOGITIMETOPIC(";state;%s;%s\n", newState,machineName);
}

static void timeToPicValueAbs(const char *name,int myVal)
{
	/* 1.5;valueabs;7;VALUEABS_example[<CR>]<LF> */

	LOGITIMETOPIC(";valueabs;%i;%s\n", myVal,name);
}

static void timeToPicEventStart(const char *name)
{
	/* <timestamp>;event;<start/stop>;<channelname>[<CR>]<LF> */
	LOGITIMETOPIC(";event;start;%s\n", name);
}

static void timeToPicEventStop(const char *name)
{
	/* <timestamp>;event;<start/stop>;<channelname>[<CR>]<LF> */
	LOGITIMETOPIC(";event;stop;%s\n", name);
}

static void timeToPicNamedEventStart(const char *name,const char *comment)
{
	LOGITIMETOPIC(";namedevent;start;%s;%s\n", name,comment);
}

static void timeToPicNamedEventStop(const char *name)
{
	LOGITIMETOPIC(";namedevent;stop;%s\n", name);
}
#endif

static void printGLString(const char *name, GLenum s) {
	const char *v = (const char *) glGetString(s);
	LOGI("GL %s = %s\n", name, v);
}

static void checkGlError(const char* op) {
	for (GLint error = glGetError(); error; error = glGetError()) {
		//LOGI("after %s() glError (0x%x)\n", op, error);
	}
}


#endif /* MAPPINGS_ANDROID_H_ */
