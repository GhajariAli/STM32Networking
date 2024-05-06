#include "MQTTFunctions.h"

static void mqtt_thread(void *arg){
	while (1){

	}
}


void MQTT_init(void)
{
  sys_thread_new("mqtt_thread", mqtt_thread, NULL, DEFAULT_THREAD_STACKSIZE,osPriorityLow);
}
