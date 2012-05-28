#include "foobar2000.h"

void abort_callback::check() const {
	if (is_aborting()) throw exception_aborted();
}

void abort_callback::sleep(double p_timeout_seconds) const {
	if (!sleep_ex(p_timeout_seconds)) throw exception_aborted();
}

bool abort_callback::sleep_ex(double p_timeout_seconds) const {
#ifdef _WIN32
	return !win32_event::g_wait_for(get_abort_event(),p_timeout_seconds);
#else
        struct timespec ts;
        struct timespec rem;
        ts.tv_sec = floor(p_timeout_seconds);
        ts.tv_nsec = (p_timeout_seconds-ts_tv_sec) * 1e9;
        if (nanosleep(&ts, &rem) != 0) {
          return false;
        } else {
          return true;
        }
#endif
}
