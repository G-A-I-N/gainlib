/*
 * mappings.h
 *
 *  Created on: Feb 2, 2013
 *      Author: villekankainen
 */

#ifndef MAPPINGS_H_
#define MAPPINGS_H_

// Undefine following if wanting disable logging
#if 1
#define LOGISON
#endif


#if 0
#ifndef TARGET_LOGGER_ADDRESS
#define TARGET_LOGGER_ADDRESS "192.168.43.80"
#endif
#endif

#ifndef TARGET_LOGGER_PORT
#define TARGET_LOGGER_PORT 5000
#endif



#ifdef ANDROID
#include <android/log.h> // this is needed to make android logging working.
#define PTHREAD
#endif

#include <stdio.h>
#include <stdint.h>
#include <string>



///////////////////////////////////////
///OPENGL
///////////////////////////////////////

#ifndef USE_OPENGL

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#define INIT_GL
#define GL_EXT_FUNC

#else

#include <GL/gl.h>
#include <QGLFunctions>

#ifdef CORE_INCLUDE
QGLFunctions* g_qglfunctions = NULL;

#define INIT_GL initgl()

static void initgl() {
    if(g_qglfunctions == NULL)
    {
        g_qglfunctions = new QGLFunctions();
        g_qglfunctions->initializeGLFunctions();
    }
}

#else
extern QGLFunctions* g_qglfunctions;
#endif

#define GL_EXT_FUNC g_qglfunctions->
#endif


#define  LOG_TAG    "libgain"
#ifdef ANDROID
#define LOGI(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);
#else
#define  LOGI(...) printf(__VA_ARGS__)
#endif
#define  LOGE(...) fprintf(stderr, __VA_ARGS__)

#define  LOG_TAG_TIMETOPIC    "libgain"

#ifdef ANDROID
#define LOGITIMETOPIC(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);
#else
#define  LOGITIMETOPIC(...) printf(__VA_ARGS__)
#endif

#define ALPHA_NOT_TRANSPARENT 0xff
#define ALPHA_TRANSPARENT 0x00


#ifdef TARGET_LOGGER_ADDRESS
#include "Logger.h"
#else

#ifdef LOGISON
    #define LOGEVENTSTART(x) timeToPicEventStart(x);
    #define LOGEVENTSTOP(x) timeToPicEventStop(x);
    #define LOGNAMEDEVENTSTART(x,comment) timeToPicNamedEventStart(x,comment);
    #define LOGNAMEDEVENTSTOP(x) timeToPicNamedEventStop(x);
    #define LOGFREETEXT(x) LOGI("%s",x);
    #define LOGSTATE(x,y) timeToPicLogState(x,y);
    #define LOGVALUE(x,y) timeToPicValueAbs(x,y);

	#define LOGSCOPE scopeTrace myScope(__PRETTY_FUNCTION__);
	#define LOGSCOPEINFO(param) scopeTrace myScope(__FUNCTION__ + param);

#else
    /* Empty macros when loggin is disabled */
    #define LOGEVENTSTART(x)
    #define LOGEVENTSTOP(x)
    #define LOGNAMEDEVENTSTART(x,comment)
    #define LOGNAMEDEVENTSTOP(x)
    #define LOGFREETEXT(x)
    #define LOGSTATE(x,y)
    #define LOGVALUE(x,y)
    #define LOGSCOPE
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

class scopeTrace {
public:
	inline scopeTrace(const char *scopeName) {
		scopeText = scopeName;
		LOGEVENTSTART(scopeText.c_str());
	}
	inline ~scopeTrace() {
		LOGEVENTSTOP(scopeText.c_str());
	}
private:
	std::string scopeText;
};

#endif

#endif //TARGET_LOGGER_ADDRESS

inline void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}


//THREAD pthread

//#define PTHREAD
#ifdef PTHREAD

#include <pthread.h>

#define LOCK pthread_mutex_t
#define LOCK_INIT( lock ) pthread_mutex_init( &lock , NULL)
#define LOCK_ACQUIRE( lock ) pthread_mutex_lock( &lock )
#define LOCK_RELEASE( lock ) pthread_mutex_unlock( &lock )
//#define LOCK_ACQUIRE( lock ) {LOGEVENT2START("L",__func__); pthread_mutex_lock( &lock );}
//#define LOCK_RELEASE( lock ) {pthread_mutex_unlock( &lock ); LOGEVENT2STOP("L",__func__);}

#elif QTHREAD

#define LOCK int
#define LOCK_INIT(lock)
#define LOCK_ACQUIRE(lock)
#define LOCK_RELEASE(lock)

#else

#define LOCK volatile int
#define LOCK_INIT(lock) {(lock) = 0;}
#define LOCK_ACQUIRE(lock) {while((lock)==1) { } (lock) = 1;}
#define LOCK_RELEASE(lock) {(lock) = 0;}

#endif



#endif /* MAPPINGS_H_ */
