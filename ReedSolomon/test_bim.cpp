#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char** argv)
{
    //FILE* f = fopen("text.txt", "rb+");

    //ofstream fs("1.bin", ios::binary);
/*
    std::string str_mess;
    std::stringstream data_s {str_mess};
    std::ifstream is {argv[1], std::ios::binary};


    int x;
    is >> x;
    data_s << x;
    
    std::cout << data_s.str(); 
  */  

    //fclose(f);

    // Открываем файл для чтения в бинарном режиме
    std::ifstream file("../data/photo.ppm", std::ios::binary);

    if (file) {
        // Определяем размер файла
        file.seekg(0, std::ios::end);
        int file_size = file.tellg();
        file.seekg(0, std::ios::beg);

        // Создаем контейнер типа std::vector<char> для хранения содержимого файла
        std::vector<char> buffer(file_size);
   //     std::string data;
        // Читаем содержимое файла в контейнер
        file.read(buffer.data(), file_size);
  //      file.read(data.begin(), file_size);

        // Закрываем файл
        file.close();

        // Выводим содержимое контейнера (можно выполнить любую дополнительную обработку, если необходимо)
        /*for (const char& c : buffer) {
            std::cout << c;
        }*/

        std::cout << "size = " << file_size << std::endl;
//       std::cout << data;
    } else {
        std::cerr << "Ошибка открытия файла" << std::endl;
    }

    return 0;
}
