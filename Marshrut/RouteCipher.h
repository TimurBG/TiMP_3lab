#pragma once
#include <string>
#include <stdexcept>

// Класс исключения для шифра
class cipher_error : public std::invalid_argument {
public:
    explicit cipher_error(const std::string& what_arg) : 
        std::invalid_argument(what_arg) {}
    explicit cipher_error(const char* what_arg) : 
        std::invalid_argument(what_arg) {}
};

class RouteCipher
{
private:
    int key; // количество столбцов таблицы
    
    // Методы валидации
    int getValidKey(const int k);
    std::wstring getValidOpenText(const std::wstring& s);
    std::wstring getValidCipherText(const std::wstring& s);
    
public:
    RouteCipher() = delete; // запрещаем конструктор без параметров
    RouteCipher(int k); // конструктор с ключом
    
    // Методы шифрования и дешифрования
    std::wstring encrypt(const std::wstring& open_text);
    std::wstring decrypt(const std::wstring& cipher_text);
};