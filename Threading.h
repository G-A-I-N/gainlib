// Threading primitive used by gainlib's containers.
//
// LOCK is `std::mutex`; the LOCK_* macros stay for legacy callsite
// compatibility. New code should prefer std::lock_guard / std::scoped_lock
// directly for exception safety.

#ifndef GAIN_THREADING_H_
#define GAIN_THREADING_H_

#include <mutex>

typedef std::mutex LOCK;

// Macro parameter is `_m` (not `lock`) because the obvious name shadows
// std::mutex::lock() during preprocessor substitution and miscompiles.
#define LOCK_INIT(_m)              ((void)0)   // std::mutex default-constructs
#define LOCK_ACQUIRE(_m)           (_m).lock()
#define LOCK_RELEASE(_m)           (_m).unlock()

#endif /* GAIN_THREADING_H_ */
