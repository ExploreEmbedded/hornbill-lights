/*
 * WS2812.cpp
 *
 *  Created on: Nov 27, 2016
 *      Author: kolban
 */

 /*
	code modified to support 'c' by sandeep at ExploreEmbedded.com
*/

#include "WS2812.h"
#include <esp_log.h>
#include <driver/gpio.h>
#include <stdint.h>
#include <driver/rmt.h>
#include <stdlib.h>
#include "sdkconfig.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

static char tag[] = "WS2812";

/**
 * A NeoPixel is defined by 3 bytes ... red, green and blue.
 * Each byte is composed of 8 bits ... therefore a NeoPixel is 24 bits of data.
 * At the underlying level, 1 bit of NeoPixel data is one item (two levels)
 * This means that the number of items we need is:
 *
 * #pixels * 24
 *
 */

/**
 * Set two levels of RMT output to the Neopixel value for a "1".
 */

uint16_t PixelCount = 150; //global pixel count
rmt_channel_t Channel = 0; //remote channel

rmt_item32_t*  Items;
pixel_t *			Pixels;

static void setItem1(rmt_item32_t *pItem) {
	pItem->level0 = 1;
	pItem->duration0 = 7;
	pItem->level1 = 0;
	pItem->duration1 = 6;
} // setItem1


/**
 * Set two levels of RMT output to the Neopixel value for a "0".
 */
static void setItem0(rmt_item32_t *pItem) {
	pItem->level0 = 1;
	pItem->duration0 = 4;
	pItem->level1 = 0;
	pItem->duration1 = 8;
} // setItem0


void hornbillLights_begin(rmt_channel_t channel, gpio_num_t gpioNum, uint16_t pixelCount) {
	PixelCount = pixelCount;
	Channel = channel;
	Items = (rmt_item32_t *)calloc(sizeof(rmt_item32_t), pixelCount * 24);
	Pixels = (pixel_t *)calloc(sizeof(pixel_t),pixelCount);

	rmt_config_t config;
	config.rmt_mode = RMT_MODE_TX;
	config.channel = channel;
	config.gpio_num = gpioNum;
	config.mem_block_num = 1;
	config.tx_config.loop_en = 0;
	config.tx_config.carrier_en = 0;
	config.tx_config.idle_output_en = 1;
	config.tx_config.idle_level = (rmt_idle_level_t)0;
	config.tx_config.carrier_duty_percent = 50;
	config.tx_config.carrier_freq_hz = 10000;
	config.tx_config.carrier_level = (rmt_carrier_level_t)1;
	config.clk_div = 8;

	ESP_ERROR_CHECK(rmt_config(&config));
	ESP_ERROR_CHECK(rmt_driver_install(Channel, 0, 0));
} //


void hornbillLights_setPixels(uint16_t index, uint8_t red, uint8_t green, uint8_t blue)
{
	if (index >= PixelCount)
	{
		ESP_LOGE(tag, "setPixel: index out of range: %d", index);
		return;
	}
	Pixels[index].red = red;
	Pixels[index].green = green;
	Pixels[index].blue = blue;
} // setPixel

void hornbillLights_showPixels()
{
	uint32_t i,j;
	rmt_item32_t *pCurrentItem = Items;
	for (i=0; i<PixelCount; i++) {
		uint32_t currentPixel = Pixels[i].red | (Pixels[i].green << 8) | (Pixels[i].blue << 16);
		for (j=0; j<24; j++) {
			if (currentPixel & 1<<j) {
				setItem1(pCurrentItem);
			} else {
				setItem0(pCurrentItem);
			}
			pCurrentItem++;
		}
	}
	// Show the pixels.
	rmt_write_items(Channel, Items, PixelCount*24,
				1 /* wait till done */);
	//printf("\n\rlights show complete\n\r");
}

void hornbillLights_clear() {
	uint16_t i;
	for (i=0; i<PixelCount; i++) {
		Pixels[i].red = 0;
		Pixels[i].green = 0;
		Pixels[i].blue = 0;
	}
} // clear

//Turns the full strip to given color with no effects
void hornbillLights_fullShow(uint8_t red, uint8_t green, uint8_t blue)
{
	for(uint8_t pixNum =0; pixNum < PixelCount; pixNum++)
	{
		hornbillLights_setPixels(pixNum, green, blue, red); //the WS2812 strip we have shows colors as GRB
	}
	hornbillLights_showPixels();
}

//effects

void rainbowCycle(int SpeedDelay) {
  uint8_t *c;
  uint16_t i, j;
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< PixelCount; i++) {
      c=Wheel(((i * 256 / PixelCount) + j) & 255);
      hornbillLights_setPixels(i, *c, *(c+1), *(c+2));
    }
    hornbillLights_showPixels();
    vTaskDelay(SpeedDelay / portTICK_PERIOD_MS);
  }
}

void theaterChaseRainbow(int SpeedDelay) {
  uint8_t *c;
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < PixelCount; i=i+3) {
          c = Wheel( (i+j) % 255);
          hornbillLights_setPixels(i+q, *c, *(c+1), *(c+2));    //turn every third pixel on
        }
        hornbillLights_showPixels();
				vTaskDelay(SpeedDelay / portTICK_PERIOD_MS);
        for (int i=0; i < PixelCount; i=i+3) {
      			hornbillLights_setPixels(i+q, 0,0,0);        //turn every third pixel off
        }
    }
  }
}

uint8_t * Wheel(uint8_t WheelPos) {
  static uint8_t c[3];
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }
  return c;
}

void FadeInOut(uint8_t red, uint8_t green, uint8_t blue){
  float r, g, b;
  for(int k = 0; k < 256; k=k+1) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    hornbillLights_fullShow(r,g,b);
  }

  for(int k = 255; k >= 0; k=k-2) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    hornbillLights_fullShow(r,g,b);
  }
}
