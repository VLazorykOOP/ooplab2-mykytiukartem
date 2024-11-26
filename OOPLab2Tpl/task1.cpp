#include <iostream>

int main() {
    int a, b, c, d;
    std::cout << "Введіть значення a, b, c та d: ";
    std::cin >> a >> b >> c >> d;

    // Обчислення виразу:
    // 17*a + 312*c переведемо в суму та зсуви
    // 17 * a = (16 * a + a) => (a << 4) + a
    // 312 * c = (256 * c + 32 * c + 16 * c) => (c << 8) + (c << 5) + (c << 4)
    // Ділення на 32 - це зсув вправо на 5
    int numerator = ((a << 4) + a) + ((c << 8) + (c << 5) + (c << 4));
    int firstPart = numerator >> 5;

    // b * 120 = (64 * b + 32 * b + 16 * b + 8 * b) => (b << 6) + (b << 5) + (b << 4) + (b << 3)
    int bPart = (b << 6) + (b << 5) + (b << 4) + (b << 3);

    // d * 127 = (128 * d - d) => (d << 7) - d
    int dPart = (d << 7) - d;

    // Повний вираз
    int result = firstPart - bPart + dPart;

    std::cout << "Результат обчислення виразу: " << result << "\n";

    return 0;
}
