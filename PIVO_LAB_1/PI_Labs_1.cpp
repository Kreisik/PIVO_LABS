#include <iostream>
#include <cmath> // Использовал для pow
#include <string> // Использовал для строк
#include <algorithm> // Использовал для поворота строки (reverse)

using namespace std;

int main() {

    std::string digits_main = ""; // Строка для целых значения
    std::string digits_decimal = ""; // Строка для десятичных
    double result = 0;
    double number;
    int base;
    int count = 0;
    cout << "Enter decimal number: ";
    cin >> number;
    cout << "Enter basis: ";
    cin >> base;
    int main_part = static_cast<int>(number); // Отделяет часть до точки
    double additional_part = number - main_part; // То что осталось после точки

    if (number < 0) {

        cout << "Your number is negative";
        return 0;

    }

    if (additional_part == 0) {

        if (base == 2) {

            while (main_part >= 1) {

                int remainder = main_part % 2; // Остаток от деления
                digits_main += std::to_string(remainder); // Записываю этот остаток в строку
                main_part /= 2;
                result += pow(2, count) * remainder; // Сразу плюсую в результат по схеме Горнера
                count ++;
            }

        } else if (base == 8) {

            while (main_part >= 1) {

                int remainder = main_part % 8;
                digits_main += std::to_string(remainder);
                main_part /= 8;
                result += pow(8, count) * remainder;
                count ++;

            }

        } else if (base == 16) {

            while (main_part >= 1) {

                int remainder = main_part % 16;
                if (remainder <= 9) { // Если число до 9 включительно

                    digits_main += std::to_string(remainder);
                    result += pow(16, count) * remainder;
                    count ++;

                } else {

                    char letter = static_cast<char>(55 + remainder); // Перевожу число по номеру к буквам по ASCII
                    digits_main += letter;
                    result += pow(16, count) * remainder;
                    count ++;
                }
                main_part /= 16;

            }

        } else {
            cout << "Wrong base!";
            return 0;
        }

        std::reverse(digits_main.begin(), digits_main.end()); // Переворачиваю список на 180 градусов
        cout << "Result in " << base << " system: " << digits_main << endl;
        cout << "Return to decimal system: " << result;

    } else {

        if (base == 2) {

            while (main_part >= 1) {

                int remainder = main_part % 2;
                digits_main += std::to_string(remainder);
                main_part /= 2;
                result += pow(2, count) * remainder;
                count ++;
            }

            count = 1;

            while (additional_part != 1) {

                additional_part *= 2;
                int remainder = static_cast<int>(additional_part); // Нахожу целую часть уже в части после точки
                result += pow(2, -count) * remainder; // Записываю опять же число по схеме Горнера
                count ++;
                additional_part = additional_part - remainder; // Отделяю целую часть
                digits_decimal += std::to_string(remainder);
                if (additional_part == 0) { // Если доп часть равна нулю, то команда заверашет этот этап)

                    break;

                }

            }

        } else if (base == 8) {

            while (main_part >= 1) {

                int remainder = main_part % 8;
                digits_main += std::to_string(remainder);
                main_part /= 8;
                result += pow(8, count) * remainder;
                count ++;
            }

            count = 1;

            while (additional_part != 1) {

                additional_part *= 8;
                int remainder = static_cast<int>(additional_part);
                result += pow(8, -count) * remainder;
                count ++;
                additional_part = additional_part - remainder;
                digits_decimal += std::to_string(remainder);
                if (additional_part == 0) {

                    break;

                }

            }

        } else if (base == 16) {

            while (main_part >= 1) {

                int remainder = main_part % 16;
                if (remainder <= 9) {

                    digits_main += std::to_string(remainder);
                    result += pow(16, count) * remainder;
                    count ++;

                } else {

                    char letter = static_cast<char>(55 + remainder);
                    digits_main += letter;
                    result += pow(16, count) * remainder;
                    count ++;

                }
                main_part /= 16;

            }

            count = 1;

            while (additional_part != 1) {

                additional_part *= 16;
                int remainder = static_cast<int>(additional_part);
                if (remainder <= 9) {

                    digits_decimal += std::to_string(remainder);
                    result += pow(16, -count) * remainder;
                    count ++;

                } else {

                    char letter = static_cast<char>(55 + remainder);
                    digits_decimal += letter;
                    result += pow(16, -count) * remainder;
                    count ++;

                }
                additional_part = additional_part - remainder;
                if (additional_part == 0) {

                    break;

                }

            }

        } else {
            cout << "Wrong base!";
            return 0;
        }

        std::reverse(digits_main.begin(), digits_main.end());
        if (main_part == 0) {

            cout << "Result in " << base << " system: " << "0" << digits_main << "." << digits_decimal << endl;
            cout << "Return to decimal system: " << result;

        } else {

            cout << "Result in " << base << " system: " << digits_main << "." << digits_decimal << endl;
            cout << "Return to decimal system: " << result;

        }


    }


}