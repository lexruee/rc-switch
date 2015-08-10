#include "../rc_switch.h"
#include <time.h>

int main() {
    
    void *s = rcs_switch_init_b(0, 1, 2);
    
    int i;
    for(i = 0; i < 4; i++) {
        if( i % 2 == 0) {
            rcs_switch_on(s);
        } else {
            rcs_switch_off(s);
        }
        sleep(2);
    }
    
    rcs_switch_destroy(s);
    
    return 0;
}

