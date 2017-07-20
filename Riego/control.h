#pragma once

double sat(double value, double min, double max) {
	if(value < min) {
		return min;
	}

	if(value > max) {
		return max;
	}
	return value;
}

/*class Buffer {*/
/*	private:*/
/*		static const int size = 5;*/
/*		double buffer[size];*/
/*		int read = size - 1;*/
/*		int write = 0;*/
/*	public:*/
/*		void put(double value);*/
/*		double remove();*/
/*};*/

class Controller {
	protected:
		double sp = 0.0;
	public:
		virtual double update(double y);
		void setpoint(double sp);
		double get_setpoint();
};

class Component {
	protected:
		int pin = A0;
	public:
		void setPin(int pin);
};

class Sensor : public Component {
	private:
		int points = 0;
		double y_cum = 0;
		double y_last = 0;
		double (*f)(double voltage);    
	public:
		void update();
		double mean();
		double read();
		double convert(double voltage);
		void setConvertFunction(double (*f)(double));
};

class Actuator : public Component {
	private:
		double min = 0.0;
		double max = 1.0;
	public:
		double write(double value);
		void setRange(double min, double max);
		void setMin(double min);
		void setMax(double max);
};
