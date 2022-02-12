#ifndef _DHT11_H_
#define _DHT11_H_

#define DHT11_OK			0
#define DHT11_Err			1

typedef struct DHT11_Info DHT11_Info;

struct DHT11_Info
{
	_Bool device_ok;
	
	float tempreture;
	float humidity;
 	
	_Bool (*dht11_exit)(void);
	_Bool (*dht11_data)(void);
};

extern DHT11_Info dht11_info;

_Bool DHT11_Exist(void);
_Bool DHT11_Get_TemperAndHumi(void);

#endif
