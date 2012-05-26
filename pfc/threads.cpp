#include "pfc.h"

namespace pfc {
	t_size getOptimalWorkerThreadCount() {
#ifdef _WINDOWS
		DWORD_PTR mask,system;
		t_size ret = 0;
		GetProcessAffinityMask(GetCurrentProcess(),&mask,&system);
		for(t_size n=0;n<sizeof(mask)*8;n++) {
			if (mask & ((DWORD_PTR)1<<n)) ret++;
		}
		if (ret == 0) return 1;
		return ret;
#else
                /* This doesn't adjust for the process affinity mask,
                   unlike the win32 version above... oh well? */
                return sysconf(_SC_NPROCESSORS_ONLN);
#endif
	}

	t_size getOptimalWorkerThreadCountEx(t_size taskCountLimit) {
		if (taskCountLimit <= 1) return 1;
		return pfc::min_t(taskCountLimit,getOptimalWorkerThreadCount());
	}
}
