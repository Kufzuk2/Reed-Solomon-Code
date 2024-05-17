#include <iostream>
#include "total.hpp"

#define     One_   1
#define     Two_   2
#define    Five_   5
#define  ERr_pr_ 0.1
#define Ker_Num_  750



int main(int argc, char** argv)
{
    const int D = 9;
    const int N = 31;
    const int err_num = 5;
        std::cout << "aaaaaaaaaa" << std::endl;

    if (argv[1] == NULL) // then string
    {
        std::string mes;
        std::cout << "type anything " << std::endl;
        std::getline(std::cin, mes);

        Data data_cl {err_num, mes, 2};  //possible to make overloading

        data_cl.encode_(); //
            std::cout << "2 e" << std::endl;
        data_cl.enc_fprint_();
            std::cout << "3" << std::endl;
        
        data_cl.harm_();
        data_cl.harm_fprint_();

        data_cl.dec_harm_fprint_();

        data_cl.decode();
        data_cl.final_fprint_();

        return 0;
    }

//    std::string message = " Hello 123 aaaaaaaaaaaaaaa dfdf ";
    //unsigned char message[] = " Hello 123 aaaaaaaaaaaaaaa dfdf ";

    Data data_cl {ERr_pr_, argv[1], 5, Ker_Num_};  //possible to make overloading

    data_cl.encode_(); //
        std::cout << "2 e" << std::endl;
   // data_cl.enc_fprint_();
        std::cout << "3" << std::endl;
    
    data_cl.harm_();
    //data_cl.harm_fprint_();

    //data_cl.dec_harm_fprint_();

    data_cl.decode();
    data_cl.final_fprint_();
}
