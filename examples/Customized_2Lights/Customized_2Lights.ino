

#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"
#include "DHT.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);
#define PIN_COOL 0
#define PIN_WARM 4
#define PIN_WARM 14
#define DHTTYPE DHT11 

// access your HomeKit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;

extern "C" homekit_characteristic_t cha_light_on_w;
extern "C" homekit_characteristic_t cha_light_brightnes_w;
extern "C" homekit_characteristic_t cha_light_on_c;
extern "C" homekit_characteristic_t cha_light_brightnes_c;
extern "C" homekit_characteristic_t cha_temperature;
extern "C" homekit_characteristic_t cha_humidity;


static uint32_t next_heap_millis = 0;
static uint32_t next_report_millis = 0;
bool is_on_w = false;
bool is_on_c = false;
float current_brightnes_w =  0.0;
float current_brightnes_c =  0.0;


DHT dht(DHTPin, DHTTYPE);

void updateLight()
{
   if(is_on_c){
    int c = map(current_brightnes_c,0, 100,0, 255);
    analogWrite(PIN_COOL,c);
   }
   
  else 
  {
      analogWrite(PIN_COOL, 0);
  }
  if(is_on_w){
    int w = map(current_brightnes_w,0, 100,0, 255);
    analogWrite(PIN_WARM,w);
   }
   
  else 
  {
      analogWrite(PIN_WARM, 0);
  }
}


void set_bright_w(const homekit_value_t v) {
    int bright_w = v.int_value;
    cha_light_brightnes_w.value.int_value = bright_w; //sync the value
    current_brightnes_w = bright_w;
    updateLight();
}
void set_bright_c(const homekit_value_t v) {
    int bright_c = v.int_value;
    cha_light_brightnes_c.value.int_value = bright_c; //sync the value
    current_brightnes_c = bright_c;
    updateLight();
}

void set_on_w(const homekit_value_t v) {
    bool on_w = v.bool_value;
    cha_light_on_w.value.bool_value = on_w; //sync the value
    if(on_w) {
        is_on_w = true;
    } else  {
        is_on_w = false;
    }
    updateLight();
}
void set_on_c(const homekit_value_t v) {
    bool on_c = v.bool_value;
    cha_light_on_c.value.bool_value = on_c; //sync the value
    if(on_c) {
        is_on_c = true;
    } else  {
        is_on_c = false;
    }
    updateLight();
}

void my_homekit_setup() {

  pinMode(PIN_COOL, OUTPUT);
  pinMode(PIN_WARM, OUTPUT);

	cha_light_on_w.setter = set_on_w;
  cha_light_brightnes_w.setter = set_bright_w;
	cha_light_on_c.setter = set_on_c;
  cha_light_brightnes_c.setter = set_bright_c;

	arduino_homekit_setup(&config);


}
void my_homekit_loop() {
	arduino_homekit_loop();
	const uint32_t t = millis();
    if (t > next_report_millis) {
    // report sensor values every 30 seconds
    next_report_millis = t + 30 * 1000;
    read_sensors();
  }
	if (t > next_heap_millis) {
		next_heap_millis = t + 5 * 1000;
		LOG_D("Free heap: %d, HomeKit clients: %d",
				ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
	}
}
void read_sensors() {
  // FIXME, read your real sensors here.
  float t = random_value(10, 30);
  float h = random_value(30, 70);
  //float h = dht.readHumidity();
   //float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
      return;
   }
  cha_temperature.value.float_value = t;
  homekit_characteristic_notify(&cha_temperature, cha_temperature.value);

  cha_humidity.value.float_value = h;
  homekit_characteristic_notify(&cha_humidity, cha_humidity.value);

}

int random_value(int min, int max) {
  return min + random(max - min);
}

void setup() {
	Serial.begin(115200);
  dht.begin();
	wifi_connect(); // in wifi_info.h
	//homekit_storage_reset(); // to remove the previous HomeKit pairing storage when you first run this new HomeKit example
	my_homekit_setup();
}

void loop() {
	my_homekit_loop();
	delay(10);
}
