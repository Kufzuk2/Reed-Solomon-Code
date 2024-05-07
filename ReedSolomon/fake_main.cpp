#include <iostream>
#include "total.hpp"



int main(int argc, char** argv)
{
    const int D = 9;
    const int N = 31;
    const int err_num =40;
        std::cout << "aaaaaaaaaa" << std::endl;

    if (argv[1] == NULL)
    {

        std::cout << "do not forget about name of file" << std::endl;
        return 0;
    }
    Data data_cl {err_num, argv[1]};

    data_cl.encode_(); //
    data_cl.enc_fprint_();
    
    data_cl.harm_();
    data_cl.harm_fprint_();

    data_cl.dec_harm_fprint_();

    data_cl.decode();
    data_cl.final_fprint_();
}
