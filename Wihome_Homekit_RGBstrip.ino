/*
 * ESP8266_homekit_rgbstrip.ino
 *
 *  Created on: 2020-11-15
 *      Author: Gernot Fattinger
 *
 * Control a RGB LED strip through homekit.
 */

#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "RGBstrip.h"
#include "WiHomeComm.h"
#include "NoBounceButtons.h"
#include "SignalLED.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

WiHomeComm whc(false);
bool initial_connect = true;
bool initial_update = true;

NoBounceButtons nbb;
char button0;
SignalLED led(15, SLED_OFF, false);
RGBstrip rgbstrip(13,4,12,false);

// access the HomeKit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t accessory_config;
extern "C" homekit_characteristic_t cha_on;
extern "C" homekit_characteristic_t cha_bright;
extern "C" homekit_characteristic_t cha_sat;
extern "C" homekit_characteristic_t cha_hue;

static uint32_t next_heap_millis = 0;

void setup()
{
	Serial.begin(115200);
	whc.set_status_led(&led, &rgbstrip);
  button0 = nbb.create(0);
	whc.set_button(&nbb, button0, NBB_LONG_CLICK);
	rgbstrip.set_on(0);
	rgbstrip.set_hsv(0,0,100);
	if (whc.is_homekit_reset())
		homekit_storage_reset();
}

void loop()
{
	whc.check();
	nbb.check();
	led.check();
	rgbstrip.check();

	if ((nbb.action(button0) == NBB_CLICK) && (whc.softAPmode==false))
	{
		LOG_D("Button0 was pressed, rgbstrip was: %d", rgbstrip.get_on());
		rgbstrip.invert_on();
		LOG_D("Rgbstrip is now %d.", rgbstrip.get_on());
		cha_on.value.bool_value = (rgbstrip.get_on()>0?true:false);
		homekit_characteristic_notify(&cha_on, cha_on.value);
		nbb.reset(button0);
	}

	initial_characteristics_update();

	if ((whc.status() == WIHOMECOMM_CONNECTED) && initial_connect)
	{
		my_homekit_setup();
		initial_connect = false;
	}
	else if ((whc.status() == WIHOMECOMM_CONNECTED) && ~initial_connect)
		my_homekit_loop();

	delay(10);
}

// Inital update of homekit characteristics
void initial_characteristics_update()
{
	if (!initial_connect && initial_update)
	{
		cha_on.value.bool_value = (rgbstrip.get_on()>0?true:false);
		homekit_characteristic_notify(&cha_on, cha_on.value);
		cha_hue.value.float_value = (float)rgbstrip.get_hue();
		homekit_characteristic_notify(&cha_hue, cha_hue.value);
		cha_sat.value.float_value = (float)rgbstrip.get_saturation();
		homekit_characteristic_notify(&cha_sat, cha_sat.value);
		cha_bright.value.int_value = (int)rgbstrip.get_brightness();
		homekit_characteristic_notify(&cha_bright, cha_bright.value);
		initial_update = false;
	}
}

//==============================
// HomeKit setup and loop
//==============================
void my_homekit_setup()
{

  cha_on.setter = set_on;
  cha_bright.setter = set_bright;
  cha_sat.setter = set_sat;
  cha_hue.setter = set_hue;

	arduino_homekit_setup(&accessory_config);

}

void my_homekit_loop()
{
	arduino_homekit_loop();
	const uint32_t t = millis();
	if (t > next_heap_millis) {
		// show heap info every 5 seconds
		next_heap_millis = t + 5 * 1000;
		LOG_D("Free heap: %d, HomeKit clients: %d",
				ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

	}
}

void set_on(const homekit_value_t v)
{
    bool on = v.bool_value;
    cha_on.value.bool_value = on; //sync the value
		rgbstrip.set_on(on?1:0);
}

void set_hue(const homekit_value_t v)
{
    float hue = v.float_value;
		cha_hue.value.float_value = hue; //sync the value
		Serial.printf("set_hue: %f (%d)\n", hue, (int)hue);
		rgbstrip.set_hue((int)hue);
}

void set_sat(const homekit_value_t v)
{
    float sat = v.float_value;
    cha_sat.value.float_value = sat; //sync the value
    Serial.printf("set_sat: %f (%d)\n", sat, (int)sat);
		rgbstrip.set_saturation((int)sat);
}

void set_bright(const homekit_value_t v)
{
    int bright = v.int_value;
    cha_bright.value.int_value = bright; //sync the value
    Serial.printf("set_bright: %d\n", bright);
		rgbstrip.set_brightness(bright);
}
