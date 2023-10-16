#include "mat.h"


#ifdef MEM_COUNTING
my_size_t Bit_matrix::mem_used = 0;
#endif



/*
Constructors and destructors

BEGIN
*/


Bit_matrix::Bit_matrix()
{
    this->amount_rows = 1;
    this->amount_column = 1;

    this->matrix = this->allocate_bits(this->amount_rows, this->amount_column);

}


Bit_matrix::Bit_matrix(bit n)
{
    this->amount_rows = 1;
    this->amount_column = 1;

    this->matrix = this->allocate_bits(this->amount_rows, this->amount_column);
    this->set_element(0, 0, n);
}


Bit_matrix::Bit_matrix(Bit_matrix* other)
{
    this->amount_rows = other->amount_rows;
    this->amount_column = other->amount_column;
    this->matrix = this->allocate_bits(this->amount_rows, this->amount_column);

    for (my_size_t i = 0; i < this->get_size_allocated_memory(); i++)
    {
        this->matrix[i] = other->matrix[i];
    }
}


Bit_matrix::Bit_matrix(const Bit_matrix& other)
{
    this->amount_rows = other.amount_rows;
    this->amount_column = other.amount_column;
    this->matrix = this->allocate_bits(this->amount_rows, this->amount_column);

    for (my_size_t i = 0; i < this->get_size_allocated_memory(); i++)
    {
        this->matrix[i] = other.matrix[i];
    }
}


Bit_matrix::Bit_matrix(my_size_t amount_rows, my_size_t amount_column)
{
    this->amount_rows = amount_rows;
    this->amount_column = amount_column;

    this->matrix = this->allocate_bits(this->amount_rows, this->amount_column);
}


void Bit_matrix::reset(my_size_t amount_rows, my_size_t amount_column)
{

    this->free_bits(this->matrix);
    this->amount_rows = amount_rows;
    this->amount_column = amount_column;
    this->matrix = this->allocate_bits(this->amount_rows, this->amount_column);


}



Bit_matrix::~Bit_matrix()
{
    this->free_bits(this->matrix);

}


/*
Constructors and destructor

END
*/



/*
Setters and Getters

BEGIN
*/


void Bit_matrix::set_zero()
{
    for (my_size_t i = 0; i < this->get_size_allocated_memory(); i++)
        this->matrix[i] = (byte)0;
}


void Bit_matrix::set_ones()
{
    for (my_size_t i = 0; i < this->get_size_allocated_memory(); i++)
        this->matrix[i] = (byte)255;
}


my_size_t Bit_matrix::get_amount_rows()
{
    return this->amount_rows;
}


my_size_t Bit_matrix::get_amount_column()
{
    return this->amount_column;
}


bit Bit_matrix::get_element(my_size_t i, my_size_t j)
{

    my_size_t bit_pos = i * this->amount_column + j;
    my_size_t byte_pos = bit_pos / 8;
    bit_pos %= 8;

    if (this->matrix[byte_pos] & this->bit_mask(bit_pos))
        return 1;
    return 0;
}


void Bit_matrix::set_element(my_size_t i, my_size_t j, bit value)
{
    my_size_t bit_pos = i * this->amount_column + j;
    my_size_t byte_pos = bit_pos / 8;
    bit_pos %= 8;

    if (value)
        this->matrix[byte_pos] |= this->bit_mask(bit_pos);
    else
        this->matrix[byte_pos] &= ~this->bit_mask(bit_pos);
}


void Bit_matrix::reverse_element(my_size_t i, my_size_t j)
{
    //bad code)
    bit el = this->get_element(i, j);
    this->set_element(i, j, !el);
}


inline my_size_t Bit_matrix::get_size_allocated_memory()
{
    return (1 + (this->amount_column * this->amount_rows - 1) / 8);
} 



/*
Setters and Getters

END
*/



/*
Memory functions

BEGIN
*/


byte* Bit_matrix::allocate_bits(my_size_t m, my_size_t n)
{
#ifdef WIN_COMPILE_DEBUG
#if DEBUG > 1

#ifdef MEM_COUNTING


    this->mem_used += this->get_size_allocated_memory();
    std::cout << "Memory used " << this->mem_used << " bytes\n";

#endif
#endif
#endif // WIN_COMPILE_DEBUG

    byte* temp_pointer = new byte[this->get_size_allocated_memory()];
    temp_pointer[this->get_size_allocated_memory() - 1] = 0;

    return temp_pointer;
}


void Bit_matrix::free_bits(void* mem)
{

#ifdef WIN_COMPILE_DEBUG
#if DEBUG > 1

#ifdef MEM_COUNTING

    this->mem_used -= this->get_size_allocated_memory();
    std::cout << "Memory used " << this->mem_used << " bytes\n";
#endif
#endif
#endif // WIN_COMPILE_DEBUG

    delete[] mem;
}


/*
Memory functions

END
*/



/*
Linal functions

BEGIN
*/


void Bit_matrix::transpose()
{

    my_size_t r = this->amount_rows;
    my_size_t c = this->amount_column;

    Bit_matrix temp_matrix(c, r);
    temp_matrix.set_zero();

    for (my_size_t i = 0; i < r; i++)
    {
        for (my_size_t j = 0; j < c; j++)
        {
            temp_matrix.set_element(j, i, this->get_element(i, j));
        }
    }

    *this = temp_matrix;
}


void Bit_matrix::reset_to_identity()
{

    this->set_zero();

    for (my_size_t i = 0; i < (this->amount_rows < this->amount_column ? (this->amount_rows) : (this->amount_column)); i++)
    {
        this->set_element(i, i, 1);
    }
}


inline bool Bit_matrix::is_square()
{
    return (this->amount_column == this->amount_rows);
}


/*
Linal functions

END
*/



/*
Help functions

BEGIN
*/


my_size_t bit_vector_to_number(Bit_matrix& vector)
{
    my_size_t a = 0;

    for (my_size_t i = 0; i < vector.get_amount_column(); i++)
    {
        a |= vector.get_element(0, i);
        a <<= 1;

    }
    a >>= 1;

    return (a);
}

Bit_matrix number_to_bit_vector(my_size_t number, my_size_t size)
{
    Bit_matrix v(1, size); 
    v.set_zero();
    my_size_t temp = number;
    my_size_t l = 0;

    while (temp)
    {
        l++;
        temp >>= 1;
    }

    if (!l)
        l = 1;
    
    for(my_size_t i = 1; i <= l; i++)
    {
        v.set_element(0, size-i, (number & 1));
        number >>= 1;
    }
    v.set_element(0, 0, (number & 1));

    return v;

}


#ifdef WIN_APP
Bit_matrix get_zeros_vector(my_size_t n)
{
    Bit_matrix temp(1, n);
    temp.set_zero();
    return temp;
}
#endif


#ifdef WIN_APP
Bit_matrix get_ones_vector(my_size_t n)
{
    Bit_matrix temp(1, n);
    temp.set_ones();
    return temp;
}
#endif

bool is_zeros_vector(Bit_matrix& v)
{
    for (my_size_t i = 0; i < v.get_amount_column(); i++)
    {
        if (v.get_element(0, i))
            return false;
    }
    return true;
}


bit xor_sum(Bit_matrix& m)
{
    bit sum = 0;
    for (my_size_t i = 0; i < m.get_amount_column(); i++)
    {
        sum ^= m.get_element(0, i);
    }
    return sum;
}

#ifdef WIN_APP
Bit_matrix get_random_vector(my_size_t len, bit(*get_rand)())
{
    Bit_matrix v(1, len);

    for (my_size_t i = 0; i < len; i++)
    {
        v.set_element(0, i, get_rand());
    }

    return v;
}
#endif

/*
Help functions

END
*/



/*
Append functions

BEGIN
*/


void Bit_matrix::right_append(Bit_matrix& other)
{
    my_size_t l_amount_rows = this->get_amount_rows();
    my_size_t l_amount_column = this->get_amount_column();
    my_size_t r_amount_rows = other.get_amount_rows();
    my_size_t r_amount_column = other.get_amount_column();

    my_size_t j_;

    if (l_amount_rows == r_amount_rows)
    {
        Bit_matrix temp(l_amount_rows, l_amount_column + r_amount_column);
        temp.set_zero();
        for (my_size_t i = 0; i < l_amount_rows; i++)
        {
            j_ = 0;
            for (my_size_t j = 0; j < l_amount_column + r_amount_column; j++)
            {
                if (j < l_amount_column)
                {
                    temp.set_element(i, j, this->get_element(i, j_));
                }
                else
                {
                    temp.set_element(i, j, other.get_element(i, j_));
                }


                j_++;
                if (j == (l_amount_column - 1))
                    j_ = 0;
            }

        }

        *this = temp;
    }
}


void Bit_matrix::down_append(Bit_matrix& other)
{
    my_size_t l_amount_rows = this->get_amount_rows();
    my_size_t l_amount_column = this->get_amount_column();
    my_size_t r_amount_rows = other.get_amount_rows();
    my_size_t r_amount_column = other.get_amount_column();

    my_size_t i_ = 0;

    if (l_amount_column == r_amount_column)
    {
        Bit_matrix temp(l_amount_rows + r_amount_rows, l_amount_column);
        temp.set_zero();
        for (my_size_t i = 0; i < l_amount_rows + r_amount_rows; i++)
        {
            for (my_size_t j = 0; j < l_amount_column; j++)
            {
                if (i < l_amount_rows)
                {
                    temp.set_element(i, j, this->get_element(i_, j));
                }
                else
                {
                    temp.set_element(i, j, other.get_element(i_, j));
                }

            }
            i_++;
            if (i == (l_amount_rows - 1))
                i_ = 0;
        }

        *this = temp;
    }
}


/*
Append functions

END
*/



/*
Operator overloads functions

BEGIN
*/


Bit_matrix operator* (Bit_matrix& left, Bit_matrix& right)
{
    my_size_t l_amount_rows = left.get_amount_rows();
    my_size_t l_amount_column = left.get_amount_column();
    my_size_t r_amount_rows = right.get_amount_rows();
    my_size_t r_amount_column = right.get_amount_column();

    Bit_matrix res;

    if (l_amount_column == r_amount_rows)
    {
        res = Bit_matrix(l_amount_rows, r_amount_column);
        res.set_zero();

        for (my_size_t left_row = 0; left_row < l_amount_rows; left_row++)
        {
            for (my_size_t right_column = 0; right_column < r_amount_column; right_column++)
            {
                bit res_el = 0;
                for (my_size_t left_column = 0; left_column < l_amount_column; left_column++)
                {
                    res_el ^= left.get_element(left_row, left_column) & right.get_element(left_column, right_column);
                }

                res.set_element(left_row, right_column, res_el);
            }
        }

    }

    return res;
}


bool Bit_matrix::operator== (Bit_matrix& other)
{
    if (this->amount_column != other.amount_column)
        return false;

    if (this->amount_rows != other.amount_rows)
        return false;

    for (my_size_t i = 0; i < this->amount_rows; i++)
        for (my_size_t j = 0; j < this->amount_column; j++)
            if (this->get_element(i, j) != other.get_element(i, j)) //#todo optimize this
                return false;

    return true;
}


Bit_matrix& Bit_matrix::operator=(const Bit_matrix& matrix)
{
    if (this == &matrix)
    {
        return *this;
    }

    if (this == nullptr)
    {
        this->amount_rows = matrix.amount_rows;
        this->amount_column = matrix.amount_column;
        this->matrix = this->allocate_bits(this->amount_rows, this->amount_column);
    }

    if (this->amount_rows != matrix.amount_rows || this->amount_column != matrix.amount_column)
    {
        this->free_bits(this->matrix);
        this->amount_rows = matrix.amount_rows;
        this->amount_column = matrix.amount_column;
        this->matrix = this->allocate_bits(this->amount_rows, this->amount_column);
    }

    for (my_size_t i = 0; i < this->get_size_allocated_memory(); i++)
    {
        this->matrix[i] = matrix.matrix[i];
    }

    return *this;
}


/*
Operator overloads functions

END
*/



Bit_matrix Bit_matrix::slice(my_size_t i_start, my_size_t i_end, my_size_t j_start, my_size_t j_end)
{
    Bit_matrix res(i_end - i_start + 1, j_end - j_start + 1);

    for (my_size_t i = i_start; i <= i_end; i++)
    {
        for (my_size_t j = j_start; j <= j_end; j++)
        {
            res.set_element(i - i_start, j - j_start, this->get_element(i, j));
        }
    }

    return res;
}


void Bit_matrix::print()
{
#ifdef MATRIX_PRINTING
    for (my_size_t i = 0; i < this->amount_rows; i++)
    {
        for (my_size_t j = 0; j < this->amount_column; j++)
        {
#ifdef WIN_COMPILE_DEBUG
            std::cout << (this->get_element(i, j) ? 1 : 0) << " ";
#endif //WIN_COMPILE_DEBUG
        }
#ifdef WIN_COMPILE_DEBUG
        std::cout << "\n";
#endif //WIN_COMPILE_DEBUG
    }

#endif
}


inline byte Bit_matrix::bit_mask(my_size_t bit_pos)
{
    return (1 << (7 - bit_pos));
}

