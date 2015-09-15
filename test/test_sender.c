#include "../rc_switch.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv) {
	void *tx = rcs_tx_init(0);
	char string[] =
                  "0000" "1000"  //0x08
                  "0001" "0000"  //0x10
                  "0010" "0000"  //0x20
                  "0100" "0000";  //0x40
	char bytes[] = { 0x01, 0x02, 0x03, 0x04};

	if(tx == NULL)
		return 0;
    
    int i = 0;
	while(i < 5) {
		sleep(5);
		printf("send frame\n");
		rcs_tx_send_bstring(tx, string);
		sleep(2);
		rcs_tx_send_bytes(tx, bytes, 4);
		i++;
	}
	
	rcs_tx_destroy(tx);

	return 0;
}
