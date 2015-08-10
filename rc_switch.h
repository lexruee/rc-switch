
/**
 * RCSwitch intetface for switch types A, B, C, D.
 */

void *rcs_init_a(int transmitter_pin, char *s_group, char *s_device);
void *rcs_init_b(int transmitter_pin, int n_address, int n_channel);
void *rcs_init_c(int transmitter_pin, char c_family, int n_group, int n_device);
void rcs_switch_on(void *_s);
void rcs_switch_off(void *_s);
void rcs_destroy(void *_s);
