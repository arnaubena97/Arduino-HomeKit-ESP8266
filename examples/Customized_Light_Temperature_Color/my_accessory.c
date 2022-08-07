/*
 * my_accessory.c
 * Define the accessory in C language using the Macro in characteristics.h
 *
 *  Created on: 2020-05-15
 *      Author: Mixiaoxiao (Wang Bin)
 */

#include <homekit/homekit.h>
#include <homekit/characteristics.h>

void my_accessory_identify(homekit_value_t _value) {
	printf("accessory identify\n");
}

homekit_characteristic_t cha_light_on = HOMEKIT_CHARACTERISTIC_(ON, false,);
homekit_characteristic_t cha_light_name = HOMEKIT_CHARACTERISTIC_(NAME, "Lights");
homekit_characteristic_t cha_light_brightness= HOMEKIT_CHARACTERISTIC_(BRIGHTNESS,50);

homekit_characteristic_t cha_light_colortemp= HOMEKIT_CHARACTERISTIC_(COLOR_TEMPERATURE,100);

homekit_accessory_t *accessories[] = {
	HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_lightbulb, .services=(homekit_service_t*[]) {
    	HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
 HOMEKIT_CHARACTERISTIC(NAME, "Lights"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Arduino HomeKit"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "0123456"),
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
			NULL
		}),
    	HOMEKIT_SERVICE(LIGHTBULB, .primary=true, .characteristics=(homekit_characteristic_t*[]) {
			&cha_light_on,
            &cha_light_name,
            &cha_light_brightness,
            &cha_light_colortemp,
			NULL
		}),
		NULL
	}),
    NULL
};

homekit_server_config_t config = {
		.accessories = accessories,
		.password = "222-22-222"
};
