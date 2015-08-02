#include <stdlib.h>
#include <dlfcn.h>

#include "locker.h"

int init_locker(lock_func *func) {
    void* handle;

    handle = dlopen("/System/Library/CoreServices/Menu Extras/User.menu/Contents/MacOS/User", RTLD_LAZY);
    if (handle == NULL) {
        return -1;
    }

    *func = dlsym(handle, "SACSwitchToLoginWindow");
    if (*func == NULL) {
        return -1;
    }

    return 0;
}
