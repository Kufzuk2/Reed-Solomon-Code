#include <iostream>

#include <stdint.h>
#include <string.h>
#include <cstring>
#include <vector>
#include <assert.h>

#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

#include <random>
#include <cmath>
#include <thread>
#include <type_traits>
#include <charconv>
//#include "h.h"

#define GENERATOR 2 //replace alpha in the classical theory

class Data
{


public:

    std::vector<char> short_data_ch_;
    std::vector<char>  long_data_ch_;
    std::vector<int>       data_int_;
    std::vector<int>        encoded_;
    std::vector<int>         harmed_;
    std::vector<int>        decoded_;
    std::vector<int>      recovered_;
    std::vector<int>    cut_decoded_;
    std::vector<int>   harm_decoded_;

    std::string  filename_;
    bool is_photo_ = false;
    bool   parall_ = false;

    std::vector<std::vector<char>>     mega_data_;
    std::vector<std::vector< int>> int_mega_data_;
    int       ker_num_;
    int s_single_size_; // 
    int l_single_size_; // 

    int k_, N_, D_;

    int  init_len_, long_len_, err_num_, synd_size_, loc_err_;
    float err_per_;

    struct pixel_ {
        unsigned char r, g, b;
    };
    //int max_size_ = 1048576;
    int max_size_ = 256;
//k - useful symbolsi

    Data(int err_num, std::string src_file, int f): err_num_ {err_num} 
    {
        std::cout << "MAX_SIZE = " << max_size_ << std::endl;

        /// calculation + check correctness of err_num


        /// reading from file
        std::ifstream file(src_file, std::ios::binary);

        if (f == 1)
        {
            filename_ = src_file;
            if (file) {
                file.seekg(0, std::ios::end);
                int file_size = file.tellg();
                file.seekg(0, std::ios::beg);
            std::cout << "file size =  " << file_size << std::endl;

                short_data_ch_.reserve(file_size);
            std::cout << "beda " << std::endl;


                data_int_.reserve(file_size); // probably * sizeof (int)
                std::cout << "beda aaaa " << std::endl;

                file.read(short_data_ch_.data(), file_size);
                std::cout << "beda aaaa " << std::endl;
                file.close();
                
                std::cout << "beda aaaa " << std::endl;
                init_len_  = file_size;
            }
        }
        else if (f == 2)  //string case
        {
            init_len_ = src_file.size();
            std::cout << "init len = " << init_len_ <<  std::endl;

            short_data_ch_.reserve(init_len_);
            data_int_.reserve(init_len_); // probably * sizeof (int)

            std::cout << "beda aaaa f = 2" << std::endl;
            short_data_ch_.resize(init_len_);
            std::copy(src_file.begin(), src_file.end(), short_data_ch_.begin());

            std::cout << " in constructor encoded size = " << encoded_.size() << " synd size = " << synd_size_ << "in encode_() short data ch size =" << short_data_ch_.size() << std::endl;
        }
        else
        {
            std::cout << "beda aaaa " << std::endl;
			throw "\"incorrect flag for constructor\n _\"";
        }
                

        long_len_  = 2 * err_num_ + init_len_; // it's a minimum, mb better add more
        synd_size_ = long_len_    - init_len_;
                 

             encoded_.reserve(long_len_);
              harmed_.reserve(long_len_);
        harm_decoded_.reserve(long_len_);
           recovered_.reserve(long_len_);
             decoded_.reserve(init_len_);
        long_data_ch_.reserve(long_len_);
            
        std::cout << " int data size = " << data_int_.size() << " synd size = " << synd_size_ << "in encode_() short data ch size =" << short_data_ch_.size() << std::endl;
        
        std::cout << "here is the initial before any changes casted string " << std::endl;
        for (int i = 0; i < init_len_; ++i)
        {
            std::cout << short_data_ch_[i];
        }
        std::cout << std::endl;


        short_data_ch_.resize(init_len_);
//        data_int_ = convert_char_to_int(short_data_ch_);

         for (int i = 0; i < init_len_; i++) {
//             std::cout << "beda " << i << std::endl;
             data_int_.push_back(short_data_ch_[i]);
         } // cast to int
  

        short_data_ch_.clear();
        short_data_ch_.resize(init_len_);
        short_data_ch_ = convert_int_to_char(data_int_); //seems to be needed
/*
        for (int i = 0; i < init_len_; i++)
        {
            short_data_ch_.push_back(static_cast<char>(decoded_[i]));
        }
*/
        std::cout << "here is the initial but twice casted string " << std::endl;
        for (int i = 0; i < init_len_; ++i)
        {
            std::cout << short_data_ch_[i];
        }
        std::cout << std::endl;

    }

    Data(float err_per, std::string src_file, int f, int ker_num): err_per_ {err_per}, ker_num_{ker_num} 
    {
        std::ifstream file(src_file, std::ios::binary);
        parall_ = true;
        filename_ = src_file;

        if (f == 5)
            is_photo_ = true;

        if (file) {

            file.seekg(0, std::ios::end);
            int file_size = file.tellg();
            file.seekg(0, std::ios::beg);

            err_num_  = file_size * err_per_ - 1;

            init_len_ = file_size;
            short_data_ch_.reserve(init_len_);
            data_int_.reserve(init_len_); // probably * sizeof (int)


            s_single_size_ = file_size / ker_num_ + 1;
            loc_err_       = s_single_size_     * err_per_;
            l_single_size_ = s_single_size_ + 2 * loc_err_;

            std::cout << "file size =  " << file_size << " l single size =" << l_single_size_ << " loc err = " << loc_err_<< std::endl;
            while (l_single_size_ >= max_size_)
            {
                ker_num_++;  
                s_single_size_ = file_size / ker_num_ + 1;
                l_single_size_ = s_single_size_ + 2 * err_num_;
                std::cout << "file size =  " << file_size << " l single size =" << l_single_size_ << " loc err = " << loc_err_<< std::endl;
                std::cout << "Number of parallel procceses increased to " << ker_num_ << "for correct work of algorithm" << std::endl;
            }

            mega_data_.reserve(ker_num_);
            mega_data_.resize (ker_num_);
            int_mega_data_.reserve(ker_num_);
            int_mega_data_.resize (ker_num_);
            synd_size_ = l_single_size_ - s_single_size_;

            std::cout << "int mega data size[0] = " << mega_data_[0].size() << "ker num = " << ker_num_ << std::endl;
            for (int i = 0; i < ker_num_ - 1; ++i) {
                    mega_data_[i].reserve(l_single_size_);
                    mega_data_[i].resize(l_single_size_);
                    file.read(mega_data_[i].data(), l_single_size_); // Считываем данные в каждый внутренний вектор
                    int_mega_data_[i] = convert_char_to_int(mega_data_[i]);
            }
                    file.read(&mega_data_[ker_num_ - 1][0], mega_data_[ker_num_ - 1].size()); // Считываем данные в каждый внутренний вектор
                    int_mega_data_[ker_num_ - 1] = convert_char_to_int(mega_data_[ker_num_ - 1]);

            std::cout << "int mega data size[0] = " << mega_data_[0].size() << "ker num = " << ker_num_ << std::endl;
            file.close();
        }
    }
    ///**********************************************************************************
    ///****************************file functions************************************
    ///**********************************************************************************


    int encoded_size_()
    {
        return encoded_.size();
    }

    void enc_fprint_()
    {
        std::ofstream out_file("build/Encoded_data.txt", std::ios::binary); 

        if (parall_)
        {
            for (const auto& innerVector : mega_data_) {
                for (char c : innerVector) {
                    out_file << c;
                }
                out_file << std::endl;  // Добавляем символ новой строки между векторами
            }
            out_file.close();
            return;
        }


        if (out_file)
        {
             for (int i = 0; i < long_len_; i++) {
                 //long_data_ch_.push_back(static_cast<const char>(encoded_[i]));
                 long_data_ch_.push_back(encoded_[i]);
             } // cast to char

            out_file.write(long_data_ch_.data(), long_len_);
            out_file.close();
        }
        else
           std::cout << "didn't manage to write file with encoded data" << std::endl;
    }

    void harm_fprint_()
    {
        if (!is_photo_)
        {    
            long_data_ch_.clear();
            std::ofstream out_file("build/Harmed_data.txt", std::ios::binary); 

            if (out_file)
            {
                 for (int i = 0; i < long_len_; i++) {
                     //long_data_ch_.push_back(static_cast<const char>(encoded_[i]));
                     long_data_ch_.push_back(harmed_[i]);
                 } // cast to char

                out_file.write(long_data_ch_.data(), long_len_);
                out_file.close();
            }
            else
               std::cout << "didn't manage to write file with harmeded data" << std::endl;
        }
    }

    void dec_harm_fprint_()
    {
        short_data_ch_.clear();
        std::cout << "dec harm fprint short data ch size =" << short_data_ch_.size() << std::endl;
        std::ofstream out_file("build/Bad_data.txt", std::ios::binary); 

        if (out_file)
        {
             for (int i = 0; i < init_len_; i++) {
                 //long_data_ch_.push_back(static_cast<const char>(encoded_[i]));
                 short_data_ch_.push_back(harmed_[i]);
             } // cast to char

            out_file.write(short_data_ch_.data(), init_len_);
            out_file.close();
        }
        else
           std::cout << "didn't manage to write file with ppm file " << std::endl;
    }

    void final_fprint_()
    {
        

        if (!is_photo_)
        {
            if (!parall_)
            {
                std::cout << " is not photo " << std::endl;
                short_data_ch_.clear();
                std::ofstream out_file("build/Recovered_data.txt", std::ios::binary); 



                if (out_file)
                {
                    //short_data_ch_ = convert_int_to_char(decoded_);
                    for (int i = 0; i < init_len_; i++)
                    {
                        short_data_ch_.push_back(decoded_[i]);
                    }


                    std::cout << "final  fprint short data ch size =" << short_data_ch_.size() << "decoded_ size" << decoded_.size() << std::endl;

                    
                    std::cout << "short_data_ch_ size  =  " << short_data_ch_.size() << std::endl;
                    std::cout << "here is the decoded string " << std::endl;
                    for (int i = 0; i < init_len_; ++i)
                    {
                        std::cout << short_data_ch_[i];
                    }
                    std::cout << std::endl;

#if 0
                 for (int i = 0; i < init_len_; i++) {
                     //long_data_ch_.push_back(static_cast<const char>(encoded_[i]));
                     short_data_ch_.push_back(decoded_[i]);
                 } // cast to char
#endif
                    out_file.write(short_data_ch_.data(), init_len_);
                    out_file.close();
                }
                else
                   std::cout << "didn't manage to write file with recovereded data" << std::endl;

            }
            else
            {
                std::ofstream out_file("build/Recovered_data.txt", std::ios::binary); 
                if (out_file)
                {
                    out_file.write(short_data_ch_.data(), init_len_);

                    for (int i = 0; i < ker_num_ - 1; ++i)
                    {
                        mega_data_[i] = convert_int_to_char(int_mega_data_[i]);                    
                        out_file.write(mega_data_[i].data(), s_single_size_);
                    }
                    out_file.write(mega_data_[ker_num_ - 1].data(), mega_data_[ker_num_ - 1].size());
                }
                out_file.close();
                return;
            }
        }
        else
        {
        
            short_data_ch_.clear();
            std::ofstream out_file("build/Recovered_photo.ppm", std::ios::binary); 

            if (out_file)
            {
                short_data_ch_ = convert_int_to_char(decoded_);
            std::ofstream out_file("build/Recovered_data.txt", std::ios::binary); 

            }
         }

    }
std::vector<char> convert_int_to_char(const std::vector<int>& intVector) {
    std::vector<char> charVector(intVector.size() * sizeof(int));
    char* charData = reinterpret_cast<char*>(charVector.data());

    for (size_t i = 0; i < intVector.size(); ++i) {
        auto res = std::to_chars(charData + i * sizeof(int), charData + (i + 1) * sizeof(int), intVector[i]);
        if (res.ec != std::errc()) {
            // Handle conversion error
            return {}; // Return empty vector to indicate failure
        }
    }

    return charVector;
}

/*
std::vector<char> convert_int_to_char(const std::vector<int>& intVector) {
    std::vector<char> charVector;
    charVector.reserve(intVector.size());

    for (int i : intVector) {
        charVector.push_back(static_cast<char>(i));
    }

    return charVector;
}
std::vector<char> convert_int_to_char(const std::vector<int>& intVector) {
    std::vector<char> charVector;
    charVector.reserve(intVector.size() * sizeof(int));

    for (int i : intVector) {
        for (size_t byte = 0; byte < sizeof(int); ++byte) {
            char byteValue = static_cast<char>((i >> (byte * 8)) & 0xFF);
            charVector.push_back(byteValue);
        }
    }

    return charVector;
}*/






    /*
std::vector<char> convert_int_to_char(const std::vector<int>& intVector) {
    const size_t totalBytes = intVector.size() * sizeof(int);
    std::vector<char> charVector(totalBytes);

    char* charData = reinterpret_cast<char*>(charVector.data());
    std::memcpy(charData, intVector.data(), totalBytes);

    return charVector;
}*/

/*
std::vector<char> convert_int_to_char(const std::vector<int>& intVector) {
    static_assert(sizeof(int) >= sizeof(char), "int should be larger or equal in size to char");

    std::vector<char> charVector(intVector.size() * sizeof(int));
    char* charData = reinterpret_cast<char*>(charVector.data());

    std::memcpy(charData, intVector.data(), charVector.size());
    return charVector;
}
*/

std::vector<int> convert_char_to_int(const std::vector<char>& charVector) {
    static_assert(sizeof(int) >= sizeof(char), "int should be larger or equal in size to char");
    
    const int* intData = reinterpret_cast<const int*>(charVector.data());
    size_t numInts = charVector.size() / sizeof(int);
    std::vector<int> intVector(intData, intData + numInts);
    return intVector;
}
    ///**********************************************************************************
    ///****************************encoding functions************************************
    ///**********************************************************************************



    std::vector<int> encode_()
    {
        if (!parall_)
        {
            std::cout << "here MAX_SIZE = " << max_size_ << std::endl;
            encoded_ = rs_encode_msg(data_int_, synd_size_);
            std::cout << "encoded size = " << encoded_.size() << " synd size = " << synd_size_ << "in encode_() short data ch size =" << short_data_ch_.size() << std::endl;

            harmed_.resize(long_len_);
            std::copy(encoded_.begin(), encoded_.end(), harmed_.begin());
            return encoded_;
        }

       // for (auto& vec : mega_data_) {
//            threads.emplace_back(rs_encode_msg, std::ref(vec), synd_size_);
           // threads.emplace_back(&Data::rs_encode_msg, this, std::ref(vec), synd_size_);
//            std::function<void()> func = std::bind(&Data::rs_encode_msg, this, std::ref(vec), synd_size_);

         //   auto func = [this, &vec, synd_size_]() { this->rs_encode_msg(vec, synd_size_); };
           // threads.emplace_back(func);
            //threads.emplace_back([func]() { func(); });

#if 0
        for (int i = 0; i < s_single_size_; i++)
            {
                std::cout << int_mega_data_[0][i];
            }
                std::cout << std::endl;
#endif
            
        std::vector<std::thread> threads;
        for (auto& vec : int_mega_data_) {
            auto func = [this, &vec]() {
                
                std::cout << "vec size = " << vec.size() << std::endl;
                this->rs_encode_msg(vec, synd_size_);
            };
            threads.emplace_back(func);
        }

        for (auto& t : threads) {
            t.join();
        }


        for (auto& t : threads) {
            t.join();
        }
    }


// A function that computes a generator polynomial for a given number of redundant characters
   /* @brief Ñomputes a generator polynomial
	* @param red_code_len - the number of characters representing the redundant code
	* @return generator polynomial*/
	std::vector<int> rs_generator_poly(int &red_code_len) {
		std::vector<int> g, temp;
		g.reserve(red_code_len);
		temp.reserve(red_code_len);

		g.push_back(1);
		temp.push_back(1);
		temp.push_back(0);

		for (uint8_t i = 0; i < red_code_len; i++) {
			temp[1] = pow_my(GENERATOR, i);
			g = gf_poly_mult(g, temp);
		}
		return g;                                         //(x-1)*(x-2^(1))*..*(x-2^(red_code_len-1))
	}


	/* @brief encoding of the message
	* @param msg_in - an incoming message that is represented by a vector of polynomials(in this case, integers)
	* @param red_code_len - the number of characters representing the redundant code
	* @return encoded message = vector [msg_in] + [redundant information] (in this case, integers)*/ 
	std::vector<int> rs_encode_msg(std::vector<int>& msg_in, int red_code_len) {
		try {
            std::cout << "msg size = " << msg_in.size() << "red_code_len = " << red_code_len << "max size = " << max_size_ << std::endl;  
            std::cout << "Thread ID: " << std::this_thread::get_id() << std::endl;

			if (msg_in.size() + red_code_len < max_size_) {  // was 256
				int msg_in_size = msg_in.size();
				std::vector<int> gen;
				gen.reserve(red_code_len);

				gen = rs_generator_poly(red_code_len);

				std::vector<int> msg_out;
				int msg_out_size = msg_in_size + red_code_len;
				msg_out.reserve(msg_out_size);

				for (int i = 0; i < msg_out_size; i++) { // initialize msg_out: len = msg_in.size() + red_code_len
					if (i < msg_in_size){
						msg_out.push_back(msg_in[i]);                      // the highest k characters contain the original message
                        
                    }                                                 
					else
					{
						msg_out.push_back(0);
					}
				}

				std::vector<int> quotient, remainder;
				quotient.reserve(msg_out_size);
				remainder.reserve(msg_out_size);

				gf_poly_div(msg_out, gen, quotient, remainder);        // divide our original polynomial by the generator polynomial and use the remainder
                std::reverse(remainder.begin(), remainder.end());
                

				for (int i = 0; i < msg_out_size; i++) { 
					if (i < msg_in_size)
						msg_out[i] = msg_in[i];                      
					else
					{
						msg_out[i] = remainder[msg_out_size - 1 - i];
					}
				}

				//I wrote the division function for polynomials later, so this was the first version of the algorithm, but it's also working!
				
				//// the method of synthetic division
				//for (int i = 0; i < msg_in.size(); i++) {
				//	int coef = msg_out[i];
				//	if (coef != 0) {                                        //log[0] is undefined, so we need to manually check for this case
				//		for (int j = 1; j < gen.size(); j++) {
				//			msg_out[i + j] ^= mul(gen[j], coef);        //~ '+='
				//		}
				//	}
				//}

				//for (int i = 0; i < msg_in.size(); i++) {                    //in the higher k characters, there is now a quotient, 
				//	msg_out[i] = msg_in[i];                                  //we do not need it for encoding, so we rewrite the msg_in here
				//}

				return msg_out;
			}
			else {
				throw "\"The total number of characters - messages + redundant code - exceeds max_size_\"";
			}
		}
		catch (const char* ex) {
			std::cerr << std::endl << std::endl << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =";
			std::cerr << std::endl << "Error: " << ex << std::endl;
			std::cerr << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =" << std::endl;
		}
	}




    std::vector<int> harm_()
    {
        if (!is_photo_)
        {
            if (!parall_)
            {
                srand(time(0));
                for (int i = 0; i < err_num_; i++) 
                {    
                    int h = 20 + rand() % (long_len_ - 21);
            //        std::cout << " not harmed[h] = " << harmed_[h]; 
                    
                    int r = rand() % 256;
                    if (r == harmed_[h])
                        harmed_[h]++;
                    else
                        harmed_[h] = r;
                    std::cout << " r = " << r << " harmed[h] = " << harmed_[h] << std::endl; 
                }
            
            return harmed_;
            }

            for (auto& vec: int_mega_data_)
            {
                
                srand(time(0));
                for (int i = 0; i < loc_err_; i++) 
                {    
                    int h = rand() % (l_single_size_);
            //        std::cout << " not harmed[h] = " << harmed_[h]; 
                    
                        vec[h] = 0;
                    //std::cout << " r = " << r << " harmed[h] = " << harmed_[h] << std::endl; 
                }

            }
        }
        
        std::ifstream inputFile(filename_, std::ios::binary);
        
        // Проверить, открылся ли файл успешно
        if (!inputFile.is_open()) {
            std::cout << "Не удалось открыть файл!" << std::endl;
            return encoded_;  // pointless
        }
        
        std::string format;
        int width, height, maxColorValue;
        
        // Считать заголовок файла PPM
        inputFile >> format >> width >> height >> maxColorValue;
        
        // Пропустить перевод строки
        inputFile.ignore();
        
        // Создать вектор для хранения пикселей изображения
        std::vector<pixel_> pixels(width * height);
        
        // Считать пиксели изображения
        inputFile.read(reinterpret_cast<char*>(pixels.data()), pixels.size() * sizeof(pixel_));
        
        // Закрыть файл после чтения
        inputFile.close();
        
        // Изменить качество изображения (случайное количество пикселей подвергаются изменению)
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, pixels.size() - 1); // Диапазон случайных пикселей
        
        int numpixel_sToDistort = pixels.size() * err_per_; // Пример: изменить 10% пикселей
        
        for (int i = 0; i < numpixel_sToDistort; ++i) {
            int index = dis(gen); // Получить случайный индекс пикселя
            pixels[index].r = 0;
            pixels[index].g = 0;
            pixels[index].b = 0;
        }
        
        // Открыть файл для записи
        std::ofstream outputFile("build/Harmed_photo.ppm", std::ios::binary);
        
        // Записать заголовок файла PPM
        outputFile << format << "\n" << width << " " << height << "\n" << maxColorValue << "\n";
        
        // Записать пиксели изображения
        outputFile.write(reinterpret_cast<char*>(pixels.data()), pixels.size() * sizeof(pixel_));
        
        // Закрыть файл после записи
        outputFile.close();
/*
        ifstream inputFile(filename_, ios::binary);
        
        // Проверить, открылся ли файл успешно
        if (!inputFile.is_open()) {
            cout << "Не удалось открыть файл!" << endl;
            return 1;
        }
        
        string format;
        int width, height, maxColorValue;
        
        // Считать заголовок файла PPM
        inputFile >> format >> width >> height >> maxColorValue;
        
        // Пропустить перевод строки
        inputFile.ignore();
        
        // Создать вектор для хранения пикселей изображения
        vector<pixel_> pixels(width * height);
        
        // Считать пиксели изображения
        inputFile.read(reinterpret_cast<char*>(pixels.data()), pixels.size() * sizeof(pixel_));
        
        // Закрыть файл после чтения
        inputFile.close();
        
        // Изменить качество изображения (случайное количество пикселей подвергаются изменению)
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, pixels.size() - 1); // Диапазон случайных пикселей
        
        int numpixel_sToDistort = pixels.size() * err_per_; // Пример: изменить 10% пикселей
        
        for (int i = 0; i < numpixel_sToDistort; ++i) {
            int index = dis(gen); // Получить случайный индекс пикселя
            pixels[index].r = 0;
            pixels[index].g = 0;
            pixels[index].b = 0;
        }
        
        // Открыть файл для записи
        ofstream outputFile("build/Harmed_photo.ppm", ios::binary);
        
        // Записать заголовок файла PPM
        outputFile << format << "\n" << width << " " << height << "\n" << maxColorValue << "\n";
        
        // Записать пиксели изображения
        outputFile.write(reinterpret_cast<char*>(pixels.data()), pixels.size() * sizeof(pixel_));
        
        // Закрыть файл после записи
        outputFile.close();*/
    }



    ///**********************************************************************************
    ///****************************decoding functions************************************
    ///**********************************************************************************


    std::vector<int> decode()
    {
        if (!parall_)
        {
            std::cout << "harmed size = " << harmed_.size() << " synd size = " << synd_size_ << std::endl;
            decoded_ = rs_decode_msg(harmed_, synd_size_);
            return decoded_;
        }
        
        std::vector<std::thread> threads;
        for (auto& vec : int_mega_data_) {
            auto func = [this, &vec]() {
                this->rs_decode_msg(vec, synd_size_);
            };
            threads.emplace_back(func);
        }

        for (auto& t : threads) {
            t.join();
        }


        for (auto& t : threads) {
            t.join();
        }
    }



#if 0
    //gets data in the initial form
    std::vector<int> harm_decode_()
    {
        harm_decoded_ = rs_decode_msg(erroneous, N-k);
        return harm_decoded_;
    }
#endif // seems to be not needed


    /* @brief Function calculates the polynomial of error syndromes
        * @param msg - an incoming message that is represented by a vector of polynomials(in this case, integers)
        * @param red_code_len - the number of characters representing the redundant code
        * @return generator polynomial */
    std::vector<int> rs_calc_syndromes(std::vector<int>& msg, int& red_code_len) {
        std::vector<int> synd;
        synd.reserve(red_code_len + 1);
        
        for (int i = 0; i < red_code_len + 1; i++) {     //initialization
            synd.push_back(0);
        }

        for (int i = 1; i < red_code_len + 1; i++) {     // We need the first element 0 for mathematical accuracy, otherwise errors occur
            int temp = pow_my(GENERATOR, i - 1);
            synd[i] = gf_poly_eval(msg, temp);       //S[i+1] = C(a^(i))     sig fault
        }

        return synd;
    }

    /* @brief Function find the error locator polynomial L(x) according to the known location of the error (for correct errarta)
        * @param err_pos - vector with the positions of the erroneous characters
        * @return error locator polynomial L(x) */
    std::vector<int> rs_find_errarta_locator(std::vector<int>& err_pos) {
        std::vector<int> e_loc, temp, temp2, add;
        e_loc.reserve(err_pos.size());
        temp.reserve(err_pos.size());
        temp2.reserve(err_pos.size());
        add.reserve(err_pos.size());

        e_loc.push_back(1);

        temp.push_back(1);

        temp2.push_back(0);
        temp2.push_back(0);

        for (int i = 0; i < err_pos.size(); i++) {
            temp2[0] = pow_my(GENERATOR, err_pos[i]);         // since we know location of the error, we can find L(x) as
            add = gf_poly_add(temp, temp2);                   // L(x) = Ï(1 + x*alpha^(i))
            e_loc = gf_poly_mult(e_loc, add);
        }

        return e_loc;
    }


    /* @brief Function find the error polynomial W(x) = S(x)*L(x)
        * @param synd - polynomial of error syndromes (vector int) S(x)
        * @param err_loc - error locator polynomial L(x)
        * @param err_loc_size - size of L(x)
        * @return error polynomial W(x) */
    std::vector<int> rs_find_error_evaluator(std::vector<int> synd, std::vector<int> err_loc, int err_loc_size) {
        std::vector<int> poly_mul, remainder;
        poly_mul.reserve(synd.size() + err_loc.size());

        poly_mul = gf_poly_mult(synd, err_loc);
        remainder.reserve(err_loc_size);
        
        for (int i = poly_mul.size() - err_loc_size; i < poly_mul.size(); i++) {
            remainder.push_back(poly_mul[i]);
        }

        // Since W(x) cannot exceed u-1, where u is the number of errors, we will use division to discard the extra part
        //gf_poly_div(poly_mul, help, quotient, remainder);

        return remainder;
    }


    /* @brief Correction of the input message using the Forney algorithm, which calculates the error value
        * @param msg_in - incoming message that is represented by a vector of polynomials(in this case, integers)
        * @param synd - polynomial of error syndromes (vector int) S(x)
        * @param err_pos - vector with the positions of the erroneous characters
        * @return corrected input message */
    std::vector<int> rs_correct_errata(std::vector<int> msg_in, std::vector<int> synd, std::vector<int> err_pos) {
        std::vector<int> coef_pos, err_loc, err_eval, x;
        coef_pos.reserve(err_pos.size());
        err_loc.reserve(err_pos.size());
        err_eval.reserve(err_pos.size());
        x.reserve(coef_pos.size());

        int len = msg_in.size();
        for (int i = 0; i < err_pos.size(); i++) {                         // convert err positions in the coefficients of degree
            coef_pos.push_back(len - 1 - err_pos[i]);
        }


        // find the error locator polynomial L(x) according to the known location of the error
        err_loc = rs_find_errarta_locator(coef_pos);
        
        //find the error polynomial W(x)
        reverse(synd.begin(), synd.end());
        err_eval = rs_find_error_evaluator(synd, err_loc, err_loc.size());
        reverse(err_eval.begin(), err_eval.end());

        //x - will storage the position of the errors
        //we need get the error location polynomial X from the error positions in err_pos 
        //(the roots of the error locator polynomial, ie, where it evaluates to 0)
        for (int i = 0; i < coef_pos.size(); i++) {
            int l = 255 - coef_pos[i];
            x.push_back(pow_my(GENERATOR, -l));
        }

        //using the Forney algorithm, we find the error magnitudes
        std::vector<int> E, err_loc_prime_tmp;
        E.reserve(msg_in.size());
        err_loc_prime_tmp.reserve(err_pos.size());

        for (int i = 0; i < msg_in.size(); i++) {
            E.push_back(0);                                 //will store the values that need to be corrected to the original message with errors
        }


        int Xlength = x.size();
        reverse(err_eval.begin(), err_eval.end());

        for (int i = 0; i < x.size(); i++) {
            int Xi_inv = inverse(x[i]);

            //Find the formal derivative of the error locator polynomial
            //the formal derivative of the errata locator is used as the denominator of the Forney Algorithm, 
            //which simply says that the ith error value is given by error_evaluator(gf_inverse(Xi)) / error_locator_derivative(gf_inverse(Xi)).
            for (int j = 0; j < Xlength; j++) {
                if (j != i) {
                    err_loc_prime_tmp.push_back(sub(1, mult(Xi_inv, x[j])));
                }
            }

            //error polynomial Yi = W(Xi^(-1))/L'(Xi^(-1))
            //calculate the product that is the denominator of the Forney algorithm(the derivative of the error locator)
            int err_loc_prime = 1;

            for (int coef = 0; coef < err_loc_prime_tmp.size(); coef++) {
                err_loc_prime = mult(err_loc_prime, err_loc_prime_tmp[coef]);
            }
            
            err_loc_prime_tmp.clear();

            int y;                                              
            y = gf_poly_eval(err_eval, Xi_inv);                          // numerator
            y = mult(pow_my(x[i], 1), y);

            try {
                if (err_loc_prime == 0) {                                     // divisor should not be 0
                    throw "Could not find error magnitude";
                }
            }
            catch (const char* ex) {
                std::cerr << std::endl << std::endl << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =";
                std::cerr << std::endl << "Error: " << ex << std::endl;
                std::cerr << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =" << std::endl;
                exit(-1);
            }

            int magnitude = div_my(y, err_loc_prime);                      //The value of the error value calculated by the Forney algorithm
                                                                               //Dividing the error estimator by the derivative of the error locator 
            E[err_pos[i]] = magnitude;                                         //gives us the error value, that is, the value for recovering the symbol
        }

        //correct our erroneous polynomial
        msg_in = gf_poly_add(msg_in, E);                                   // C(x) = C'(x) + E(x) (xor)
        return msg_in;
    }


    /* @brief Function find the error locator polynomial L(x) using the Berlekamp-Messi algorithm
        * @param synd - polynomial of error syndromes (vector int) S(x)
        * @param red_code_len - the number of characters representing the redundant code
        * @return error locator polynomial L(x)*/
    std::vector<int> rs_find_error_locator(std::vector<int> synd, int red_code_len) {
        std::vector<int> err_loc, old_loc;
        err_loc.reserve(synd.size());
        old_loc.reserve(synd.size());

        err_loc.push_back(1);                               //error locator polynomial (sigma) C(x)
        old_loc.push_back(1);                               //the error locator polynomial of the previous iteration
        
        std::cout << "im here in 2 " << std::endl;
        
        int synd_shift = synd.size() - red_code_len;

        //The BerlekampMassey algorithm is an alternative to the ReedSolomon Peterson decoder for solving the set of linear equations.
        //The main idea is that the algorithm iteratively evaluates the error locator polynomial.To do this, it calculates the delta discrepancy, 
        //by which we can determine whether we need to update the error locator or not
        int k = 0, delta = 0;
        for (int i = 0; i < red_code_len; i++) {
            k = i + synd_shift;
            
            //calculating the delta of the discrepancy
            delta = synd[k];
                    
            for (int j = 1; j < err_loc.size() ; j++) {
                delta ^= mult(err_loc[err_loc.size() - 1 - j], synd[k - j]);    // delta = Sn + C1*Sn-1 +..+ Cj*Sk-j
            }

            //shift the polynomials to calculate the next degree
            old_loc.push_back(0);
            
            std::vector<int> new_loc;
            if(delta != 0){                                                         //if delta == 0, algorithm assumed that C(x) and L are correct for the moment and continues
                if (old_loc.size() > err_loc.size()) {                              //~2*L <= k + erase_count
            std::cout << "im here in 3, synd size = " << synd_size_ << " synd.size = " << synd.size() << std::endl;
                    //Computing errata locator polynomial Sigma
                    new_loc = gf_poly_scale(old_loc, delta);
            std::cout << "im here in 3 " << std::endl;
                    old_loc = gf_poly_scale(err_loc, inverse(delta));
            std::cout << "im here in 3 " << std::endl;
                    err_loc = new_loc;
                }
            std::cout << "im here in 3 " << std::endl;

                //Update with the discrepancy
                err_loc = gf_poly_add(err_loc, gf_poly_scale(old_loc, delta));

            }
        }

        while (err_loc.size() && err_loc[0] == 0) {
            err_loc.erase(err_loc.begin());
        }


        int errs = err_loc.size() - 1;
        try {
            if (errs*2 > red_code_len) {
                throw "Too many errors to correct";
            }
        }
        catch (const char* ex) {
            std::cerr << std::endl << std::endl << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =";
            std::cerr << std::endl << "Error: " << ex << std::endl;
            std::cerr << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =" << std::endl;
            exit(-1);
        }

        return err_loc;
    }


    /* @brief Function find the zeros of this polynomial
        * @param err_loc - error locator polynomial L(x)
        * @param nmess - message size
        * @return  vector of the index of the characters that need to be corrected */
    std::vector<int> rs_find_errors(std::vector<int> err_loc, int nmess) {
        // eraseusing the error locator polynomial, we simply use trial substitution to find the zeros of this polynomial, 
        //which identifies the error locations(ie, the index of the characters that need to be corrected)
        std::vector<int> err_pos;
        err_pos.reserve(nmess);
            std::cout << "nmess = " << nmess << std::endl ;

        int errs = err_loc.size() - 1;
        for (int i = 0; i < nmess; i++) {
            if (gf_poly_eval(err_loc, pow_my(GENERATOR, i)) == 0) {   // if 0 then it is a root of the error locator polynomial
                err_pos.push_back(nmess - 1 - i);
            }
        }
        try {
            std::cout << "err_pos.size = " << err_pos.size() << " errs = " << errs << std::endl ;
            if (err_pos.size() != errs) {
                throw "Too many (or few) errors found for the errata locator polynomial!";
            }
        }
        catch (const char* ex) {
            std::cerr << std::endl << std::endl << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =";
            std::cerr << std::endl << "Error: " << ex << std::endl;
            std::cerr << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =" << std::endl;
            exit(-1);
        }
        
        return err_pos;
    }


    std::vector<int> rs_decode_msg(std::vector<int> msg_in, int red_code_len) {
        try {
            if (msg_in.size() > max_size_ - 1) {  // initially 255 
                throw "Message is too long";
            }
        }
        catch (const char* ex) {
            std::cerr << std::endl << std::endl << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =";
            std::cerr << std::endl << "Error: " << ex << std::endl;
            std::cerr << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =" << std::endl;
            exit(-1);
        }
        std::cout << "im here " << std::endl;
//        std::vector<int> msg_out = msg_in;

        std::vector<int> msg_out;
        msg_out.reserve(long_len_);
        std::cout << "after creation msg_in size = " << msg_in.size() << " long len = " << long_len_ << std::endl; 
        msg_out.resize(encoded_.size());
        std::copy(msg_in.begin(), msg_in.end(), msg_out.begin());

        std::cout << "im here 2 " << std::endl;
        //so that we do not count the generator polynomial several times and do not divide, 
        //we immediately count the error syndrome polynomial, and if there is not at least 
        //one non-0 value in it, then the message is not distorted
        std::vector<int> synd = rs_calc_syndromes(msg_out, red_code_len);  // beda here, sig fault
        std::cout << "after syndromes msg_out size = " << msg_out.size() << std::endl; 
        int max = *max_element(synd.begin(), synd.end());
        std::cout << "im here 2 " << std::endl;

        if (max == 0) {
            return msg_out;
        }
        std::cout << "im here 3 " << std::endl;


        std::vector<int> err_pos, err_loc;
        err_loc.reserve(synd.size());
        err_pos.reserve(synd.size());

        std::cout << "im here 4 " << std::endl;
            //Find the error locator polynomial L(x)
            err_loc = rs_find_error_locator(synd, red_code_len);
        std::cout << "im here 5 " << std::endl;
            
            reverse(err_loc.begin(), err_loc.end());

            std::cout << "just before rs_find_errors msg_out size = " << msg_out.size() << std::endl; 
            //find the vector of the index of the characters that need to be corrected
            err_pos = rs_find_errors(err_loc, msg_out.size());
            
            try {
                if (err_pos.size() == 0) {
                    throw "Could not locate error";
                }
            }
            catch (const char* ex) {
                std::cerr << std::endl << std::endl << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =";
                std::cerr << std::endl << "Error: " << ex << std::endl;
                std::cerr << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =" << std::endl;
                exit(-1);
            }

        
        //Find errors values and apply them to correct the message
        //compute errata evaluatorand errata magnitude polynomials, then correct errorsand erasures
        msg_out = rs_correct_errata(msg_out, synd, err_pos);
        
        //count the error syndrome polynomial, and if there is not at least 
        //one non-0 value in it, then the message is decoded successful
        synd = rs_calc_syndromes(msg_out, red_code_len);
        max = *max_element(synd.begin(), synd.end());
        try {
            if (max > 0) {
                throw "Could not correct message";
            }
        }
        catch (const char* ex) {
            std::cerr << std::endl << std::endl << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =";
            std::cerr << std::endl << "Error: " << ex << std::endl;
            std::cerr << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =" << std::endl;
            exit(-1);
        }

        return msg_out;
    }




    
    ///**********************************************************************************
    ///****************************math     functions************************************
    ///**********************************************************************************


#if 0

    //auxiliary output functions
    void print(std::vector<int> vec) {
        for (int i = 0; i < vec.size(); i++) {
            std::cout << vec[i] << " ";
        }
        std::cout << std::endl;
    }

    void print(std::vector<int> vec, int len) {
        //std::cout << std::endl;
        for (int i = 0; i < len; i++) {
            std::cout << char(vec[i]);
        }
        std::cout << std::endl;
    }

    void print(unsigned char* vec, int len) {
        for (int i = 0; i < len ; i++) {
            std::cout << vec[i];
        }
        std::cout << std::endl;
    }

    void print(char* vec, int len) {
        for (int i = 0; i < len; i++) {
            std::cout << vec[i];
        }
        std::cout << std::endl;
    }

    void print(int x) {
        std::cout << x << std::endl;
    }

    void print(std::string x)
    {
        for (int i = 0; i < x.size(); i++) {
            std::cout << x[i];
        }
        std::cout << std::endl;
    }

#endif // pointless prints

    const int exp_[512] = {
        1, 2, 4, 8, 16, 32, 64, 128, 29, 58, 116, 232, 205, 135, 19, 38, 76, 152, 45, 90, 180, 117, 234,
        201, 143, 3, 6, 12, 24, 48, 96, 192, 157, 39, 78, 156, 37, 74, 148, 53, 106, 212, 181, 119, 238,
        193, 159, 35, 70, 140, 5, 10, 20, 40, 80, 160, 93, 186, 105, 210, 185, 111, 222, 161, 95, 190,
        97, 194, 153, 47, 94, 188, 101, 202, 137, 15, 30, 60, 120, 240, 253, 231, 211, 187, 107, 214,
        177, 127, 254, 225, 223, 163, 91, 182, 113, 226, 217, 175, 67, 134, 17, 34, 68, 136, 13, 26, 52,
        104, 208, 189, 103, 206, 129, 31, 62, 124, 248, 237, 199, 147, 59, 118, 236, 197, 151, 51, 102,
        204, 133, 23, 46, 92, 184, 109, 218, 169, 79, 158, 33, 66, 132, 21, 42, 84, 168, 77, 154, 41, 82,
        164, 85, 170, 73, 146, 57, 114, 228, 213, 183, 115, 230, 209, 191, 99, 198, 145, 63, 126, 252,
        229, 215, 179, 123, 246, 241, 255, 227, 219, 171, 75, 150, 49, 98, 196, 149, 55, 110, 220, 165,
        87, 174, 65, 130, 25, 50, 100, 200, 141, 7, 14, 28, 56, 112, 224, 221, 167, 83, 166, 81, 162, 89,
        178, 121, 242, 249, 239, 195, 155, 43, 86, 172, 69, 138, 9, 18, 36, 72, 144, 61, 122, 244, 245,
        247, 243, 251, 235, 203, 139, 11, 22, 44, 88, 176, 125, 250, 233, 207, 131, 27, 54, 108, 216, 173,
        71, 142, 1, 2, 4, 8, 16, 32, 64, 128, 29, 58, 116, 232, 205, 135, 19, 38, 76, 152, 45, 90, 180,
        117, 234, 201, 143, 3, 6, 12, 24, 48, 96, 192, 157, 39, 78, 156, 37, 74, 148, 53, 106, 212, 181,
        119, 238, 193, 159, 35, 70, 140, 5, 10, 20, 40, 80, 160, 93, 186, 105, 210, 185, 111, 222, 161, 95,
        190, 97, 194, 153, 47, 94, 188, 101, 202, 137, 15, 30, 60, 120, 240, 253, 231, 211, 187, 107, 214,
        177, 127, 254, 225, 223, 163, 91, 182, 113, 226, 217, 175, 67, 134, 17, 34, 68, 136, 13, 26, 52,
        104, 208, 189, 103, 206, 129, 31, 62, 124, 248, 237, 199, 147, 59, 118, 236, 197, 151, 51, 102,
        204, 133, 23, 46, 92, 184, 109, 218, 169, 79, 158, 33, 66, 132, 21, 42, 84, 168, 77, 154, 41, 82,
        164, 85, 170, 73, 146, 57, 114, 228, 213, 183, 115, 230, 209, 191, 99, 198, 145, 63, 126, 252, 229,
        215, 179, 123, 246, 241, 255, 227, 219, 171, 75, 150, 49, 98, 196, 149, 55, 110, 220, 165, 87, 174,
        65, 130, 25, 50, 100, 200, 141, 7, 14, 28, 56, 112, 224, 221, 167, 83, 166, 81, 162, 89, 178, 121,
        242, 249, 239, 195, 155, 43, 86, 172, 69, 138, 9, 18, 36, 72, 144, 61, 122, 244, 245, 247, 243,
        251, 235, 203, 139, 11, 22, 44, 88, 176, 125, 250, 233, 207, 131, 27, 54, 108, 216, 173, 71, 142
    };

    const int log_[256] = {
        0, 0, 1, 25, 2, 50, 26, 198, 3, 223, 51, 238, 27, 104, 199, 75, 4, 100, 224, 14, 52, 141, 239,
        129, 28, 193, 105, 248, 200, 8, 76, 113, 5, 138, 101, 47, 225, 36, 15, 33, 53, 147, 142, 218, 240,
        18, 130, 69, 29, 181, 194, 125, 106, 39, 249, 185, 201, 154, 9, 120, 77, 228, 114, 166, 6, 191,
        139, 98, 102, 221, 48, 253, 226, 152, 37, 179, 16, 145, 34, 136, 54, 208, 148, 206, 143, 150, 219,
        189, 241, 210, 19, 92, 131, 56, 70, 64, 30, 66, 182, 163, 195, 72, 126, 110, 107, 58, 40, 84, 250,
        133, 186, 61, 202, 94, 155, 159, 10, 21, 121, 43, 78, 212, 229, 172, 115, 243, 167, 87, 7, 112, 192,
        247, 140, 128, 99, 13, 103, 74, 222, 237, 49, 197, 254, 24, 227, 165, 153, 119, 38, 184, 180, 124,
        17, 68, 146, 217, 35, 32, 137, 46, 55, 63, 209, 91, 149, 188, 207, 205, 144, 135, 151, 178, 220, 252,
        190, 97, 242, 86, 211, 171, 20, 42, 93, 158, 132, 60, 57, 83, 71, 109, 65, 162, 31, 45, 67, 216, 183,
        123, 164, 118, 196, 23, 73, 236, 127, 12, 111, 246, 108, 161, 59, 82, 41, 157, 85, 170, 251, 96, 134,
        177, 187, 204, 62, 90, 203, 89, 95, 176, 156, 169, 160, 81, 11, 245, 22, 235, 122, 117, 44, 215, 79,
        174, 213, 233, 230, 231, 173, 232, 116, 214, 244, 234, 168, 80, 88, 175
    };



    /* ################################
     * # OPERATIONS OVER GALUA FIELDS #
     * ################################ */

     /* @brief Addition in Galua Fields
      * @param x - left operand
      * @param y - right operand
      * @return x + y */
    inline int add(int x, int y) {
        return x ^ y;
    }

    /* ##### GF substraction ###### */
    /* @brief Substraction in Galua Fields
     * @param x - left operand
     * @param y - right operand
     * @return x - y */
    inline int sub(int x, int y) {
        return x ^ y;
    }

   
    /* @brief Multiplication in Galua Fields
     * @param x - left operand
     * @param y - rifht operand
     * @return x * y */
    inline int mult(int x, int y) {

        //std::cout << "im here in mult " << std::endl;
        if (x == 0 || y == 0)
        {
            return 0;
        }
            //std::cout << "beda mult" << std::endl;
          //  std::cout << "im here in mult 2 " << std::endl;
        return exp_[log_[x] + log_[y]];
    }

    /* @brief Division in Galua Fields
     * @param x - dividend
     * @param y - divisor
     * @return x / y */
    inline int div_my(int x, int y) {
        assert(y != 0);
        if (x == 0) return 0;
        return exp_[(log_[x] + 255 - log_[y]) % 255]; //add 255 to make sure the difference is not negative
    }

    /* @brief X in power Y w
     * @param x     - operand
     * @param power - power
     * @return x^power */
    inline int pow_my(int x, int power) {
        int i = log_[x];
        //std::cout << "log" << i << std::endl;
        i *= power;
        i %= 255;
        
        if (i < 0) i = i + 255;

        return exp_[i];
    }

    /* @brief Inversion in Galua Fields
     * @param x - number
     * @return inversion of x */
    inline int inverse(int x) {
        return exp_[255 - log_[x]]; /* == div(1, x); */
    }



    /* ##########################
     * # POLYNOMIALS OPERATIONS #
     * ########################## */


     /* @brief Multiplication polynomial by scalar
      * @param &p    - source polynomial
      * @param x     - scalar
      * @return      - result  polynomial */
    std::vector<int> gf_poly_scale(std::vector<int>& p, int x) {
        std::vector<int> res;
        res.reserve(p.size());
            std::cout << "in gf poly scale " << std::endl;

        for (int i = 0; i < p.size(); i++) {
            res.push_back(mult(p[i], x));
        }
        return res;
    }

    /* @brief Addition of two polynomials
     * @param &p    - right operand polynomial
     * @param &q    - left operand polynomial
     * @return destination polynomial */
    std::vector<int> gf_poly_add(std::vector<int> p, std::vector<int> q) {
        std::vector<int> res;
        int max = std::max(p.size(), q.size());
        res.reserve(max);

        for (int i = 0; i < max; i++) {
            res.push_back(0);
        }

        for (int i = 0; i < p.size(); i++) {
            res[i + res.size() - p.size()] = p[i];
        }
        for (int i = 0; i < q.size(); i++) {
            res[i + res.size() - q.size()] ^= q[i];
        }

        return res;
    }

    /* @brief Multiplication of two polynomials
     * @param &p    - right operand polynomial
     * @param &q    - left operand polynomial
     * @return destination polynomial */
    std::vector<int> gf_poly_mult(std::vector<int>& p, std::vector<int>& q) {
        std::vector<int> res;
        res.reserve(p.size() + q.size() - 1);

        for (int i = 0; i < p.size() + q.size() - 1; i++) {
            res.push_back(0);
        }

        //multiply each coefficient p by all coefficients q
        for (int j = 0; j < q.size(); j++) {
            for (int i = 0; i < p.size(); i++) {
                res[i + j] ^= mult(p[i], q[j]);      //~res[i + j] = add(res[i+j], mul(p[i], q[j]))
            }
        }

        return res;
    }

    /* @brief Division of two polynomials
     * @param &divident    - right operand polynomial
     * @param &divisor    - left operand polynomial
     * @param &quotient - destination polynomial
     * @param &remainder - destination polynomial */
    void gf_poly_div(std::vector<int>& divident, std::vector<int>& divisor, std::vector<int>& quotient, std::vector<int>& remainder) {
        std::vector<int> msg_out;
        msg_out.reserve(divident.size());
                std::cout << "1CCCCC" << std::endl;  

        for (int i = 0; i < divident.size(); i++) {
            msg_out.push_back(divident[i]);
            //std::cout << "msg out ["  << i << "] = " << msg_out[i] << std::endl;
        }
#if 0
                std::cout << "2CCCCC" << std::endl;  
                std::cout << "divisor  size " << divisor.size() << std::endl;  
                std::cout << "divident size " << divident.size() << std::endl;  
                std::cout << "msg out size " << msg_out.size() << std::endl;  
#endif
        //use the synthetic division algorithm
        for (int i = 0; i < divident.size() - (divisor.size() - 1); i++) {
            int coef = msg_out[i];
            //std::cout << "i = "  << i  << std::endl;
            if (coef != 0) {                                          //log[0] is undefined, so we need to manually check for this case
                for (int j = 1; j < divisor.size(); j++) {
                    if (divisor[j] != 0) {
                        //std::cout << "i + j ="  << i + j << std::endl;
                        //msg_out[i + j] ^= mult(divisor[j], coef);      //~ msg_out[i + j] += - divisor[j] * coef, but xor is faster
                        int x = - divisor[j] * coef;
                        //std::cout << "i + j ="  << i + j << std::endl;
                        msg_out[i + j] += x;
                        //std::cout << "i + j ="  << i + j << std::endl;
                    }
                }
            }
        }

        for (int i = 0; i < msg_out.size(); i++) {
            if (i < msg_out.size() - (divisor.size() - 1))
                quotient.push_back(msg_out[i]);
            else
                remainder.push_back(msg_out[i]);
        }
    }

    // Calculate the polynomial in the field given by the value x
    /* @brief Evaluation of polynomial in x
     * @param &p - polynomial to evaluate
     * @param x  - evaluation point */
    int gf_poly_eval(std::vector<int>& poly, int x) {
        int y = poly[0];

        for (int i = 1; i < poly.size(); i++) {
            y = mult(y, x) ^ poly[i];
        }
        return y;
    }



    
    ///**********************************************************************************
    ///****************************extra    functions************************************
    ///**********************************************************************************



    uint8_t exp_h[512]; //For efficiency, gf_exp[] has size 2 * GF_SIZE, so that a simple multiplication of two numbers can be resolved without calling % 255
    uint8_t log_h[256];

    //Here we use the multiplication algorithm according to the method of "Russian peasants" to find a non-reduced polynomial
    /* @brief Multiplication in Galua Fields
         * @param x - left operand
         * @param y - rifht operand
         * @param prim - primitive binary polynomial
         * @param field_charac_full - the number of elements in Galois field
         * @return x * y */
    int gf_mult(int x, int y, int prim = 0, int field_charac_full=256) {
        int res = 0;

        while (y) {
            if ((y & 1) == 1) {
                res = res ^ x;
            }
            y = y >> 1; //~ y/2
            x = x << 1; //~ x*2

            if ((prim > 0) && (x & field_charac_full)) { // if x ~ 256 use xor
                x = x ^ prim;
            }
            
        }

        return res;
    }


    /* @brief Finding a list of all simple polynomials for a given generator(2) and the characteristic exponent of the Galois field(8)
         * @param c_exp - Galois field exponent
         * @return [irreducible polynomial] */
    std::vector<int> find_prime_polys(int c_exp = 8) {
        int root_charac = 2;
        int field_charac = pow(root_charac, c_exp) - 1;             //255
        int field_charac_next = pow(root_charac, c_exp + 1) - 1;    // 511

        std::vector<int> prim_candidates;
        std::vector<int> irreducible_polynomial;
        std::vector<int> seen;

        for (int j = 0; j < field_charac + 1; j++) {
            seen.push_back(0);
        }

        for (int i = field_charac + 2; i < field_charac_next; i += root_charac) { //we need the number > 256 to avoid repetitions
            prim_candidates.push_back(i);                                         // try every simple polynomial, excluding even ones
        }

        //Here is implemented a bruteforce approach to find all these prime polynomials, by generating every possible 
        //prime polynomials(ie, every integers between field_charac + 1 and field_charac * 2), and then we build 
        //the whole Galois Field, and we reject the candidate prime polynomial if it duplicates even one value or if it 
        //generates a value above field_charac(ie, cause an overflow).
        for (int i = 0; i < prim_candidates.size(); i++) {
            int prim = prim_candidates[i];
            for (int j = 0; j < field_charac + 1; j++) {
                seen[j] = 0;                                       // specifies whether the value in the field has already been generated (seen[x] ?== 1) or not
            }

            bool conflict = false;                                 // to find out if there was at least 1 conflict
            int x = 1;

            for (int j = 0; j < field_charac; j++) {
                x = gf_mult(x, GENERATOR, prim, field_charac + 1); //compute the next value in the field

                if (x > field_charac || seen[x] == 1) {            //if this number is a duplicate, then we reject it
                    //std::cout << x << std::endl;
                    conflict = true;
                    break;
                }
                else {
                    seen[x] = 1;                                   //remember this value to detect future duplicates
                }
            }

            if (!conflict) {                                       //if there was no conflict, then this is a simple polynomial
                irreducible_polynomial.push_back(prim);
            }

        }

        return irreducible_polynomial;
    }

    /* @brief Calculating the "logarithm" and "exponent" tables for further use in arithmetic operations
         * @param gf_exp - the vector passed by reference, in which the values will be placed exp
         * @param gf_log - the vector passed by reference, in which the values will be placed log
         * @param prim - primitive binary polynomial
         * @param c_exp - Galois field exponent */
    void init_tables(std::vector<int>& gf_exp, std::vector<int>& gf_log, int prim = 285, int c_exp = 8) {
        int field_charac = pow(2, c_exp) - 1;        //255

        for (int i = 0; i < field_charac * 2; i++) { //initialization
            gf_exp.push_back(0);
        }
        for (int i = 0; i < field_charac + 1; i++) {
            gf_log.push_back(0);
        }

        //For each element from the Galois field, we calculate the log and exp
        int x = 1;
        for (int i = 0; i < field_charac; i++) {
            gf_exp[i] = x;
            gf_log[x] = i;
            x = gf_mult(x, GENERATOR, prim, field_charac + 1);
        }

        for (int i = field_charac; i < field_charac * 2; i++) {
            gf_exp[i] = gf_exp[i - field_charac];
        }
    }




};
