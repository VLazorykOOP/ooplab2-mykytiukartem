#include <iostream>
#include <fstream>
#include <string>

std::string encrypt(const std::string& text) {
    std::string encrypted = text;
    for (char& c : encrypted) {
        unsigned char upper = (c & 0xF0) >> 4; // Старші 4 біти
        unsigned char lower = (c & 0x0F) << 4; // Молодші 4 біти
        c = upper | lower; // Комбінуємо
    }
    return encrypted;
}

std::string decrypt(const std::string& encrypted) {
    return encrypt(encrypted); // Процес шифрування і розшифрування однаковий
}

int main() {
    std::string inputFile = "input.txt";
    std::string encryptedFile = "encrypted.txt";
    std::string decryptedFile = "decrypted.txt";

    // Зчитування тексту з файлу
    std::ifstream inFile(inputFile);
    if (!inFile) {
        std::cerr << "Error: Could not open input file." << std::endl;
        return 1;
    }

    std::string text((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    // Шифрування тексту
    std::string encrypted = encrypt(text);

    // Запис зашифрованого тексту у файл
    std::ofstream outEncryptedFile(encryptedFile);
    if (!outEncryptedFile) {
        std::cerr << "Error: Could not open encrypted file." << std::endl;
        return 1;
    }
    outEncryptedFile << encrypted;
    outEncryptedFile.close();

    // Розшифрування тексту
    std::string decrypted = decrypt(encrypted);

    // Запис розшифрованого тексту у файл
    std::ofstream outDecryptedFile(decryptedFile);
    if (!outDecryptedFile) {
        std::cerr << "Error: Could not open decrypted file." << std::endl;
        return 1;
    }
    outDecryptedFile << decrypted;
    outDecryptedFile.close();

    std::cout << "Encryption and decryption completed.\n"
              << "Encrypted text saved to: " << encryptedFile << "\n"
              << "Decrypted text saved to: " << decryptedFile << std::endl;

    return 0;
}
