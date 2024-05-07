#include <iostream>

#include "gf.h"
#include "help_gf.cpp"
#include "rs_encoding.h"
#include "rs_decoding.h"
#include "help_gf.h"

int main() {

     setlocale(LC_ALL, "rus");
     // D = N - k + 1
     // 9 = 31 - k + 1
     // N = 31
     // D = 9
     // k = 23

     const int D = 9;
     const int N = 31;
     const int k = N - D + 1;

     //unsigned char message[] = " Hello 123 aaaaaaaaaaaaaaa dfdf ";
     std::vector <unsigned char> message = " Hello 123 aaaaaaaaaaaaaaa dfdf ";
     const int msglen = sizeof(message) - 1;

     std::cout << "Message:   ";
//     gf::print(message, msglen);

     std::cout << message << std::endl;

     std::vector<int> original, encoded, erroneous, decoded;
     original.reserve(msglen);
     encoded.reserve(N);
     erroneous.reserve(N);
     decoded.reserve(N);

     std::cout << "Original:  ";
     for (int i = 0; i < msglen; i++) {
         original.push_back(message[i]);
         std::cout << original[i] << " ";
     }
     encoded = rs_encode_msg(original, N-k);

     std::cout << std::endl << "Encoded:   ";
     for (int i = 0; i < encoded.size(); i++) {
         std::cout << encoded[i] << " ";
     }

     std::vector<char> encoded_str;
     for (int i = 0; i < encoded.size(); i++) {
         encoded_str.push_back(encoded[i]);
         erroneous.push_back(encoded[i]);
     }

     std::cout << std::endl << "Erroneous: ";
     srand(time(0));
     for (int i = 0; i < encoded.size(); i++) {
         if (i < (N - k) / 2) {
             int h = rand() % (msglen);
             it r = rand() % 256;
             erroneous[h] = r;
         }
         //std::cout << erroneous[i] << " ";
     }
     gf::print(erroneous);
     
     decoded = rs_decode_msg(erroneous, N-k);
     std::cout << std::endl << "Decoded:   ";

     for (int i = 0; i < decoded.size(); i++) {
         std::cout << decoded[i] << " ";
     }

     std::cout << std::endl << "Message:   ";
     gf::print(decoded, msglen);
     std::cout << std::endl;

//    system("pause");
    return 0;
}
