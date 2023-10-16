#include <Arduino.h>

class Led
{
public:
	Led() {};

	void set_pin(my_pin_number_t pin_number)
	{
		this->pin = pin_number;
		pinMode(pin_number, OUTPUT);
	}

	void turn_on()
	{
		if (!block)
			digitalWrite(pin, HIGH);
	}

	void turn_off()
	{
		if (!block)
			digitalWrite(pin, LOW);
	}


	void lock()
	{
		block = true;
	}

	void unlock()
	{
		block = false;
	}
  

private:
	my_pin_number_t pin;

	bool block = false;

};