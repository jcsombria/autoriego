// -- Buffer --
/*void Buffer::put(double value) {*/
/*	noInterrupts();*/
/*	buffer[write] = value;*/
/*	int next = (write + 1) % size;*/
/*	if(write == read) {*/
/*		read = next; */
/*	}*/
/*	write = next;*/
/*	interrupts();*/
/*}*/

/*double Buffer::remove() {*/
/*	double value = 0;*/
/*	noInterrupts();*/
/*	value = buffer[read];*/
/*	int next = (read + 1) % size;*/
/*	if(next != write) {*/
/*		read = next;*/
/*	}*/
/*	interrupts();*/
/*	return value;*/
/*}*/

// -- Controller --
void Controller::setpoint(double sp) {
	this->sp = sp;
}

double Controller::get_setpoint() {
	return this->sp;
}

// -- Component --
void Component::setPin(int pin) {
	this->pin = pin;
}

// -- Sensor --
double Sensor::read() {
	int sensorReading = analogRead(pirPin);
	double voltage = sensorReading * 5.0 / 1024.0;
	return voltage;
}

void Sensor::update() {
	points ++;
	int sensorReading = analogRead(pirPin);
	double voltage = sensorReading * 5.0 / 1024.0;
	if(!isnan(voltage)) {
		y_last = voltage;
		y_cum += y_last;
	}
}

double Sensor::mean() {
	if(points == 0) {
		return 0.0;
	}
	double value = y_cum / points;
	y_cum = 0;
	points = 0;
	return value;
}

double Sensor::convert(double voltage) {
	return f(voltage);
}

void Sensor::setConvertFunction(double (*f)(double)) {
	this->f = f;
}

// -- Actuator --
double Actuator::write(double value) {
	double u = sat(value, min, max);
	analogWrite(pin, 255*u);
	return u;
}

void Actuator::setRange(double min, double max) {
	if(max < min) {
		return;
	}
	this->setMin(min);
	this->setMax(max);
}

void Actuator::setMin(double min) {
	if(min > this->max) {
		return;
	}
	this->min = (min >= 0) && (min <= 1) ? min : this->min;
}

void Actuator::setMax(double max) {
	if(max < this->min) {
		return;
	}
	this->max = (max >= 0) && (max <= 1) ? max : this->max;
}
