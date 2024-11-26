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

// Функція для обміну старших і молодших 4 бітів
uint8_t swap_nibbles(uint8_t value) {
    return (value << 4) | (value >> 4);
}

uint8_t calculate_parity(uint16_t value) {
    uint8_t parity = 0;
    while (value) {
        parity ^= (value & 1);
        value >>= 1;
    }
    return parity;
}

int main() {
    std::ifstream input_file("encoded4.bin", std::ios::binary);
    if (!input_file) {
        std::cerr << "Не вдалося відкрити файл encrypted.bin\n";
        return 1;
    }

    std::string decrypted_text(128, ' ');
    for (size_t i = 0; i < 128; ++i) {
        uint16_t encoded_data;
        input_file.read(reinterpret_cast<char*>(&encoded_data), sizeof(encoded_data));
        if (!input_file) {
            std::cerr << "Помилка читання з файлу encrypted.bin\n";
            return 1;
        }

        EncodedUnion encoded;
        encoded.full = encoded_data;

        // Перевірка біта парності
        uint8_t calculated_parity = calculate_parity(encoded.full & 0x7FFF);
        if (calculated_parity != encoded.fields.parity) {
            std::cerr << "Помилка парності у символі з позицією " << encoded.fields.position << "\n";
            return 1;
        }

        // Зворотний обмін старших і молодших 4 бітів
        decrypted_text[encoded.fields.position] = static_cast<char>(swap_nibbles(encoded.fields.ascii_code));
    }

    input_file.close();

    std::ofstream output_file("output4.txt");
    if (!output_file) {
        std::cerr << "Не вдалося відкрити файл output4.txt для запису\n";
        return 1;
    }

    output_file << decrypted_text;
    output_file.close();

    std::cout << "Розшифрований текст: \n" << decrypted_text << "\n";
    std::cout << "Текст збережено у output4.txt\n";

    return 0;
}
