#include <Arduino.h>



class Lab1_worker
{
  public:
  Lab1_worker()
  {
    /*
      Here we assume that the encoding used uses a consecutive arrangement of letters.
    */
    if((byte)'a' < (byte)'A')
      this->shift_upper_lower = (byte)'A' - (byte)'a';
    else
      this->shift_upper_lower = -((byte)'a' - (byte)'A');
  }
  ~Lab1_worker(){}


  byte get_random_letter()
  {
    return random((int)'a', ((int)'z')+1);
  }


  byte to_lower_case(byte letter)
  {
    if((letter < (byte)'Z') && (letter > (byte)'A'))
      return letter - this->shift_upper_lower;
    return letter;
  }

  byte to_upper_case(byte letter)
  {
    if((letter < (byte)'z') && (letter > (byte)'a'))
      return letter + this->shift_upper_lower;
    return letter;
  }


  /*
    Here we use Caesar cipher not for letter but for byte values: 0..255.
  */
  byte decode(byte value)
  {
    int16_t temp = (int16_t)value-(int16_t)this->caesar_cipher_step; 
    if(temp<0)
    {
      temp+=255;
    }
    return (byte)temp;
   }

  byte code(byte value)
  {
    int16_t temp = (int16_t)value+(int16_t)this->caesar_cipher_step; 
    if(temp>255)
    {
      temp-=255;
    }
    return (byte)temp;

  }

  void set_caesar_cipher_step(my_size_t value)
  {
    this->caesar_cipher_step = value;
  }




  private:

  my_size_t caesar_cipher_step;
  byte shift_upper_lower;

};