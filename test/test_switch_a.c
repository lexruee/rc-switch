#include "../rc_switch.h"
#include <time.h>

int main() {
    
    void *s = rcs_init_a("11001", "01010");
    
    rcs_enable_transmit(s, 0);
    
    int i;
    for(i = 0; i < 4; i++) {
        if( i % 2 == 0) {
            rcs_switch_on(s);
        } else {
            rcs_switch_off(s);
        }
        sleep(2);
    }
    
    rcs_destroy(s);
    
    return 0;
}

