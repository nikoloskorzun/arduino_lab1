#ifndef HAMMING_H

#define HAMMING_H

#include "mat.h"


class Hamming_codec
{
public:
 
    Hamming_codec(my_size_t m);
    ~Hamming_codec();
    

    my_size_t get_k();


    Bit_matrix get_syndrome(Bit_matrix &coded_word);
    Bit_matrix get_word(Bit_matrix& coded_word);

#ifdef HAMMING_CODER_OPTIMIZATION

    Bit_matrix code(Bit_matrix &word, bool fast_code = true);
#else
    Bit_matrix code(Bit_matrix& word);

#endif
    Bit_matrix decode(Bit_matrix& word);
#ifdef HAMMING_EXTENDED_ON
    Bit_matrix decode_extended(Bit_matrix& word);

    Bit_matrix convert_to_extended(Bit_matrix& word);
#endif

    void print_params();
    


#if 0
    void convert_to_extended_matrix();    
#endif

private:
    
    Bit_matrix generator_matrix;
    Bit_matrix parity_check_matrix;
    Bit_matrix B1_matrix;
    
#ifdef HAMMING_CODER_OPTIMIZATION

    Bit_matrix* dict;



#endif // HAMMING_CODER_OPTIMIZATION



    my_size_t m;
    my_size_t n;
    my_size_t k;
    my_size_t r;

    my_size_t syndrome_to_position(Bit_matrix& syndrome);


    void create_generator_matrix();
    void create_parity_check_matrix();
    void create_B1_matrix();

#ifdef HAMMING_CODER_OPTIMIZATION

    void generate_dict();

#endif // HAMMING_CODER_OPTIMIZATION


};

#endif

