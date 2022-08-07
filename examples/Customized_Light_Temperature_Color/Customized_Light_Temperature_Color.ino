

#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);


// access your HomeKit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;

extern "C" homekit_characteristic_t cha_light_on;
extern "C" homekit_characteristic_t cha_light_brightness;
extern "C" homekit_characteristic_t cha_light_colortemp;


static uint32_t next_heap_millis = 0;


#define PIN_COOL 0
#define PIN_WARM 4

bool is_on = false;
float current_brightness =  0.0;
float current_coltemp = 0.0;


void updateLight()
{
   if(is_on)
  {
    int b = map(current_brightness,0, 100,0, 255);
    int t_warm = map(current_coltemp,70, 400,0, b);
    int t_cool = map((400-current_coltemp),70, 400,0, b);
      analogWrite(PIN_COOL,t_warm);
      analogWrite(PIN_WARM,t_cool);
   }
   
  else if(!is_on) //lamp - switch to off
  {
      analogWrite(PIN_COOL, 0);
      analogWrite(PIN_WARM, 0);
  }
}

void set_bright(const homekit_value_t v) {
    //Serial.println("set_bright");
    int bright = v.int_value;
    cha_light_brightness.value.int_value = bright; //sync the value
    current_brightness = bright;
    updateLight();
}
void set_temp(const homekit_value_t v) {
    //Serial.println("set_temp");
    int temp = v.int_value;
    cha_light_colortemp.value.int_value = temp; //sync the value
    current_coltemp = temp;
    updateLight();
}
void set_on(const homekit_value_t v) {
    bool on = v.bool_value;
    cha_light_on.value.bool_value = on; //sync the value
    if(on) {
        is_on = true;
    } else  {
        is_on = false;
    }
    updateLight();
}

void my_homekit_setup() {

  pinMode(PIN_COOL, OUTPUT);
  pinMode(PIN_WARM, OUTPUT);

	cha_light_on.setter = set_on;
  cha_light_brightness.setter = set_bright;
  cha_light_colortemp.setter = set_temp;

	arduino_homekit_setup(&config);


}
void my_homekit_loop() {
	arduino_homekit_loop();
	const uint32_t t = millis();
	if (t > next_heap_millis) {
		next_heap_millis = t + 5 * 1000;
		LOG_D("Free heap: %d, HomeKit clients: %d",
				ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
	}
}


void setup() {
	Serial.begin(115200);
	wifi_connect(); // in wifi_info.h
	//homekit_storage_reset(); // to remove the previous HomeKit pairing storage when you first run this new HomeKit example
	my_homekit_setup();
}

void loop() {
	my_homekit_loop();
	delay(10);
}

