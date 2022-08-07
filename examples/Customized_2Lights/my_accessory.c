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

homekit_characteristic_t cha_light_on_c = HOMEKIT_CHARACTERISTIC_(ON, false,);
homekit_characteristic_t cha_light_on_w = HOMEKIT_CHARACTERISTIC_(ON, false,);
homekit_characteristic_t cha_light_name_w = HOMEKIT_CHARACTERISTIC_(NAME, "WarmLeds");
homekit_characteristic_t cha_light_name_c = HOMEKIT_CHARACTERISTIC_(NAME, "CoolLeds");
homekit_characteristic_t cha_light_brightnes_w= HOMEKIT_CHARACTERISTIC_(BRIGHTNESS,50);
homekit_characteristic_t cha_light_brightnes_c= HOMEKIT_CHARACTERISTIC_(BRIGHTNESS,50);

homekit_characteristic_t cha_temperature = HOMEKIT_CHARACTERISTIC_(CURRENT_TEMPERATURE, 1);

homekit_characteristic_t cha_humidity = HOMEKIT_CHARACTERISTIC_(CURRENT_RELATIVE_HUMIDITY, 1);


homekit_accessory_t *accessories[] = {
	HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_lightbulb, .services=(homekit_service_t*[]) {
    	HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
 HOMEKIT_CHARACTERISTIC(NAME, "LedStrip2W-DHT11"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Arduino HomeKit"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "0123457"),
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.1"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
			NULL
		}),
    	HOMEKIT_SERVICE(LIGHTBULB, .primary=true, .characteristics=(homekit_characteristic_t*[]) {
			&cha_light_on_w,
            &cha_light_name_w,
            &cha_light_brightnes_w,
			NULL
		}),
		NULL
	}),
		HOMEKIT_ACCESSORY(.id=2, .category=homekit_accessory_category_lightbulb, .services=(homekit_service_t*[]) {
    	HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
 HOMEKIT_CHARACTERISTIC(NAME, "CoolLeds"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
			NULL
		}),
    	HOMEKIT_SERVICE(LIGHTBULB, .primary=true, .characteristics=(homekit_characteristic_t*[]) {
			&cha_light_on_c,
            &cha_light_name_c,
            &cha_light_brightnes_c,
			NULL
		}),
		NULL
	}),
  HOMEKIT_ACCESSORY(.id=3, .category=homekit_accessory_category_sensor, .services=(homekit_service_t*[]) {
      HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
      HOMEKIT_CHARACTERISTIC(NAME, "Temperature Sensor"),
      HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
      NULL
    }),
      HOMEKIT_SERVICE(TEMPERATURE_SENSOR, .primary=true, .characteristics=(homekit_characteristic_t*[]) {
      HOMEKIT_CHARACTERISTIC(NAME, "Temperature"),
      &cha_temperature,
      NULL
    }),
    NULL
  }),
  HOMEKIT_ACCESSORY(.id=4, .category=homekit_accessory_category_sensor, .services=(homekit_service_t*[]) {
      HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
      HOMEKIT_CHARACTERISTIC(NAME, "Humidity Sensor"),
      HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
      NULL
    }),
      HOMEKIT_SERVICE(HUMIDITY_SENSOR, .primary=true, .characteristics=(homekit_characteristic_t*[]) {
      HOMEKIT_CHARACTERISTIC(NAME, "Humidity"),
      &cha_humidity,
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
