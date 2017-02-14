#include "main.h"

using namespace std;
using namespace Arithmetic;

int main(){
    ProbabilityModel model;
    Encoder<ProbabilityModel> encoder(&model);

    //equal probability for all characters, resulting in no reduction in size of encoded result.
    for(unsigned short i = 0; i<256; ++i){
        model.countValue(i);
    }

    cout << "Total Counts: " << model.totalCounts() << endl;

    for(unsigned short i = 0; i < 256; ++i){
        ProbabilityModel::count_pair p = model.getCount(i);
        cout << (unsigned char)i << " : " << p.first << ',' << p.second << endl;
    }

    string toEncode = "HOTDOG TO DECODE";

    cout << "Encode: '" << toEncode << '\'' << endl;
    cout << "Original Size: " << toEncode.size()*8 << " bits" << std::endl;
    encoder.startEncoding();
    for(unsigned int i = 0; i < toEncode.length(); ++i){
        encoder.encodeSymbol(toEncode[i]);
    }
    BitStream encoded_stream = encoder.endEncoding();
    cout << "Finished Encoding" << endl;

    BitStream encoded_stream_tmp = encoded_stream;
    cout << "Encoded Stream: ";
    for(unsigned int i = 0; i < encoded_stream.getCurrentValues().size(); ++i){
        cout << encoded_stream.getCurrentValues()[i] << " ";
    }
    cout << "Number Of Bits: " << encoded_stream_tmp.numberOfBits() << " {";
    bool b = false;
    while(encoded_stream_tmp.numberOfBits() > 0){
        if(b)   cout << ',';
        else b = true;
        cout << encoded_stream_tmp.pop_bit();
    }
    cout << "}" << endl << endl;


    cout << "Start Decoding" << endl;
    Decoder<ProbabilityModel> decoder(&model, encoded_stream);
    cout << "Symbols: " << endl;
    for(unsigned int i = 0; i < toEncode.length(); ++i){
        cout << "\tDecoded Symbol: '" << decoder.decodeSymbol() << '\'' << endl;
    }
    cout <<  "Finished Decoding" << endl;

    cout << "DONE" << endl;
    cin.ignore();
    return 0;
}
