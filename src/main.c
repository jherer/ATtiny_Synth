#include "app/app.h"
#include "core/error_code.h"
#include "system/fault.h"

#define CHECK_FAULT

int main(void) {
    error_code_t init_err = app_init();

    #ifdef CHECK_FAULT
        if (init_err != ERROR_OK) {
            fault_panic(init_err);
        }
    #endif

    while (1) {
        error_code_t run_err = app_run();
        
        #ifdef CHECK_FAULT
            if (run_err != ERROR_OK) {
                fault_panic(run_err);
            }
        #endif
    }
    return 0;
}
