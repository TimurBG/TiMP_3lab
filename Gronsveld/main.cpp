#include <iostream>
#include <locale>
#include <string>
#include <codecvt>
#include "modAlphaCipher.h"

// Правильное преобразование string в wstring с поддержкой UTF-8
std::wstring string_to_wstring(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

void check(const std::wstring& Text, const std::wstring& key, const bool destructCipherText = false) {
    try {
        std::wstring cipherText;
        std::wstring decryptedText;
        modAlphaCipher cipher(key);
            
        cipherText = cipher.encrypt(Text);
        
        // Опционально "портим" зашифрованный текст для тестирования
        if (destructCipherText && !cipherText.empty()) {
            cipherText[0] = L'z'; // Меняем первый символ
        }
        
        decryptedText = cipher.decrypt(cipherText);
            
        std::wcout << L"Ключ: " << key << std::endl;
        std::wcout << L"Исходный текст: " << Text << std::endl;
        std::wcout << L"Зашифрованный текст: " << cipherText << std::endl;
        std::wcout << L"Расшифрованный текст: " << decryptedText << std::endl;
            
        if (Text == decryptedText) {
            std::wcout << L"Успешно!\n";
        } else {
            std::wcout << L"Ошибка!\n";
        }
    } catch (const cipher_error& e) {
        std::wstring error_wst = string_to_wstring(e.what());
        std::wcerr << L"Ошибка: " << error_wst << std::endl;
    }
    std::wcout << std::endl;
}

int main() {
    // Установка глобальной локали
    std::locale loc("ru_RU.UTF-8");
    std::locale::global(loc);

    // Тесты с корректными данными
    std::wcout << L"=== Тесты с корректными данными ===" << std::endl;
    check(L"ПРИВЕТМИР", L"Я");
    check(L"ТЕСТИРОВАНИЕ", L"АБВГД");
    check(L"ПРОГРАММИРОВАНИЕ", L"СИШП");
    check(L"АЛГОРИТМЫ", L"КЛЮЧЕВОЙ");
    check(L"ДАПРОСТОТЕКСТ", L"ДА");

    // Тесты с исключениями
    std::wcout << L"=== Тесты с исключениями ===" << std::endl;
    
    // Пустой ключ
    check(L"ПРИВЕТ", L"");
    
    // Ключ с недопустимыми символами
    check(L"ПРИВЕТ", L"КЛЮЧ123");
    
    // Пустой открытый текст
    check(L"", L"КЛЮЧ");
    
    // Открытый текст только с не-буквами
    check(L"123!@#", L"КЛЮЧ");
    
    // "Испорченный" зашифрованный текст
    check(L"ПРИВЕТ", L"КЛЮЧ", true);
    
    return 0;
}