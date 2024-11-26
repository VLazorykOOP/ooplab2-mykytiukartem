#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include <cstdint>

struct EncodedChar {
    unsigned int position : 7;  // 0-6 біти
    unsigned int ascii_code : 8; // 7-14 біти
    unsigned int parity : 1;    // 15 біт
};

union EncodedUnion {
    EncodedChar fields;
    uint16_t full;
};

uint8_t calculate_parity(uint16_t value) {
    uint8_t parity = 0;
    while (value) {
        parity ^= (value & 1);
        value >>= 1;
    }
    return parity;
}

int main() {
    std::ifstream input_file("encoded2.bin", std::ios::binary);
    if (!input_file) {
        std::cerr << "Не вдалося відкрити файл encrypted.bin\n";
        return 1;
    }

    std::string decrypted_text(128, ' ');
    for (size_t i = 0; i < 128; ++i) {
        uint16_t encoded_data;
        input_file.read(reinterpret_cast<char*>(&encoded_data), sizeof(encoded_data));
        if (!input_file) {
            std::cerr << "Помилка читання з файлу encoded2.bin\n";
            return 1;
        }

        EncodedUnion encoded2;
        encoded2.full = encoded_data;

        // Перевірка біта парності
        uint8_t calculated_parity = calculate_parity(encoded2.full & 0x7FFF);
        if (calculated_parity != encoded2.fields.parity) {
            std::cerr << "Помилка парності у символі з позицією " << encoded2.fields.position << "\n";
            return 1;
        }

        decrypted_text[encoded2.fields.position] = static_cast<char>(encoded2.fields.ascii_code);
    }

    input_file.close();

    std::ofstream output_file("output2.txt");
    if (!output_file) {
        std::cerr << "Не вдалося відкрити файл output.txt для запису\n";
        return 1;
    }

    output_file << decrypted_text;
    output_file.close();

    std::cout << "Розшифрований текст: \n" << decrypted_text << "\n";
    std::cout << "Текст збережено у output2.txt\n";

    return 0;
}
