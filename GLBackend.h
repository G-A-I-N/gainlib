// GL backend selection: pulls in the right OpenGL / OpenGL ES 2.0 headers
// for the build target. Defines INIT_GL, GL_EXT_FUNC, and the inline
// checkGlError helper used by render code.
//
// Apple platforms split via <TargetConditionals.h>:
//   - TARGET_OS_IPHONE   — iOS / iPadOS / tvOS: <OpenGLES/ES2/gl.h>.
//   - else (macOS / Catalyst): desktop GL via <OpenGL/gl.h> + glext.
//
// Other backends:
//   ANDROID + Linux  — GLES2 via <GLES2/gl2.h>.
//   USE_OPENGL       — Qt/QGLFunctions integration; legacy.

#ifndef GAIN_GLBACKEND_H_
#define GAIN_GLBACKEND_H_

#include "LogMacros.h"

#ifdef __APPLE__
#include <TargetConditionals.h>
#if !TARGET_OS_IPHONE
// macOS desktop OpenGL — GLES 2.0 is a strict subset of the GL 2.1 surface
// Apple's <OpenGL/gl.h> exposes.
#define USE_DESKTOP_GL
#endif
#endif

#ifndef USE_OPENGL

#ifdef USE_DESKTOP_GL
#define GL_SILENCE_DEPRECATION 1
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#elif defined(__APPLE__) && TARGET_OS_IPHONE
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#define INIT_GL
#define GL_EXT_FUNC

#else  // USE_OPENGL — Qt path

#include <GL/gl.h>
#include <QGLFunctions>

#ifdef CORE_INCLUDE
QGLFunctions* g_qglfunctions = NULL;
#define INIT_GL initgl()

static void initgl() {
    if (g_qglfunctions == NULL) {
        g_qglfunctions = new QGLFunctions();
        g_qglfunctions->initializeGLFunctions();
    }
}
#else
extern QGLFunctions* g_qglfunctions;
#endif

#define GL_EXT_FUNC g_qglfunctions->

#endif  // USE_OPENGL

#define ALPHA_NOT_TRANSPARENT 0xff
#define ALPHA_TRANSPARENT 0x00

inline void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

#endif /* GAIN_GLBACKEND_H_ */
