#include "rc_switch.h"
#include "wiringPi.h"
#include <stdlib.h>


#define FALSE 0
#define TRUE 1


/*
 * Private structures.
 */
 
 
typedef struct {
    unsigned int protocol;
    unsigned int delay;
    unsigned int bit_length;
    unsigned long received_value;
    int tolerance;
    int pin;
} _Rx;


typedef struct {
    int pin;
    int pulse_length;
    int repeat_transmit;
    char protocol;
} _Tx;


typedef struct {
    char c_group;
    char **s_group;
    char c_device;
    char **s_device;
    char c_family;
    int n_group;
    int n_device;
    int n_address;
    int n_channel;
    int b_status;
} _Params;


typedef struct {
    void *tx;
    void *rx;
    char type;
    char *(*get_codeword)(_Params params, int status);
    _Params params;
} _RCSwitch;


/*
 * Private variables.
 */

static int RCS_WIRING_PI_INIT = FALSE;


/*
 * Private function prototypes.
 */

void *_rcs_tx_init(int pin);
void _rcs_tx_destroy(void *_tx);
void _rcs_tx_set_pulse_length(void *_tx, int pulse_length);
void _rcs_tx_set_repeat_transmit(void *_tx, int repeat_transmit);
void _rcs_tx_set_protocol(void *_tx, int protocol);
void _rcs_tx_sendTriState(void *_tx, char* sCodeWord);
void _rcs_tx_sendSync(void *_tx);
void _rcs_tx_send0(void *_tx);
void _rcs_tx_send1(void *_tx);
void _rcs_tx_sendT1(void *_tx);
void _rcs_tx_sendT0(void *_tx);
void _rcs_tx_sendTF(void *_tx);


char *_rcs_get_codewordA(_Params params, int status);
char *_rcs_get_codewordB(_Params params, int status);
char *_rcs_get_codewordC(_Params params, int status);
char *_rcs_get_codewordD(_Params params, int status);


char *_rcs_dec2binWzerofill(unsigned long dec, unsigned int bit_length);
char *_rcs_dec2binWcharfill(unsigned long dec, unsigned int bit_length, char fill);

void _rcs_init_wiring_pi();



/*
 * Private functions.
 */


void _rcs_init_wiring_pi() {
    if(RCS_WIRING_PI_INIT == FALSE) {
        wiringPiSetup();
        RCS_WIRING_PI_INIT = TRUE;
    }
}


void *rcs_switch_init_a(int transmitter_pin, char *s_group, char *s_device) {
    _RCSwitch *s = malloc(sizeof(_RCSwitch));
    void *tx = _rcs_tx_init(transmitter_pin);
    _rcs_tx_set_pulse_length(tx, 650);
    _rcs_tx_set_repeat_transmit(tx, 10);
    _rcs_tx_set_protocol(tx, 2);
    s->tx = tx;
    s->type = 'A';
    s->get_codeword = &_rcs_get_codewordA;
    s->params.s_group = &s_group;
    s->params.s_device = &s_device;
    return s;
}


void *rcs_switch_init_b(int transmitter_pin, int n_address, int n_channel) {
    _RCSwitch *s = malloc(sizeof(_RCSwitch));
    void *tx = _rcs_tx_init(transmitter_pin);
    _rcs_tx_set_pulse_length(tx, 350);
    _rcs_tx_set_repeat_transmit(tx, 10);
    _rcs_tx_set_protocol(tx, 1);
    s->tx = tx;
    s->type = 'B';
    s->get_codeword = &_rcs_get_codewordB;
    s->params.n_address = n_address;
    s->params.n_channel = n_channel;
    return s;
}


void *rcs_switch_init_c(int transmitter_pin, char c_family, int n_group, int n_device) {
    _RCSwitch *s = malloc(sizeof(_RCSwitch));
    void *tx = _rcs_tx_init(transmitter_pin);
    _rcs_tx_set_pulse_length(tx, 100);
    _rcs_tx_set_repeat_transmit(tx, 10);
    _rcs_tx_set_protocol(tx, 1);
    s->tx = tx;
    s->type = 'C';
    s->get_codeword = &_rcs_get_codewordC;
    s->params.c_family = c_family;
    s->params.n_group = n_group;
    s->params.n_device = n_device;
    return s;
}


/**
 * 
 * 
 */
void rcs_switch_destroy(void *_s) {
    _RCSwitch *s = _s;
    _rcs_tx_destroy(s->tx);
    s->tx = NULL;
    free(s);
    s = NULL;
}


/**
 * 
 * 
 */
void rcs_switch_on(void *_s) {
    _RCSwitch *s = _s;
    char *(*get_codeword)(_Params params, int status) = s->get_codeword;
    _rcs_tx_sendTriState(s->tx, get_codeword(s->params, TRUE));
}


/**
 * 
 * 
 */
void rcs_switch_off(void *_s) {
    _RCSwitch *s = _s;
    char *(*get_codeword)(_Params params, int status) = s->get_codeword;
    _rcs_tx_sendTriState(s->tx, get_codeword(s->params, FALSE));
}



/**
 * 
 * 
 */
void *_rcs_tx_init(int pin) {
    _rcs_init_wiring_pi();
    _Tx *tx = malloc(sizeof(_Tx));
    if(tx != NULL) {
        tx->pin = pin;
        tx->pulse_length = 350;
        tx->repeat_transmit = 10;
        tx->protocol = 1;
    }
    return tx;
}


/**
 * 
 * 
 */
void _rcs_tx_destroy(void *_tx) {
    free(_tx);
}


/**
 * 
 * 
 */
void _rcs_tx_set_pulse_length(void *_tx, int pulse_length) {
    _Tx *tx = _tx;
    tx->pulse_length = pulse_length;
}


/**
 * 
 * 
 */
void _rcs_tx_set_repeat_transmit(void *_tx, int repeat_transmit) {
    _Tx *tx = _tx;
    tx->repeat_transmit = repeat_transmit;
}


/**
 * 
 * 
 */
void _rcs_tx_set_protocol(void *_tx, int protocol) {
    _Tx *tx = _tx;
    tx->protocol = protocol;
}


/**
 * 
 * 
 */
void _rcs_tx_enable_transmit(void *_tx) {
    _Tx *tx = _tx;
    pinMode(tx->pin, OUTPUT);
}


/**
 * 
 * 
 */
void _rcs_tx_disable_transmit(void *_tx) {
    _Tx *tx = _tx;
    tx->pin = -1;
}


/*
 * 
 * 
 */
void _rcs_tx_transmit(_Tx *tx, int high_pulses, int low_pulses) {
    int pin = tx->pin;
    int pulse_length = tx->pulse_length;
    
    if (pin != -1) {
        digitalWrite(pin, HIGH);
        delayMicroseconds(pulse_length * high_pulses);
        digitalWrite(pin, LOW);
        delayMicroseconds(pulse_length * low_pulses);
    }
}


/**
 * Sends a "0" Bit
 *                       _    
 * Waveform Protocol 1: | |___
 *                       _  
 * Waveform Protocol 2: | |__
 */
void _rcs_tx_send0(void *_tx) {
    _Tx *tx = _tx;
    
    if (tx->protocol == 1){
        _rcs_tx_transmit(tx, 1, 3);
    } else if(tx->protocol == 2) {
        _rcs_tx_transmit(tx, 1, 2);
    } else if(tx->protocol == 3) {
        _rcs_tx_transmit(tx, 4, 11);
    }
}


/**
 * Sends a "1" Bit
 *                       ___  
 * Waveform Protocol 1: |   |_
 *                       __  
 * Waveform Protocol 2: |  |_
 */
void _rcs_tx_send1(void *_tx) {
    _Tx *tx = _tx;
    
    if (tx->protocol == 1){
        _rcs_tx_transmit(tx, 3, 1);
    } else if (tx->protocol == 2) {
        _rcs_tx_transmit(tx, 2, 1);
    } else if (tx->protocol == 3) {
        _rcs_tx_transmit(tx, 9, 6);
    }
}


/**
 * Sends a Tri-State "0" Bit
 *            _     _
 * Waveform: | |___| |___
 */
void _rcs_tx_sendT0(void *_tx) {
    _Tx *tx = _tx;
    _rcs_tx_transmit(tx, 1, 3);
    _rcs_tx_transmit(tx, 1, 3);
}


/**
 * Sends a Tri-State "1" Bit
 *            ___   ___
 * Waveform: |   |_|   |_
 */
void _rcs_tx_sendT1(void *_tx) {
    _Tx *tx = _tx;
    _rcs_tx_transmit(tx, 3, 1);
    _rcs_tx_transmit(tx, 3, 1);
}


/**
 * Sends a Tri-State "F" Bit
 *            _     ___
 * Waveform: | |___|   |_
 */
void _rcs_tx_sendTF(void *_tx) {
    _Tx *tx = _tx;
    _rcs_tx_transmit(tx, 1, 3);
    _rcs_tx_transmit(tx, 3, 1);
}


/**
 * Sends a "Sync" Bit
 *                       _
 * Waveform Protocol 1: | |_______________________________
 *                       _
 * Waveform Protocol 2: | |__________
 */
void _rcs_tx_sendSync(void *_tx) {
    _Tx *tx = _tx;
    if (tx->protocol == 1){
        _rcs_tx_transmit(tx, 1, 31);
    } else if (tx->protocol == 2) {
        _rcs_tx_transmit(tx, 1, 10);
    } else if (tx->protocol == 3) {
        _rcs_tx_transmit(tx, 1, 71);
    }
}


/**
 * @param sCodeWord   /^[10FS]*$/  -> see getCodeWord
 */
void _rcs_tx_sendTriState(void *_tx, char* sCodeWord) {
    _Tx *tx = _tx;
    int nRepeat;
    for (nRepeat=0; nRepeat < tx->repeat_transmit; nRepeat++) {
        int i = 0;
        while (sCodeWord[i] != '\0') {
            switch(sCodeWord[i]) {
                case '0':
                _rcs_tx_sendT0(_tx);
                break;
                case 'F':
                _rcs_tx_sendTF(_tx);
                break;
                case '1':
                _rcs_tx_sendT1(_tx);
                break;
            }
        i++;
        }
        _rcs_tx_sendSync(_tx);    
    }
}


/*
 * Returns a char[13], representing the Code Word to be send.
 * A Code Word consists of 9 address bits, 3 data bits and one sync bit but in our case only the first 8 address bits and the last 2 data bits were used.
 * A Code Bit can have 4 different states: "F" (floating), "0" (low), "1" (high), "S" (synchronous bit)
 *
 * +-------------------------------+--------------------------------+-----------------------------------------+-----------------------------------------+----------------------+------------+
 * | 4 bits address (switch group) | 4 bits address (switch number) | 1 bit address (not used, so never mind) | 1 bit address (not used, so never mind) | 2 data bits (on|off) | 1 sync bit |
 * | 1=0FFF 2=F0FF 3=FF0F 4=FFF0   | 1=0FFF 2=F0FF 3=FF0F 4=FFF0    | F                                       | F                                       | on=FF off=F0         | S          |
 * +-------------------------------+--------------------------------+-----------------------------------------+-----------------------------------------+----------------------+------------+
 *
 * @param nAddressCode  Number of the switch group (1..4)
 * @param nChannelCode  Number of the switch itself (1..4)
 * @param bStatus       Wether to switch on (true) or off (false)
 *
 * @return char[13]
 */
char* _rcs_get_codewordB(_Params params, int bStatus) {
    int nAddressCode = params.n_address;
    int nChannelCode = params.n_channel;
    
   int nReturnPos = 0;
   static char sReturn[13];
   
   char* code[5] = { "FFFF", "0FFF", "F0FF", "FF0F", "FFF0" };
   if (nAddressCode < 1 || nAddressCode > 4 || nChannelCode < 1 || nChannelCode > 4) {
    return '\0';
   }
   
   int i;
   for(i = 0; i < 4; i++) {
     sReturn[nReturnPos++] = code[nAddressCode][i];
   }

   for(i = 0; i < 4; i++) {
     sReturn[nReturnPos++] = code[nChannelCode][i];
   }
   
   sReturn[nReturnPos++] = 'F';
   sReturn[nReturnPos++] = 'F';
   sReturn[nReturnPos++] = 'F';
   
   if(bStatus) {
      sReturn[nReturnPos++] = 'F';
   } else {
      sReturn[nReturnPos++] = '0';
   }
   
   sReturn[nReturnPos] = '\0';
   
   return sReturn;
}


/**
 * Returns a char[13], representing the Code Word to be send.
 *
 * getCodeWordA(char*, char*)
 *
 */
char* _rcs_get_codewordA(_Params params, int bStatus) {
    char *sGroup = *params.s_group;
    char *sDevice = *params.s_device;
    
    static char sDipSwitches[13];
    int i = 0;
    int j = 0;
    
    for(i=0; i < 5; i++) {
        if(sGroup[i] == '0') {
            sDipSwitches[j++] = 'F';
        } else {
            sDipSwitches[j++] = '0';
        }
    }

    for(i=0; i < 5; i++) {
        if(sDevice[i] == '0') {
            sDipSwitches[j++] = 'F';
        } else {
            sDipSwitches[j++] = '0';
        }
    }

    if(bStatus) {
        sDipSwitches[j++] = '0';
        sDipSwitches[j++] = 'F';
    } else {
        sDipSwitches[j++] = 'F';
        sDipSwitches[j++] = '0';
    }

    sDipSwitches[j] = '\0';

    return sDipSwitches;
}


/*
 * Like getCodeWord (Type C = Intertechno)
 */
char* _rcs_get_codewordC(_Params params, int bStatus) {
    char sFamily = params.c_family;
    int nGroup = params.c_group;
    int nDevice = params.c_device;
    
    static char sReturn[13];
    int nReturnPos = 0;
  
    if((char)sFamily < 97 || (char)sFamily > 112 || nGroup < 1 || nGroup > 4 || nDevice < 1 || nDevice > 4) {
        return '\0';
    }
  
    char* sDeviceGroupCode =  _rcs_dec2binWzerofill(  (nDevice-1) + (nGroup-1)*4, 4  );
    
    char familycode[16][5] = { 
        "0000", "F000", "0F00", "FF00", "00F0", "F0F0", "0FF0", "FFF0", 
        "000F", "F00F", "0F0F", "FF0F", "00FF", "F0FF", "0FFF", "FFFF" 
    };
    
    int i;
    
    for(i = 0; i<4; i++) {
        sReturn[nReturnPos++] = familycode[ (int)sFamily - 97 ][i];
    }
    
    for(i = 0; i<4; i++) {
        sReturn[nReturnPos++] = (sDeviceGroupCode[3-i] == '1' ? 'F' : '0');
    }
    
    sReturn[nReturnPos++] = '0';
    sReturn[nReturnPos++] = 'F';
    sReturn[nReturnPos++] = 'F';
    sReturn[nReturnPos++] = bStatus ? 'F' : '0';
        
    sReturn[nReturnPos] = '\0';
    return sReturn;
}


/*
 * Decoding for the REV Switch Type
 *
 * Returns a char[13], representing the Tristate to be send.
 * A Code Word consists of 7 address bits and 5 command data bits.
 * A Code Bit can have 3 different states: "F" (floating), "0" (low), "1" (high)
 *
 * +-------------------------------+--------------------------------+-----------------------+
 * | 4 bits address (switch group) | 3 bits address (device number) | 5 bits (command data) |
 * | A=1FFF B=F1FF C=FF1F D=FFF1   | 1=0FFF 2=F0FF 3=FF0F 4=FFF0    | on=00010 off=00001    |
 * +-------------------------------+--------------------------------+-----------------------+
 *
 * Source: http://www.the-intruder.net/funksteckdosen-von-rev-uber-arduino-ansteuern/
 *
 * @param sGroup        Name of the switch group (A..D, resp. a..d) 
 * @param nDevice       Number of the switch itself (1..3)
 * @param bStatus       Wether to switch on (true) or off (false)
 *
 * @return char[13]
 */

char* _rcs_get_codewordD(_Params params, int bStatus){
    char sGroup = params.c_group;
    int nDevice = params.n_device;
    
    static char sReturn[13];
    int nReturnPos = 0;

    // Building 4 bits address
    // (Potential problem if dec2binWcharfill not returning correct string)
    char *sGroupCode;
    switch(sGroup){
        case 'a':
        case 'A':
            sGroupCode = _rcs_dec2binWcharfill(8, 4, 'F'); break;
        case 'b':
        case 'B':
            sGroupCode = _rcs_dec2binWcharfill(4, 4, 'F'); break;
        case 'c':
        case 'C':
            sGroupCode = _rcs_dec2binWcharfill(2, 4, 'F'); break;
        case 'd':
        case 'D':
            sGroupCode = _rcs_dec2binWcharfill(1, 4, 'F'); break;
        default:
            return '\0';
    }
    
    int i;
    for(i = 0; i<4; i++) {
        sReturn[nReturnPos++] = sGroupCode[i];
    }

    // Building 3 bits address
    // (Potential problem if dec2binWcharfill not returning correct string)
    char *sDevice;
    switch(nDevice) {
        case 1:
            sDevice = _rcs_dec2binWcharfill(4, 3, 'F'); break;
        case 2:
            sDevice = _rcs_dec2binWcharfill(2, 3, 'F'); break;
        case 3:
            sDevice = _rcs_dec2binWcharfill(1, 3, 'F'); break;
        default:
            return '\0';
    }
    
    for (i = 0; i < 3; i++) {
        sReturn[nReturnPos++] = sDevice[i];
    }
    
    // fill up rest with zeros
    for (i = 0; i < 5; i++) {
        sReturn[nReturnPos++] = '0';
    }
    
    // encode on or off
    if (bStatus) {
        sReturn[10] = '1';
    } else {
        sReturn[11] = '1';
    }
    
    // last position terminate string
    sReturn[12] = '\0';
    return sReturn;
}


/*
  * Turns a decimal value to its binary representation
  */
char *_rcs_dec2binWzerofill(unsigned long dec, unsigned int bit_length) {
    return _rcs_dec2binWcharfill(dec, bit_length, '0');
}


char *_rcs_dec2binWcharfill(unsigned long dec, unsigned int bit_length, char fill) {
  static char bin[64];
  unsigned int i, j = 0;

  while(dec > 0) {
    bin[32+i++] = ((dec & 1) > 0) ? '1' : fill;
    dec = dec >> 1;
  }
  
  for(j = 0; j < bit_length; j++) {
    if(j >= bit_length - i) {
      bin[j] = bin[ 31 + i - (j - (bit_length - i)) ];
    } else {
      bin[j] = fill;
    }
  }
  bin[bit_length] = '\0';
  
  return bin;
}
