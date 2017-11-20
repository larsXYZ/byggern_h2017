#include "ir_sensor.h"
#include "ADC_2560.h"
#include <util/delay.h>
#include "../../../drivers/f_cpu.h"

#define VOLTAGE_THRESHOLD 25 //[0-1023]
#define IR_PIN 0
#define IR_DETECTION_DELAY 30000 //in clock cycles

volatile uint16_t number_detections = 0;
volatile int value_history;
volatile uint64_t cycles_since_detection = 0;
volatile int filter_enable = 0;



void ir_init()
{
	adc2560_init();
	value_history = adc2560_read_bin(IR_PIN);
}

int ir_get_score()
{
	return number_detections;
}

void ir_detect_ball()
{
	int voltage = adc2560_read_bin(IR_PIN);
	
	if (voltage < VOLTAGE_THRESHOLD && value_history > VOLTAGE_THRESHOLD && !filter_enable)
	{
		number_detections++;
		cycles_since_detection = 0;
		filter_enable = 1;
	}
	
	//Update voltage history
	value_history = voltage;
	
	//Update filter-cycle counter
	cycles_since_detection++;
	
	//If enough time has passed, allow detection
	if (cycles_since_detection > IR_DETECTION_DELAY) filter_enable = 0;
}
