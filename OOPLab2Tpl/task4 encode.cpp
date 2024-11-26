#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <cstring>
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
    std::string input_text;
    std::ifstream input_file("input.txt");
    if (!input_file) {
        std::cerr << "Не вдалося відкрити файл input.txt\n";
        return 1;
    }

    std::getline(input_file, input_text);
    input_file.close();

    // Доповнюємо текст до 128 символів пробілами
    if (input_text.size() < 128) {
        input_text.append(128 - input_text.size(), ' ');
    } else {
        input_text = input_text.substr(0, 128);
    }

    std::ofstream output_file("encoded4.bin", std::ios::binary);
    if (!output_file) {
        std::cerr << "Не вдалося відкрити файл для запису encrypted.bin\n";
        return 1;
    }

    for (size_t i = 0; i < input_text.size(); ++i) {
        EncodedUnion encoded;
        encoded.fields.position = i;

        // Міняємо місцями старші і молодші 4 біти
        encoded.fields.ascii_code = swap_nibbles(static_cast<uint8_t>(input_text[i]));

        // Обчислюємо біт парності
        encoded.fields.parity = calculate_parity(encoded.full & 0x7FFF);

        output_file.write(reinterpret_cast<const char*>(&encoded.full), sizeof(encoded.full));
    }

    output_file.close();
    std::cout << "Текст успішно зашифровано і записано в encoded4.bin\n";

    return 0;
}
