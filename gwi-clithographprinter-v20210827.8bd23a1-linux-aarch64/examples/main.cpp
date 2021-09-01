#include "CLithographPrinter.h"

#ifdef STATIC_EXAMPLES
#  include "Evolis_Avansia_LithographPrinter.h"
#  include "Evolis_Elypso_LithographPrinter.h"
#endif

#ifdef _WIN32
#  include "win32/dlfcn.h"
#else
#  include <dlfcn.h>
#  define WINAPI
#endif

#include <cstdio>
#include <cstring>

typedef LPVOID (*fnLithographCreateInstance)(LPVOID lpReserve);
typedef VOID (*fnFreeInstance)(LPVOID p);

void example(CLithographPrinter* lp, const char* type, const char* device);

static void usage(const char* command)
{
#ifdef STATIC_EXAMPLES
#  define FIRST_ARG_NAME "LIBRARY"
#  define FIRST_ARG_DESC "  " FIRST_ARG_NAME " is the name of the lithography interface to use.\n"
#else
#  define FIRST_ARG_NAME "LIBRARY_PATH"
#  define FIRST_ARG_DESC \
    "  LIBRARY_PATH is the path to a library implementing the\n" \
    "  LithographPrinter interface. The library will be loaded\n" \
    "  and used to communicate with the printer.\n"
#endif
    printf("usage: %s " FIRST_ARG_NAME " [PORT] [PORT_PARAM]\n", command);
    printf("\n");
    printf(FIRST_ARG_DESC);
    printf("\n");
    printf("  PORT will be passed to Print_Open() (first argument).\n");
    printf("  Optional, default value is \"USB\".\n");
    printf("\n");
    printf("  PORT_PARAM will be passed to Print_Open() (second argument).\n");
    printf("  Optional, default value is \"/dev/usb/lp0\".\n");
    printf("\n");
    printf("  Examples:\n");
    printf("    - %s libEvolis_Avansia_LithographPrinter.so\n", command);
    printf("    - %s libEvolis_Avansia_LithographPrinter.so USB /dev/usb/lp0\n", command);
}

static void delete_lithograph_printer(void* handle, CLithographPrinter* printer)
{
#ifdef STATIC_EXAMPLES
    delete printer;
#else
    if (handle) {
        if (printer) {
            void* FreeInstance;

            FreeInstance = dlsym(handle, "FreeInstance");
            if (FreeInstance)
                ((fnFreeInstance) FreeInstance)(printer);
        }
        dlclose(handle);
    }
#endif
}

static CLithographPrinter* new_lithograph_printer(void** handle, const char* path)
{
#ifdef STATIC_EXAMPLES
    CLithographPrinter* lp = nullptr;

    if (!strcmp(path, "Evolis_Avansia_LithographPrinter")) {
        lp = new Evolis_Avansia_LithographPrinter();
    } else if (!strcmp(path, "Evolis_Elypso_LithographPrinter")) {
        lp = new Evolis_Elypso_LithographPrinter();
    }

    if (lp != nullptr) {
        LPVOID reply;
        char   ec[5];

        // This will dump log messages to the file pointed by "path".
        const char* path = "/tmp/printer.log";
        lp->Print_ExtraCommand(0, (char*) "setLogPath", (char*) path, reply, ec);
        printf("ec=%s reply=%s\n", ec, (char*) reply);
    }
    return lp;
#else
    void* CreateInstance;
    CLithographPrinter* lp;

    *handle = dlopen(path, RTLD_NOW);
    if (*handle) {
        CreateInstance = dlsym(*handle, "CreateInstance");
        if (CreateInstance) {
            lp = static_cast<CLithographPrinter*>(
                ((fnLithographCreateInstance) CreateInstance)(nullptr)
            );

            if (lp != nullptr) {
                LPVOID reply;
                char   ec[5];

                // This will dump log messages to the file pointed by "path".
                const char* path = "/tmp/printer.log";
                lp->Print_ExtraCommand(0, (char*) "setLogPath", (char*) path, reply, ec);
                printf("ec=%s reply=%s\n", ec, (char*) reply);
                return lp;
            }
        }
        delete_lithograph_printer(*handle, nullptr);
    }
    return nullptr;
#endif
}

int main(int argc, char* argv[])
{
    void*               handle;
    CLithographPrinter* lithographPrinter;
    const char*         library = nullptr;
    const char*         type    = "USB";
    const char*         device  = "/dev/usb/lp0";

    if (argc < 2) {
        usage(argv[0]);
        return 1;
    } else {
        library = argv[1];
        if (argc > 2)
            type = argv[2];
        if (argc > 3)
            device = argv[3];
    }

    lithographPrinter = new_lithograph_printer(&handle, library);
    if (lithographPrinter) {
        example(lithographPrinter, type, device);
        delete_lithograph_printer(handle, lithographPrinter);
        return 0;
    } else {
        printf("error: Can't instantiate a CLithographPrinter:\n");
        printf("       %s\n", dlerror());
        return 1;
    }
}
