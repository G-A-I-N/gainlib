// Aggregator header — kept for source-compat with code that does
// `#include "mappings.h"`. New gainlib internal code should prefer the
// focused subset headers directly:
//
//   GLBackend.h    — OpenGL / GLES2 includes, INIT_GL, GL_EXT_FUNC,
//                    checkGlError, ALPHA_* constants.
//   Threading.h    — LOCK (= std::mutex) and the LOCK_* macros.
//   LogMacros.h    — LOGI / LOGE / LOGITIMETOPIC dispatch.
//
// The original file mixed all of the above plus a pile of unused
// TARGET_LOGGER_* / STORE_RELEASE knobs and three abandoned threading
// branches; the split surfaces the load-bearing pieces.

#ifndef MAPPINGS_H_
#define MAPPINGS_H_

#include <stdio.h>
#include <stdint.h>
#include <string>

#ifndef MAX_SCENES
#define MAX_SCENES 9
#endif

#ifdef LOGISON
#define logmessage "now using logging"
#else
#define STORE_RELEASE
#endif

#include "LogMacros.h"
#include "GLBackend.h"
#include "Threading.h"

#endif /* MAPPINGS_H_ */
