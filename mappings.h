// Copyright 2014
//   erkki.salonen@tpnet.fi
//   not.committed.yet
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

#ifndef MAPPINGS_H_
#define MAPPINGS_H_


#include <stdio.h>
#include <stdint.h>
#include <string>

#ifndef MAX_SCENES
#define MAX_SCENES 9
#endif

#ifdef LOGISON
#define logmessage  "now using logging"
#else
#define STORE_RELEASE
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
#endif

#ifdef __APPLE__
// macOS desktop OpenGL — GLES 2.0 is a strict subset of the GL 2.1 surface
// Apple's <OpenGL/gl.h> exposes. iOS still needs a different code path; if/when
// we target iOS, gate that on TARGET_OS_IPHONE rather than __APPLE__.
#define USE_DESKTOP_GL
#endif


///////////////////////////////////////
///OPENGL
///////////////////////////////////////

#ifndef USE_OPENGL

#ifdef USE_DESKTOP_GL
#define GL_SILENCE_DEPRECATION 1
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#elif defined(IOS)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

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
#define LOGITIMETOPIC(LOGTEXT) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "%s",LOGTEXT);
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


// Locking primitive: std::mutex on every platform.
//
// LOCK is a type alias rather than a macro so it composes with templates and
// shows up cleanly in debuggers. The ACQUIRE / RELEASE / INIT macros stay for
// callsite compatibility — internal gainlib code is migrating to RAII
// (std::lock_guard / std::scoped_lock) where the lock scope is clear.
#include <mutex>

typedef std::mutex LOCK;

// Param name `_m` deliberately mangled — `lock` would collide with the
// std::mutex::lock() method during macro substitution.
#define LOCK_INIT(_m)                ((void)0)   // std::mutex default-constructs
#define LOCK_ACQUIRE(_m)             (_m).lock()
#define LOCK_RELEASE(_m)             (_m).unlock()



#endif /* MAPPINGS_H_ */
