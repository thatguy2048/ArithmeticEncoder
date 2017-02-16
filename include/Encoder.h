#ifndef ENCODER_H_INCLUDED
#define ENCODER_H_INCLUDED

#include "BitStream.h"
#include "ProbabilityModel.h"

namespace Arithmetic{

    template<typename model_type>
    class Encoder{
    protected:
        model_type* _model;
        BitStream bs;
        typename model_type::count_pair currentRange;
        unsigned int count_bits = {0};

        void output_bit(bool bit){
            bs << bit;
            while(count_bits > 0){
                bs << !bit;
                count_bits--;
            }
        }

    public:

        Encoder(model_type* model){
            _model = model;
            startEncoding();
        }

        void startEncoding(){
            currentRange.first = 0;
            currentRange.second = MAX_COUNT_VALUE;
            count_bits = 0;
            bs.clear();
        }

        void encodeSymbol(const typename model_type::symbol_type& s){
            currentRange = _model->getCountScaled(s,currentRange);
            for(;;){
                if(currentRange.second < HALF_COUNT_VALUE){
                    output_bit(false);
                }else if(currentRange.first >= HALF_COUNT_VALUE){
                    output_bit(true);
                    currentRange.first -= HALF_COUNT_VALUE;
                    currentRange.second -= HALF_COUNT_VALUE;
                }else if(currentRange.first >= QUARTER_COUNT_VALUE && currentRange.second < THREE_QUARTERS_COUNT_VALUE){
                    count_bits++;
                    currentRange.first -= QUARTER_COUNT_VALUE;
                    currentRange.second -= QUARTER_COUNT_VALUE;
                }else{
                    break;
                }

                currentRange.first <<= 1;
                currentRange.second <<= 1;
                currentRange.second |= 1;
            }
        }

        //Finish encoding, and return a bitstream with the result.
        BitStream endEncoding(){
            count_bits++;
            output_bit(currentRange.first >= QUARTER_COUNT_VALUE);
            return bs;
        }
    };
};//end namespace

#endif // ENCODER_H_INCLUDED
