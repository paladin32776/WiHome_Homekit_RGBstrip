/*
 * RGBstrip_accessory.c
 * Define the accessory in C language using the Macro in characteristics.h
 *
 *  Created on: 2020-12-06
 *      Author: Gernot Fattinger
 */

#include <homekit/homekit.h>
#include <homekit/characteristics.h>

void my_accessory_identify(homekit_value_t _value)
{
	printf("accessory identify\n");
}

homekit_characteristic_t cha_on = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha_name = HOMEKIT_CHARACTERISTIC_(NAME, "wihome_hk_rgb_1");
homekit_characteristic_t cha_bright = HOMEKIT_CHARACTERISTIC_(BRIGHTNESS, 50);
homekit_characteristic_t cha_sat = HOMEKIT_CHARACTERISTIC_(SATURATION, (float) 0);
homekit_characteristic_t cha_hue = HOMEKIT_CHARACTERISTIC_(HUE, (float) 180);

homekit_characteristic_t cha_name_accessory = HOMEKIT_CHARACTERISTIC_(NAME, "wihome_hk_rgb_1");

homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_lightbulb, .services=(homekit_service_t*[])
		{
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[])
				{
            &cha_name_accessory,
						// HOMEKIT_CHARACTERISTIC(NAME, "wihome_hk_rgb_1"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Gernot Fattinger"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "0000002"),
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "0.1"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),
        HOMEKIT_SERVICE(LIGHTBULB, .primary=true, .characteristics=(homekit_characteristic_t*[])
				{
            &cha_on,
            &cha_name,
            &cha_bright,
            &cha_sat,
            &cha_hue,
            NULL
        }),
        NULL
    }),
    NULL
};

homekit_server_config_t accessory_config = {
    .accessories = accessories,
    .password = "111-11-111"
};
