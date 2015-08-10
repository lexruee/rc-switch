/**
 * rc-switch-c is a pure C port of the rc-switch C++ library 
 * for the Raspberry Pi.
 *
 * Copyright (C) 2015  Alexander Rüedlinger <a.rueedlinger.gmail.com>
 * 
 * rc-switch-c is a derived work of the rc-switch project:
 *      
 *      https://github.com/sui77/rc-switch
 * 
 *      RCSwitch - Arduino libary for remote control outlet switches
 *      Copyright (c) 2011 Suat Özgür.  All right reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

/**
 * RCSwitch constructors for switch types A, B, C, D.
 */
void *rcs_init_a(char *s_group, char *s_device);
void *rcs_init_b(int n_address, int n_channel);
void *rcs_init_c(char c_family, int n_group, int n_device);
void *rcs_init_d(char c_group, int n_device);

void rcs_enable_transmit(void *_s, int transmitter_pin);

/**
 * RCSwitch setter methods.
 */
void rcs_set_pulse_length(void *_s, int pulse_length);
void rcs_set_repeat_transmit(void *_s, int repeat_transmit);
void rcs_set_protocol(void *_s, int protocol);


/**
 * RCSwitch switch methods.
 */
void rcs_switch_on(void *_s);
void rcs_switch_off(void *_s);



void rcs_destroy(void *_s);
