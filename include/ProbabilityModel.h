#ifndef PROBABILITYMODEL_H
#define PROBABILITYMODEL_H

#define MAX_COUNT_VALUE 0xFFFFFFFF
#define THREE_QUARTERS_COUNT_VALUE 0xC0000000
#define HALF_COUNT_VALUE 0x80000000
#define QUARTER_COUNT_VALUE 0x40000000

#include <utility> //includes pair

namespace Arithmetic{

    template<typename countType, typename symbolType, unsigned int max_size>
    class basic_probabilityModel{
    public:
        typedef countType count_type;
        typedef symbolType symbol_type;
        typedef std::pair<count_type, count_type> count_pair;

    protected:
        //Store the counted values for each symbol, with the last being the total number of counted values
        count_type counts[max_size+1] = {0};

    public:

        unsigned int maxSize(){ return max_size;    }

        count_type totalCounts(){   return counts[max_size]; }

        void countValue(const symbol_type& c){
            for(unsigned short i = c+1; i < max_size+1; ++i){
                counts[i]++;
            }
        }

        void countValues(const symbol_type* buffer, unsigned int length){
            while(length-->0){
                countValue(buffer[length]);
            }
        }

        count_pair getCount(const symbol_type& c){
            return std::make_pair(counts[c], counts[c+1]);
        }

        count_pair getCount(const symbol_type& c, const count_type& scaleDenominator, const count_type& offset){
            count_type scale = scaleDenominator/totalCounts();
            return std::make_pair(
                counts[c]*scale + offset,
                counts[c+1]*scale + offset
            );
        }

        count_pair getCountScaled(const symbol_type&c, const count_type& low, const count_type& high){
            return getCount(c, high-low, low);
        }

        count_pair getCountScaled(const symbol_type& c, const count_pair& range){
            return getCountScaled(c, range.first, range.second);
        }

        char countWithinIndex(const count_type& count, const short& indx){
            if(count >= counts[indx+1]) return 1;
            if(count < counts[indx])    return -1;
            return 0;
        }

        unsigned char getSymbolFromCount(const count_type& count,const unsigned short& indxMin, const unsigned short& indxMax){
            unsigned short indx = (indxMax-indxMin)/2+indxMin;
            if(count >= counts[indx+1])     return getSymbolFromCount(count,indx, indxMax);
            else if(count < counts[indx])   return getSymbolFromCount(count,indxMin, indx);
            return indx;
        }

        unsigned char getSymbolFromCount(const count_type& count){
            return getSymbolFromCount(count,0,256);
        }

        unsigned char getSymbolFromCount(const count_type& count, const count_type& scaledDenominator){
            return getSymbolFromCount((count*totalCounts())/scaledDenominator);
        }

        unsigned char getSymbolFromCount(const count_type& count, const count_pair& range){
            return getSymbolFromCount((((count-range.first)))/((range.second-range.first)/totalCounts()));
        }
    };

    typedef basic_probabilityModel<unsigned int, unsigned char, 256> ProbabilityModel;

};//End namespace
#endif // PROBABILITYMODEL_H
