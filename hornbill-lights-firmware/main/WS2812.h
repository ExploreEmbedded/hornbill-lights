/*
 * WS2812.h
 *
 *  Created on: Nov 27, 2016
 *      Author: kolban
 */

#ifndef MAIN_WS2812_H_
#define MAIN_WS2812_H_
#include <stdint.h>
#include <driver/rmt.h>

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} pixel_t;

void hornbillLights_begin(rmt_channel_t channel, gpio_num_t gpioNum, uint16_t pixelCount);
void hornbillLights_setPixels(uint16_t index, uint8_t red, uint8_t green, uint8_t blue);
void hornbillLights_fullShow(uint8_t red, uint8_t green, uint8_t blue);
void hornbillLights_showPixels();
void hornbillLights_clear();
void rainbowCycle(int);
void theaterChaseRainbow(int);
uint8_t * Wheel(uint8_t );
void FadeInOut(uint8_t red, uint8_t green, uint8_t blue);


#endif /* MAIN_WS2812_H_ */
