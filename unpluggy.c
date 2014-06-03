#include <stdio.h>
#include <signal.h>

#include <CoreFoundation/CoreFoundation.h>

#include <IOKit/IOKitLib.h>
#include <IOKit/IOMessage.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/usb/IOUSBLib.h>

void bail(const char* msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

static IONotificationPortRef port;
static io_iterator_t iter;
static CFRunLoopRef loop;

void handler(int sig)
{
    exit(0);
}

static char* cmd = NULL;

void removed(void *rc, io_service_t svc, natural_t type, void *arg)
{
    if (type == kIOMessageServiceIsTerminated) {
        fprintf(stderr, "Device removed.\n");
        system(cmd);
    }
}

void added(void *rc, io_iterator_t it)
{
    io_service_t device;
    while ((device = IOIteratorNext(it))) {
        printf("Device added.\n");

        IOServiceAddInterestNotification(port, device, kIOGeneralInterest, removed, NULL, &iter);

        IOObjectRelease(device);
    }
}

int main(int argc, char** argv) {
    if (argc > 1) {
        cmd = argv[1];
    }

    CFMutableDictionaryRef matching;
    CFNumberRef nr;
    long vendor = 0x1050;
    long product = 0x0110;

    CFRunLoopSourceRef loopsrc;
    kern_return_t kr;

    (void)signal(SIGINT, handler);

    matching = IOServiceMatching(kIOUSBDeviceClassName);
    if (matching == NULL) {
        return -1;
    }

    nr = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &vendor);
    CFDictionarySetValue(matching, CFSTR(kUSBVendorID), nr);
    CFRelease(nr);

    nr = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &product);
    CFDictionarySetValue(matching, CFSTR(kUSBProductID), nr);
    CFRelease(nr);

    port = IONotificationPortCreate(kIOMasterPortDefault);
    loopsrc = IONotificationPortGetRunLoopSource(port);

    loop = CFRunLoopGetCurrent();
    CFRunLoopAddSource(loop, loopsrc, kCFRunLoopDefaultMode);

    kr = IOServiceAddMatchingNotification(port, kIOFirstMatchNotification, matching, added, NULL, &iter);

    added(NULL, iter);

    CFRunLoopRun();
    return 0;
}
