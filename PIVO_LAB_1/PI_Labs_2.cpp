#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

int hexCharToValue(char c) { // Фукнция для перевода букв обратно в код ASCII
    if (c >= '0' && c <= '9') return c - '0';
    return c - 'A' + 10;
}

int main() {

    string input;
    cout << "Enter something: ";
    cin >> input;
    string save = "";
    int count = 0;

    for (int i = 0; i < input.length(); i++) {
        string gap; // Создаю промежуточную строку
        int code = (static_cast<unsigned char>(input[i])); // Создаю переменную которая будет получать значение не -127 -128 а от 0 до 255
        while (code >= 1) {

            int remainder = code % 16;
            if (remainder <= 9) {

                gap += std::to_string(remainder);

            } else {

                char letter = static_cast<char>(55 + remainder);
                gap += letter;
            }

            code /= 16;

        }

        std::reverse(gap.begin(), gap.end());
        save += gap;
        save += " ";
        count++;

    }

    cout << "Coded: " << save << endl;
    cout << "Spending bite: " << count << endl;

    string decoded = "";
    string token = "";
    for (int i = 0; i < save.length(); i++) {

        if (save[i] == ' ') { // Когда символ равен пробелу

            if (!token.empty()) { // Проверка на пустоту токена

                int result = hexCharToValue(token[0]) * 16 + hexCharToValue(token[1]); // Получаю число путем декодирования обратно
                decoded += static_cast<char>(result);
                token = "";

            }

        } else { // Если символ не пробел, то я добавляю к нему число из кодировки
            token += save[i];
        }

    }

    cout << "Decoding: " << decoded;

}