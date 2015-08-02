#include "locker.h"

int main(int argc, char** argv) {
    void (*lock_session)(void);
    if (init_locker(&lock_session) != 0) {
        return -1;
    }
    lock_session();
    return 0;
}
