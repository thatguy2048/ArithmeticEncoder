#ifndef DECODER_H_INCLUDED
#define DECODER_H_INCLUDED

#include "BitStream.h"
#include "ProbabilityModel.h"

namespace Arithmetic{

    template<typename model_type>
    class Decoder{
    protected:
        BitStream bs;
        model_type* _model;
        typename model_type::count_pair currentRange;
        typename model_type::count_type currentValue;

        void startDecode(){
            currentRange.first = 0;
            currentRange.second = MAX_COUNT_VALUE;
            currentValue = 0;
            for(unsigned int i = 0; i < NumberOfBitsInType(currentValue); ++i){
                currentValue <<= 1;
                if(bs.numberOfBits() > 0){
                    currentValue += bs.pop_bit()?1:0;
                }
            }
        }

    public:

        Decoder(model_type* model, const BitStream& bitstream){
            _model = model;
            bs = bitstream;
            startDecode();
        }

        typename model_type::symbol_type decodeSymbol(){
            typename model_type::symbol_type s = _model->getSymbolFromCount(currentValue, currentRange);
            currentRange = _model->getCountScaled(s,currentRange);

            while(true){
                if(currentRange.second < HALF_COUNT_VALUE){
                }else if(currentRange.first >= HALF_COUNT_VALUE){
                    currentValue -= HALF_COUNT_VALUE;
                    currentRange.first -= HALF_COUNT_VALUE;
                    currentRange.second -= HALF_COUNT_VALUE;
                }else if( currentRange.first >= QUARTER_COUNT_VALUE && currentRange.second < THREE_QUARTERS_COUNT_VALUE){
                    currentValue -= QUARTER_COUNT_VALUE;
                    currentRange.first -= QUARTER_COUNT_VALUE;
                    currentRange.second -= QUARTER_COUNT_VALUE;
                }else{
                    break;
                }
                currentRange.first <<= 1;
                currentRange.second = (currentRange.second<<1)+1;
                currentValue <<= 1;
                if(bs.pop_bit())    currentValue+=1;
            }
            return s;
        }
    };
};//end namespace

#endif // DECODER_H_INCLUDED
