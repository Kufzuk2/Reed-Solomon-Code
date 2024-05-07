/* Author: Yulia Kuznetsova (YuliaKUA)
 * Date: 21.03.2021
 *
 * See LICENSE */

#include <iostream>
#include "main.h"
#include <string>
#include <sstream>
#include <fstream>


int main(int argc, char **argv) {

     setlocale(LC_ALL, "rus");
     // D = N - k + 1
     // 9 = 31 - k + 1
     // N = 31
     // D = 9
     // k = 23

     const int D = 9;
     const int N = 31;
     const int k = N - D + 1;
    


    
    std::string str_mess;
    std::stringstream data_s {str_mess};
    std::ifstream is {argv[1]};
    
/*    
    std::string tmp;
    while (std::getline(tmp))
    {

    }
  */      
    int x;
    is >> x;
    data_s << x;
    // unsigned char message[] = "Ïðèâåò ìèð! Hello 123; kkkkkkkkkkkkkkkkkkkkkkkkkkkdfwnfowejfojowjfijwoifjweijfpwiejfpiwjefijwefijweijfpweijfweijfoiej";
     auto c_str = data_s.str().c_str();

     std::cout << "Message:   ";
     std::cout << c_str << std::endl << "Size: " <<
                strlen(c_str) << std::endl;

     std::vector<int> original, encoded, erroneous, decoded;
     original.reserve(data_s.str().size());
     encoded.reserve(N);
     erroneous.reserve(N);
     decoded.reserve(N);

     std::cout << "Original:  ";
     for (int i = 0; i < data_s.str().size(); i++) {
         original.push_back(c_str[i]);
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
             int h = rand() % (data_s.str().size());
             int r = rand() % 256;
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
     gf::print(decoded, strlen(c_str));
     std::cout << std::endl;

    //system("pause");
    return 0;
}
