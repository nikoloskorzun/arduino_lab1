
#include "src//hamming_codec//hamming.h"


typedef int my_pin_number_t;
typedef int ms_tick;




#include "src//led//led.h"
#include "src//caiser//caiser.h"

#define PIN_LED_1 53

#define PIN_S_1 3 //sender, PWM ONLY
#define PIN_S_2 A0 //reciever, ANALOG 

#define PIN_SYNC_R 2 //reciever, DIGITAL pin MUST support interrupts! 
#define PIN_SYNC_S 52 //sender, DIGITAL


class Double_simplex_channel
{

/*

data transfer scheme

device1                               device2

pin_sig_out = 1, ------------------- pin_sig_in


  pin_data_out = byte, ------------- pin_data_in
  delay 100 milli sec


pin_sig_out = 0, ------------------- pin_sig_in


*/

public:
	Double_simplex_channel()
	{
    this->delay_after_send = 100;
	}

	~Double_simplex_channel()
	{

	}

	void set_pins(my_pin_number_t pin_number_data_reciever, my_pin_number_t pin_number_data_sender, my_pin_number_t pin_number_int_sender)
	{
		this->pin_data_out = pin_number_data_sender;
		this->pin_data_in = pin_number_data_reciever;
		this->pin_sig_out = pin_number_int_sender;

		pinMode(pin_number_data_sender, OUTPUT);
		pinMode(pin_number_data_reciever, INPUT);
		pinMode(pin_number_int_sender, OUTPUT);
	}

  

	void send_data(byte data)
	{
    #if DEBUG
      Serial.print("sending byte =");      
      Serial.println(data);
    #endif
    digitalWrite(this->pin_sig_out, HIGH);

    analogWrite(this->pin_data_out, data);
    delay(this->delay_after_send);
    
    digitalWrite(this->pin_sig_out, LOW);

	}
  
  byte listen()
  {
    uint16_t duration;
    duration = pulseIn(this->pin_data_in, HIGH);
    this->data_recieved = this->duration_to_byte(duration);

    #if DEBUG
      Serial.print("recieved duration =");      
      Serial.print(duration);
      Serial.print(" recieved data =");      
      Serial.println(this->data_recieved);
    #endif

    return this->data_recieved;
  }

private:

  byte duration_to_byte(uint16_t duration)
  {
    return duration / 4 - duration / 64;
  }


	byte data_recieved;

	
	my_pin_number_t pin_data_out;
	my_pin_number_t pin_data_in;

	my_pin_number_t pin_sig_out;

  ms_tick delay_after_send;
};




Hamming_codec codec(3);

Led info_led;
Lab1_worker lab1;
Double_simplex_channel channel;


volatile struct Interrupts_flags
{
  volatile byte i0:1;
  byte i1:1;
  //uno ends
  byte i2:1;
  byte i3:1;
  byte i4:1;
  byte i5:1;
  // Mega2560 ends
  byte reserve:2;

} interrupts_flags;


void interrupt_intercept0()
{
   interrupts_flags.i0 = 1;
}


void setup()
{
  	Serial.begin(9600);
  info_led.set_pin(PIN_LED_1);
  lab1.set_caesar_cipher_step(5);
  channel.set_pins(PIN_S_2, PIN_S_1, PIN_SYNC_S);
  
  pinMode(PIN_SYNC_R, INPUT);

	attachInterrupt(digitalPinToInterrupt(PIN_SYNC_R), interrupt_intercept0, RISING);
  interrupts_flags.i0 = 0;

}

void send(byte *data)
{

  (*data) = lab1.get_random_letter();
  byte temp = *data;
  byte part1 = (temp & 0xf0) >> 4;
  byte part2 = (temp & 0x0f);

  Bit_matrix w_part1 = number_to_bit_vector(part1, 4);
  Bit_matrix w_part2 = number_to_bit_vector(part2, 4);

  Bit_matrix res1 = codec.code(w_part1);
  Bit_matrix res2 = codec.code(w_part2);
  res1 = codec.convert_to_extended(res1);
  res2 = codec.convert_to_extended(res2);

  part1 = bit_vector_to_number(res1);
  part2 = bit_vector_to_number(res2);

  channel.send_data(part1);
  channel.send_data(part2);

  //
  //channel.send_data();
}
void recieve(byte* data)
{
  byte byte1 = channel.listen();
  delay(100);
  byte byte2 = channel.listen();

  Bit_matrix w_part1 = number_to_bit_vector(byte1, 8);
  Bit_matrix w_part2 = number_to_bit_vector(byte2, 8);

  w_part1 = codec.decode_extended(w_part1);
  w_part2 = codec.decode_extended(w_part2);

  w_part1 = codec.get_word(w_part1);
  w_part2 = codec.get_word(w_part2);

  w_part1.right_append(w_part2);
  *data = bit_vector_to_number(w_part1);
}

void loop()
{
byte data2;


  if(interrupts_flags.i0)
  {
    info_led.turn_on();

    recieve(&data2);
    interrupts_flags.i0 = 0;
    send(&data2);
    Serial.println(data2);
    	info_led.turn_off();

  }

delay(1000);
}

