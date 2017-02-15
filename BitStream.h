#ifndef _BITSTREAM_H
#define _BITSTREAM_H

#include <deque>
#include <cstdlib>

#define NumberOfBitsInType(type) (sizeof(type)*8)
#define GetBit(number, bitNumber) ((number >> bitNumber) & 1)
#define SetBit(number, bitValue, bitNumber) (number ^= (-bitValue ^ number) & (1 << bitNumber))

//Class to contain a stream of bit values.
//store_type should be a Primitive Data Type.
template<typename store_type>
class basic_bitstream{
public:
    unsigned char store_type_size = {sizeof(store_type)};
    unsigned char numberOfBitsInType = {NumberOfBitsInType(store_type)};

protected:
    unsigned char write_bit = {0};
    unsigned char read_bit = {0};
    std::deque<store_type> _values;

    void incrementWriteBit(){
        if(++write_bit >= numberOfBitsInType){
            _values.push_back(0);
            write_bit = 0;
        }
    }

    void incrementReadBit(){
        if(++read_bit >= numberOfBitsInType){
            if(_values.size() > 1)   _values.pop_front();
            read_bit = 0;
        }
    }

public:
    basic_bitstream(){
        _values.push_back(0);
    }

    void clear(){
        _values.clear();
        _values.push_back(0);
        write_bit = 0;
        read_bit = 0;
    }

    unsigned int numberOfBits(){    return (_values.size()-1)*numberOfBitsInType + write_bit-read_bit;  }

    void push_bit_true(){
        SetBit(_values.back(), 1, write_bit);
        incrementWriteBit();
    }

    void push_bit_false(){
        SetBit(_values.back(), 0, write_bit);
        incrementWriteBit();
    }

    void push_bit(bool b){
        if(b==0) push_bit_false();
        else    push_bit_true();
    }

    bool pop_bit(){
        bool output = GetBit(_values.front(),read_bit);
        incrementReadBit();
        return output;
    }

    unsigned char currentReadBit(){ return read_bit;    }
    unsigned char currentWriteBit(){    return write_bit;   }
    std::deque<store_type> getCurrentValues(){
        return _values;
    }

    template<typename input_type>
    void push_value(const input_type& inpt){
        unsigned char i = NumberOfBitsInType(input_type);
        while(i > 0){
            push_bit(GetBit(inpt,--i) == 1);
        }
    }

    template<typename output_type>
    output_type pop_value(){
        output_type output;
        unsigned char i = NumberOfBitsInType(output_type);
        while(i > 0){
            SetBit(output, pop_bit(), --i);
        }
        return output;
    }

    //stream operators
    basic_bitstream<store_type>& operator<<(const bool& bit){
        push_bit(bit);
        return *this;
    }

    basic_bitstream<store_type>& operator>>(bool& bit){
        bit = pop_bit();
        return *this;
    }

    template<typename input_type>
    basic_bitstream<store_type>& operator<<(const input_type& value){
        push_value<input_type>(value);
        return *this;
    }

    template<typename output_type>
    basic_bitstream<store_type>& operator>>(output_type& value){
        value = pop_value<output_type>();
        return *this;
    }
};

typedef basic_bitstream<unsigned char> BitStream;

#endif // _BITSTREAM_H
