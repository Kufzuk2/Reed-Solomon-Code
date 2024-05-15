#include <iostream>
#include "total.hpp"

#define One_ 1
#define Two_ 2



int main(int argc, char** argv)
{
    const int D = 9;
    const int N = 31;
    const int err_num = 5;
        std::cout << "aaaaaaaaaa" << std::endl;

    if (argv[1] == NULL)
    {

        std::cout << "do not forget about name of file" << std::endl;
        return 0;
    }

    std::string message = " Hello 123 aaaaaaaaaaaaaaa dfdf ";
    //unsigned char message[] = " Hello 123 aaaaaaaaaaaaaaa dfdf ";

    Data data_cl {err_num, message, 2};  //possible to make overloading

        std::cout << "1 eeeee" << std::endl;
    data_cl.encode_(); //
        std::cout << "2 e" << std::endl;
    data_cl.enc_fprint_();
        std::cout << "3" << std::endl;
    
    data_cl.harm_();
    data_cl.harm_fprint_();

    data_cl.dec_harm_fprint_();

    data_cl.decode();
    data_cl.final_fprint_();
}
