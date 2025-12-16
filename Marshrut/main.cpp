#include <iostream>
#include <locale>
#include <string>
#include <codecvt>
#include "RouteCipher.h"

using namespace std;

// Правильное преобразование string в wstring с поддержкой UTF-8
std::wstring string_to_wstring(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

// Функция для демонстрации работы шифра с обработкой исключений
void check(const wstring& text, const int key)
{
    try {
        wstring cipherText;
        wstring decryptedText;
        
        RouteCipher cipher(key);
        cipherText = cipher.encrypt(text);
        decryptedText = cipher.decrypt(cipherText);
        
        wcout << L"Ключ: " << key << endl;
        wcout << L"Исходный текст: '" << text << L"' (длина: " << text.length() << L")" << endl;
        wcout << L"Зашифрованный текст: '" << cipherText << L"' (длина: " << cipherText.length() << L")" << endl;
        wcout << L"Расшифрованный текст: '" << decryptedText << L"' (длина: " << decryptedText.length() << L")" << endl;
        
        if (text == decryptedText) {
            wcout << L"Успех: тексты совпадают" << endl;
        } else {
            wcout << L"Ошибка: тексты не совпадают" << endl;
        }
    } catch (const cipher_error& e) {
        wstring error_wst = string_to_wstring(e.what());
        wcerr << L"Ошибка шифрования: " << error_wst << endl;
    }
    wcout << endl;
}

int main()
{
    // Устанавливаем русскую локаль
    setlocale(LC_ALL, "ru_RU.UTF-8");
    locale::global(locale("ru_RU.UTF-8"));
    // wcout.imbue(locale("ru_RU.UTF-8"));
    // wcin.imbue(locale("ru_RU.UTF-8"));
    
    wcout << L"=== Шифр табличной маршрутной перестановки с обработкой исключений ===\n" << endl;
    
    // Тестирование различных сценариев
    wcout << L"--- Позитивные тесты ---" << endl;
    check(L"ЗДРАВСТВУЙ", 3);
    check(L"ШИФРОВАНИЕ", 4);
    check(L"ПРОГРАММИРОВАНИЕ", 5);
    
    wcout << L"--- Тесты с пробелами и знаками ---" << endl;
    check(L"ПРИВЕТ МИР!", 3);
    check(L"C++ PROGRAMMING", 4);
    
    wcout << L"--- Негативные тесты ---" << endl;
    
    // Тест с неверным ключом
    wcout << L"Тест 1: Неверный ключ" << endl;
    check(L"ТЕКСТ",1);
    
    // Тест с пустым текстом при шифровании
    wcout << L"\nТест 2: Пустой открытый текст" << endl;
    check(L"",3);
    
    // Тест с пустым текстом при дешифровании
    wcout << L"\nТест 3: Пустой зашифрованный текст" << endl;
    try {
        RouteCipher cipher(3);
        cipher.decrypt(L"");
        wcout << L"ОШИБКА: Исключение не было выброшено!" << endl;
    } catch (const cipher_error& e) {
        wstring error_wst = string_to_wstring(e.what());
        wcout << L"Ошибка шифрования: " << error_wst << endl;
    }
    
	wcout << L"\nТест 4: Большое значение ключа" << endl;
	check(L" ", 2);
    
    wcout << L"Программа завершена.\n";
    return 0;
}