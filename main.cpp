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

    string toEncode = "HOTDOG TO DECODE";
    model.countValues((unsigned char*)toEncode.c_str(), toEncode.length());

    cout << "Encode: '" << toEncode << '\'' << endl;
    cout << "Original Size: " << toEncode.size() << "x8 = " << toEncode.size()*8 << " bits" << std::endl;
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
    unsigned int numberOfZero = 0, numberOfOne = 0;
    while(encoded_stream_tmp.numberOfBits() > 0){
        if(b)   cout << ',';
        else b = true;
        if(encoded_stream_tmp.pop_bit()){
            cout << 1;
            numberOfOne++;
        }else{
            cout << 0;
            numberOfZero++;
        }
    }
    cout << "}" << endl;
    cout << "Totals 0:" << numberOfZero << " 1:" << numberOfOne << endl;
    cout << endl;

    cout << "Start Decoding" << endl;
    Decoder<ProbabilityModel> decoder(&model, encoded_stream);
    cout << "Symbols: " << endl;
    for(unsigned int i = 0; i < toEncode.length(); ++i){
        cout << "\tDecoded Symbol: '" << decoder.decodeSymbol() << '\'' << endl;
    }
    cout <<  "Finished Decoding" << endl;

    cout << endl << "Test Binary Encoding" << endl;

    BinaryProbabilityModel binaryModel;

    string binaryToEncode = "100000000000000000000100000000000000000000";

    for(unsigned int i = 0; i < binaryToEncode.length(); ++i){
        binaryModel.countValue(binaryToEncode[i]=='1');
    }

    cout << "To Encode: " << binaryToEncode << endl;
    cout << "Length: " << binaryToEncode.length() << endl;

    Encoder<BinaryProbabilityModel> binEncoder(&binaryModel);

    for(unsigned int i = 0; i < binaryToEncode.length(); ++i){
        binEncoder.encodeSymbol(binaryToEncode[i]=='1');
    }

    BitStream bin_stream = binEncoder.endEncoding();
    BitStream bin_stream_tmp = bin_stream;

    cout << endl << "Binary Stream "<< bin_stream_tmp.numberOfBits() <<" {";
    while(bin_stream_tmp.numberOfBits()){
        cout << bin_stream_tmp.pop_bit();
    }
    cout << "}" << endl;

    cout << "Decoded Binary: ";
    Decoder<BinaryProbabilityModel> binDecoder(&binaryModel, bin_stream);
    for(unsigned int i = 0; i < binaryToEncode.length(); ++i){
        cout <<  binDecoder.decodeSymbol();
    }
    cout << endl;

    cout << endl << "DONE" << endl;
    cin.ignore();
    return 0;
}
