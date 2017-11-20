/* Defines CPU clock speed */
#ifndef _FREQUENCY_
#define _FREQUENCY_

#ifdef __AVR_ATmega162__
#define F_CPU 4915200UL // = 4,9152MHz
#elif __AVR_ATmega2560__
#define F_CPU 16000000 // = 16 MHz
#endif


#endif