#include <pthread.h>
#include <stdint.h>

#if !_WINDOWS
#define INVALID_HANDLE_VALUE (pthread_t)0xDEAD
#endif

namespace pfc {
	t_size getOptimalWorkerThreadCount();
	t_size getOptimalWorkerThreadCountEx(t_size taskCountLimit);

	//! IMPORTANT: all classes derived from thread must call waitTillDone() in their destructor, to avoid object destruction during a virtual function call!
	class thread {
	public:
		PFC_DECLARE_EXCEPTION(exception_creation, exception, "Could not create thread");
                
                thread() : m_thread(INVALID_HANDLE_VALUE) {}

		~thread() {
                  PFC_ASSERT(!isActive());
                  waitTillDone();
                };
                
		void startWithPriority(int priority) {
			close();
#ifdef _WINDOWS
			HANDLE thread;
			thread = CreateThread(NULL,0,g_entry,reinterpret_cast<void*>(this),CREATE_SUSPENDED,NULL);
			if (thread == NULL) throw exception_creation();
			SetThreadPriority(thread, priority);
			ResumeThread(thread);
			m_thread = thread;
#elsif
                        int err;
			pthread_t thread;
                        pthread_attr_t attr;
                        struct sched_param sched_param = {
                        sched_priority: priority
                        };
                        
                        /* FIXME: does this mean that attr won't be
                           cleaned up? */
                        pthread_attr_init(&attr) || throw exception_creation();
                        pthread_attr_setschedparam(&attr, &sched_param);
                        
                        int err = pthread_create(&thread, NULL, g_entry, reinterpret_cast<void*>(this));
			if (err != 0) throw exception_creation();
			m_thread = thread;
#endif
		};

		void setPriority(int priority) {
			PFC_ASSERT(isActive());
#if _WINDOWS
			SetThreadPriority(m_thread, priority);
#else
                        pthread_setschedprio(m_thread, priority);
#endif
		};

		void start() {
			close();
#if _WINDOWSS
			const int priority = GetThreadPriority(GetCurrentThread());
#else
                        /* FIXME: implement this! */
                        const int priority = 0;
#endif
                        startWithPriority(priority);
		};

		bool isActive() const {
			return m_thread != INVALID_HANDLE_VALUE;
		};

		void waitTillDone() {
			close();
		};

	protected:
		virtual void threadProc() {
                  PFC_ASSERT(!"Stub thread entry - should not get here");
                };

	private:
		void close() {
			if (isActive()) {
#ifdef _WINDOWS
				WaitForSingleObject(m_thread,INFINITE);
				CloseHandle(m_thread);
#else
                                pthread_join(m_thread, NULL);
#endif
                                m_thread = INVALID_HANDLE_VALUE;
			}
		};

#if !_WINDOWS
#define CALLBACK /* nothing */
#endif

		static uint32_t CALLBACK g_entry(void* p_instance) {
			return reinterpret_cast<thread*>(p_instance)->entry();
		};

		unsigned entry() {
			try {
				threadProc();
			} catch(...) {}
			return 0;
		};

#if _WINDOWS
		HANDLE m_thread;
#else
                pthread_t m_thread;
#endif

		PFC_CLASS_NOT_COPYABLE_EX(thread)
	};
}
