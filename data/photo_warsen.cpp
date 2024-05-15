
#if 0

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Структура для представления пикселя
struct Pixel {
    unsigned char r, g, b;
};

int main() {
    // Открыть файл .ppm для чтения
    ifstream inputFile("photo.ppm", ios::binary);
    
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
    vector<Pixel> pixels(width * height);
    
    // Считать пиксели изображения
    inputFile.read(reinterpret_cast<char*>(pixels.data()), pixels.size() * sizeof(Pixel));
    
    // Закрыть файл после чтения
    inputFile.close();
    
    // Изменить качество изображения (пример искажения)
    for (auto& pixel : pixels) {
        // Пример: уменьшение значения красного канала на 10
        if (pixel.r >= 30) {
            pixel.r -= 30;
        } else {
            pixel.r = 0;
        }
    }
    
    for (Pixel& pixel : pixels) {
        // Пример: уменьшение значения красного канала на 10
        if (pixel.g >= 30) {
            pixel.g -= 30;
        } else {
            pixel.g = 0;
        }
    }

    for (Pixel& pixel : pixels) {
        // Пример: уменьшение значения красного канала на 10
        pixel.b = 0;
    }

    // Открыть файл для записи
    ofstream outputFile("output_image.ppm", ios::binary);
    
    // Записать заголовок файла PPM
    outputFile << format << "\n" << width << " " << height << "\n" << maxColorValue << "\n";
    
    // Записать пиксели изображения
    outputFile.write(reinterpret_cast<char*>(pixels.data()), pixels.size() * sizeof(Pixel));
    
    // Закрыть файл после записи
    outputFile.close();
    
    cout << "Изображение успешно искажено и сохранено в файл output_image.ppm" << endl;
    
    return 0;
}


#endif





#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>

using namespace std;

// Структура для представления пикселя
struct Pixel {
    unsigned char r, g, b;
};

int main() {
    // Открыть файл .ppm для чтения
    ifstream inputFile("photo.ppm", ios::binary);
    
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
    vector<Pixel> pixels(width * height);
    
    // Считать пиксели изображения
    inputFile.read(reinterpret_cast<char*>(pixels.data()), pixels.size() * sizeof(Pixel));
    
    // Закрыть файл после чтения
    inputFile.close();
    
    // Изменить качество изображения (случайное количество пикселей подвергаются изменению)
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, pixels.size() - 1); // Диапазон случайных пикселей
    
    int numPixelsToDistort = pixels.size() / 10; // Пример: изменить 10% пикселей
    
    for (int i = 0; i < numPixelsToDistort; ++i) {
        int index = dis(gen); // Получить случайный индекс пикселя
        pixels[index].r = 0;
        pixels[index].g = 0;
        pixels[index].b = 0;
    }
    
    // Открыть файл для записи
    ofstream outputFile("output_image.ppm", ios::binary);
    
    // Записать заголовок файла PPM
    outputFile << format << "\n" << width << " " << height << "\n" << maxColorValue << "\n";
    
    // Записать пиксели изображения
    outputFile.write(reinterpret_cast<char*>(pixels.data()), pixels.size() * sizeof(Pixel));
    
    // Закрыть файл после записи
    outputFile.close();
    
    cout << "Изображение успешно искажено и сохранено в файл output_image.ppm" << endl;
    
    return 0;
}

