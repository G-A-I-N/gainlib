// Logging macros used throughout gainlib internals.
//
// `Logger.h` (the class) is a separate, higher-level facility. These macros
// are the lowest layer — pre-formatted printf or Android log dispatch.
//
// Define LOGISON to silence info logs while keeping errors. STORE_RELEASE
// (set by mappings.h on stripped builds) silences both.

#ifndef GAIN_LOGMACROS_H_
#define GAIN_LOGMACROS_H_

#include <stdio.h>

#ifdef ANDROID
#include <android/log.h>
#endif

#ifdef STORE_RELEASE

#define LOGI(...)
#define LOGE(...)

#else  // STORE_RELEASE

#define LOG_TAG "libgain"

#ifdef ANDROID
#define LOGI(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);
#else
#define LOGI(...) printf(__VA_ARGS__)
#endif

#define LOGE(...) fprintf(stderr, __VA_ARGS__)

#endif  // STORE_RELEASE

#ifdef ANDROID
#define LOGITIMETOPIC(LOGTEXT) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "%s", LOGTEXT);
#else
#define LOGITIMETOPIC(...) printf(__VA_ARGS__)
#endif

#endif /* GAIN_LOGMACROS_H_ */
