#include "utils.h"

#include <TlHelp32.h>

namespace utils {
    void suspend_other_threads() {
        DWORD currentThreadId = GetCurrentThreadId();

        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        THREADENTRY32 threadEntry;
        threadEntry.dwSize = sizeof(THREADENTRY32);

        if (Thread32First(snapshot, &threadEntry)) {
            do {
                if (threadEntry.th32OwnerProcessID == GetCurrentProcessId() &&
                    threadEntry.th32ThreadID != currentThreadId) {
                    HANDLE thread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, threadEntry.th32ThreadID);
                    if (thread != NULL) {
                        SuspendThread(thread);
                        CloseHandle(thread);
                    }
                }
            } while (Thread32Next(snapshot, &threadEntry));
        }

        CloseHandle(snapshot);
	}

    void resume_other_threads() {
        DWORD currentThreadId = GetCurrentThreadId();

        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        THREADENTRY32 threadEntry;
        threadEntry.dwSize = sizeof(THREADENTRY32);

        if (Thread32First(snapshot, &threadEntry)) {
            do {
                if (threadEntry.th32OwnerProcessID == GetCurrentProcessId() &&
                    threadEntry.th32ThreadID != currentThreadId) {
                    HANDLE thread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, threadEntry.th32ThreadID);
                    if (thread != NULL) {
                        ResumeThread(thread);
                        CloseHandle(thread);
                    }
                }
            } while (Thread32Next(snapshot, &threadEntry));
        }

        CloseHandle(snapshot);
    }
}