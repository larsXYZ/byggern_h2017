#include "ir_sensor.h"
#include "ADC_2560.h"

//See .h file for function explanations

#define VOLTAGE_THRESHOLD 25 //[0-1023]
#define IR_PIN 0 //ADC
#define DETECTION_LIMIT 10 //If ball is detected 10 times in a row, send game over signal. Filters noise

volatile uint16_t detection_count = 0;

void ir_init()
{
	adc2560_init();
	detection_count = 0;
}

int ir_detect_ball()
{
	int voltage = adc2560_read_bin(IR_PIN);
	
	if (voltage < VOLTAGE_THRESHOLD) //The voltage falls when the IR sensors are eclipsed by the ball
	{
		//Increase detection count
		detection_count++;
		
		//If we have enough detections the ball is detected
		if (detection_count > DETECTION_LIMIT) return 1;
	}
	else
	{
		detection_count = 0; //If ball isn't detected reset counter
	}
	
	return 0;
}

