#include <SoftwareSerial.h>
#include <TimerOne.h>
#include "control.h"

// Sensor
const int period = 10;

const int pinHumedad = A1;
const int pinTemperatura = A0;

const int pinPump = 3;

Sensor T0, H0;
SoftwareSerial USB(0, 1), BT(10, 11); // RX | TX

Actuator P0;

int time = 0;
int last = 0;
// Control Loop
void tic() {
	time ++;
	read();
}

double volts2celsius(double volts) {
	return 100.0*volts - 50.0;
}

double volts2humidity(double volts) {
	return (100.0 / 3.0) * (5.0 - volts);
}

double temperatura = 0;
double humedad = 0;
boolean nuevo = false;
//const double kv = 5.0 / 1024.0; 
void read() {
	double rawTemp = T0.read(),
		rawHumid = H0.read();
  temperatura = T0.convert(rawTemp);
  humedad = H0.convert(rawHumid);
	T0.update();
	H0.update();
  nuevo = true;
}

void setup() {
	H0.setPin(pinHumedad);
	H0.setConvertFunction(volts2humidity);
	T0.setPin(pinTemperatura);
	T0.setConvertFunction(volts2celsius);
  P0.setPin(pinPump);
  P0.setRange(0.0, 0.1);
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(tic);
  BT.begin(9600);
  USB.begin(9600);
}

double pump = 0;
int nextTs = period;
void loop() {
	if(time > nextTs) {
		nextTs += period;
    nuevo = false;
    logTo(USB);
    logTo(BT);
    P0.write(pump);
    pump += 0.1;
    if(pump > 1.0) {
    	pump = 0.0;
    }
  }
}

void logTo(SoftwareSerial dst) {
	dst.print("{\"T0\":");
	dst.print(T0.convert(T0.read()));
	dst.print(", \"H0\": ");
	dst.print(H0.convert(H0.read()));
	dst.print(", \"P0\": ");
	dst.print(pump);
	dst.println("}");
}
