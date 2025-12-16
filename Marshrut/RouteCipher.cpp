#include "RouteCipher.h"
#include <locale>
#include <vector>
#include <algorithm>

// Валидация ключа
int RouteCipher::getValidKey(const int k)
{
    if (k <= 1) {
        throw cipher_error("Ключ должен быть положительным числом, которое больше 1");
    }
    return k;
}

// Валидация открытого текста
std::wstring RouteCipher::getValidOpenText(const std::wstring& s)
{
    if (s.empty()) {
        throw cipher_error("Открытый текст не может быть пустым");
    }
    return s;
}

// Валидация зашифрованного текста
std::wstring RouteCipher::getValidCipherText(const std::wstring& s)
{
    if (s.empty()) {
        throw cipher_error("Зашифрованный текст не может быть пустым");
    }
    return s;
}

// Конструктор
RouteCipher::RouteCipher(int k)
{
    key = getValidKey(k);
}

// Шифрование
std::wstring RouteCipher::encrypt(const std::wstring& open_text)
{
    std::wstring text = getValidOpenText(open_text);
    
    // Определяем количество строк
    int text_length = text.length();
    int rows = (text_length + key - 1) / key;
    
    // Создаем таблицу для записи
    std::vector<std::vector<wchar_t>> table(rows, std::vector<wchar_t>(key, L' '));
    
    // Записываем текст в таблицу по горизонтали слева направо, сверху вниз
    int index = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < key; j++) {
            if (index < text_length) {
                table[i][j] = text[index++];
            }
        }
    }
    
    // Считываем текст сверху вниз, справа налево
    std::wstring cipher_text;
    for (int j = key - 1; j >= 0; j--) {
        for (int i = 0; i < rows; i++) {
            cipher_text.push_back(table[i][j]);
        }
    }
    
    return cipher_text;
}

// Дешифрование
std::wstring RouteCipher::decrypt(const std::wstring& cipher_text)
{
    std::wstring text = getValidCipherText(cipher_text);
    
    // Определяем количество строк
    int text_length = text.length();
    int rows = (text_length + key - 1) / key;
    
    // Создаем таблицу для чтения
    std::vector<std::vector<wchar_t>> table(rows, std::vector<wchar_t>(key, L' '));
    
    // Записываем зашифрованный текст в таблицу сверху вниз, справа налево
    int index = 0;
    for (int j = key - 1; j >= 0; j--) {
        for (int i = 0; i < rows; i++) {
            if (index < text_length) {
                table[i][j] = text[index++];
            }
        }
    }
    
    // Считываем текст по горизонтали слева направо, сверху вниз
    std::wstring open_text;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < key; j++) {
            open_text.push_back(table[i][j]);
        }
    }
    
    // УДАЛЯЕМ ЛИШНИЕ ПРОБЕЛЫ В КОНЦЕ
    // Находим позицию последнего не-пробельного символа
    size_t last_pos = open_text.find_last_not_of(L' ');
    if (last_pos != std::wstring::npos) {
        open_text = open_text.substr(0, last_pos + 1);
    } else {
        // Если вся строка состоит из пробелов, оставляем пустую строку
        open_text.clear();
    }
    
    return open_text;
}