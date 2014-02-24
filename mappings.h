/*
 * mappings.h
 *
 *  Created on: Feb 2, 2013
 *      Author: villekankainen
 */

#ifndef MAPPINGS_H_
#define MAPPINGS_H_

#include <stdio.h>
#include <stdint.h>
#include <string>


//#define STORE_RELEASE

// Undefine following if wanting disable logging
#if 0
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

#ifdef STORE_RELEASE

#define LOGI(...)
#define LOGE(...)

#else //STORE_RELEASE
#define  LOG_TAG    "libgain"
#ifdef ANDROID
#define LOGI(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);
#else
#define  LOGI(...) printf(__VA_ARGS__)
#endif
#define  LOGE(...) fprintf(stderr, __VA_ARGS__)
#define  LOG_TAG_TIMETOPIC    "libgain"
#endif //STORE_RELEASE

#ifdef ANDROID
#define LOGITIMETOPIC(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);
#else
#define  LOGITIMETOPIC(...) printf(__VA_ARGS__)
#endif

#define ALPHA_NOT_TRANSPARENT 0xff
#define ALPHA_TRANSPARENT 0x00

inline void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}


//THREAD pthread

#ifdef PTHREAD

#include <pthread.h>

#define LOCK pthread_mutex_t
#define LOCK_INIT( lock ) pthread_mutex_init( &lock , NULL)
#define LOCK_ACQUIRE( lock ) pthread_mutex_lock( &lock )
#define LOCK_RELEASE( lock ) pthread_mutex_unlock( &lock )

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
