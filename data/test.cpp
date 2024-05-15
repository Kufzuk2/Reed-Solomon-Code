#include <iostream>
#include <opencv2/opencv.hpp>

void distortImage(const std::string& input_file, const std::string& output_file) {
    // Загрузка изображения
    cv::Mat img = cv::imread(input_file);
    
    if (img.empty()) {
        std::cerr << "Не удалось загрузить изображение." << std::endl;
        return;
    }
    
    // Применение искажения (например, добавление шума)
    // В этом примере я добавлю случайный шум
    cv::Mat noise(img.size(), CV_8UC3);
    cv::randn(noise, cv::Scalar::all(0), cv::Scalar::all(50));
    cv::Mat distorted_img = img + noise;
    
    // Сохранение искаженного изображения
    cv::imwrite(output_file, distorted_img);
    
    std::cout << "Искаженное изображение сохранено в " << output_file << std::endl;
}

int main() {
    std::string input_file = "photo.ppm";
    std::string output_file = "new_photo.ppm";
    
    distortImage(input_file, output_file);
    
    return 0;
}
