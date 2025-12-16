#include "modAlphaCipher.h"
#include <UnitTest++/UnitTest++.h>
#include <string>
#include <iostream>

// ==================== ТЕСТЫ КОНСТРУКТОРА ====================

SUITE(ConstructorTests) {
    // Тест 1.1: Верный ключ
    TEST(ValidKey) {
        modAlphaCipher cipher(L"БВГ");
        std::wstring result = cipher.encrypt(L"ААААА");
        CHECK(result == L"БВГБВ");
    }
    
    // Тест 1.2: Ключ длиннее сообщения
    TEST(LongKey) {
        modAlphaCipher cipher(L"БВГДЕЁЖЗИЙК");
        std::wstring result = cipher.encrypt(L"ААААА");
        CHECK(result == L"БВГДЕ");
    }
    
    // Тест 1.3: В ключе строчные буквы - ДОЛЖНО БЫТЬ ИСКЛЮЧЕНИЕ
    TEST(LowercaseKey) {
        CHECK_THROW(modAlphaCipher cipher(L"бвг"), cipher_error);
    }
    
    // Тест 1.4: В ключе цифры - ДОЛЖНО БЫТЬ ИСКЛЮЧЕНИЕ
    TEST(DigitsInKey) {
        CHECK_THROW(modAlphaCipher cipher(L"Б1В"), cipher_error);
    }
    
    // Тест 1.5: В ключе знаки препинания - ДОЛЖНО БЫТЬ ИСКЛЮЧЕНИЕ
    TEST(PunctuationInKey) {
        CHECK_THROW(modAlphaCipher cipher(L"Б,В"), cipher_error);
    }
    
    // Тест 1.6: В ключе пробелы - ДОЛЖНО БЫТЬ ИСКЛЮЧЕНИЕ
    TEST(WhitespaceInKey) {
        CHECK_THROW(modAlphaCipher cipher(L"Б В"), cipher_error);
    }
    
    // Тест 1.7: Пустой ключ - ДОЛЖНО БЫТЬ ИСКЛЮЧЕНИЕ
    TEST(EmptyKey) {
        CHECK_THROW(modAlphaCipher cipher(L""), cipher_error);
    }
    
    // Тест 1.8: Вырожденный ключ (ААА) - ДОЛЖНО БЫТЬ ИСКЛЮЧЕНИЕ
    TEST(WeakKey) {
        CHECK_THROW(modAlphaCipher cipher(L"ААА"), cipher_error);
    }
}

// ==================== ТЕСТЫ МЕТОДА ENCRYPT ====================

// Фикстура для тестов с ключом "Б"
struct KeyB_Fixture {
    modAlphaCipher* p;
    
    KeyB_Fixture() {
        p = new modAlphaCipher(L"Б");  // Ключ "Б" (сдвиг 1)
    }
    
    ~KeyB_Fixture() {
        delete p;
    }
};

SUITE(EncryptTests) {
    // Тест 2.1: Строка из прописных букв
    TEST_FIXTURE(KeyB_Fixture, UpperCaseString) {
        std::wstring result = p->encrypt(L"ПРИВЕТМИР");
        std::wstring expected = L"РСЙГЁУНЙС";  // Рассчитано заранее
        CHECK(result == expected);
    }
    
    // Тест 2.2: Строка из строчных букв - ДОЛЖНО БЫТЬ ИСКЛЮЧЕНИЕ
    TEST_FIXTURE(KeyB_Fixture, LowerCaseString) {
        CHECK_THROW(p->encrypt(L"приветмир"), cipher_error);
    }
    
    // Тест 2.3: Строка с пробелами и знаками препинания
    TEST_FIXTURE(KeyB_Fixture, StringWithWhitespaceAndPunctuation) {
        std::wstring result = p->encrypt(L"ПРИВЕТ, МИР!");
        std::wstring expected = L"РСЙГЁУНЙС";  // Без знаков препинания
        CHECK(result == expected);
    }
    
    // Тест 2.4: Строка с цифрами
    TEST_FIXTURE(KeyB_Fixture, StringWithNumbers) {
        std::wstring result = p->encrypt(L"СНОВЫМ2025ГОДОМ");
        std::wstring expected = L"ТОПГЬНДПЕПН";  // Без цифр
        CHECK(result == expected);
    }
    
    // Тест 2.5: Пустой текст - ДОЛЖНО БЫТЬ ИСКЛЮЧЕНИЕ
    TEST_FIXTURE(KeyB_Fixture, EmptyString) {
        CHECK_THROW(p->encrypt(L""), cipher_error);
    }
    
    // Тест 2.6: Нет букв - ДОЛЖНО БЫТЬ ИСКЛЮЧЕНИЕ
    TEST_FIXTURE(KeyB_Fixture, NoAlphaString) {
        CHECK_THROW(p->encrypt(L"1234+8765=9999"), cipher_error);
    }
    
    // Тест 2.7: Максимальный сдвиг (ключ "Я")
    TEST(MaxShiftKey) {
        modAlphaCipher cipher(L"Я");  // Сдвиг 32 = -1 mod 33
        std::wstring result = cipher.encrypt(L"ПРИВЕТМИР");
        std::wstring expected = L"ОПЗБДСЛЗП";  // П-1=О, Р-1=П, и т.д.
        CHECK(result == expected);
    }
}

// ==================== ТЕСТЫ МЕТОДА DECRYPT ====================

SUITE(DecryptTests) {
    // Используем ту же фикстуру с ключом "Б"
    
    // Тест 3.1: Строка из прописных букв
    TEST_FIXTURE(KeyB_Fixture, UpperCaseString) {
        std::wstring encrypted = L"РСЙГЁУНЙС";
        std::wstring result = p->decrypt(encrypted);
        std::wstring expected = L"ПРИВЕТМИР";
        CHECK(result == expected);
    }
    
    // Тест 3.2: Есть строчные буквы - ДОЛЖНО БЫТЬ ИСКЛЮЧЕНИЕ
    TEST_FIXTURE(KeyB_Fixture, LowerCaseString) {
        CHECK_THROW(p->decrypt(L"рсйгёунйс"), cipher_error);
    }
    
    // Тест 3.3: Есть пробелы - ДОЛЖНО БЫТЬ ИСКЛЮЧЕНИЕ
    TEST_FIXTURE(KeyB_Fixture, WhitespaceString) {
        CHECK_THROW(p->decrypt(L"РСЙ ГЁУ НЙС"), cipher_error);
    }
    
    // Тест 3.4: Есть цифры - ДОЛЖНО БЫТЬ ИСКЛЮЧЕНИЕ
    TEST_FIXTURE(KeyB_Fixture, DigitsString) {
        CHECK_THROW(p->decrypt(L"РСЙГЁУ123НЙС"), cipher_error);
    }
    
    // Тест 3.5: Есть знаки препинания - ДОЛЖНО БЫТЬ ИСКЛЮЧЕНИЕ
    TEST_FIXTURE(KeyB_Fixture, PunctuationString) {
        CHECK_THROW(p->decrypt(L"РСЙГЁУ,НЙС!"), cipher_error);
    }
    
    // Тест 3.6: Пустой текст - ДОЛЖНО БЫТЬ ИСКЛЮЧЕНИЕ
    TEST_FIXTURE(KeyB_Fixture, EmptyString) {
        CHECK_THROW(p->decrypt(L""), cipher_error);
    }
    
    // Тест 3.7: Максимальный сдвиг (ключ "Я")
    TEST(MaxShiftKey) {
        modAlphaCipher cipher(L"Я");
        std::wstring encrypted = L"ОПЗБДСЛЗП";
        std::wstring result = cipher.decrypt(encrypted);
        std::wstring expected = L"ПРИВЕТМИР";
        CHECK(result == expected);
    }
}


// ==================== ГЛАВНАЯ ФУНКЦИЯ ====================

int main() {
    std::cout << "=== Модульное тестирование шифра Гронсфельда ===" << std::endl;
    std::cout << "Язык: C++, Фреймворк: UnitTest++" << std::endl;
    std::cout << "================================================" << std::endl;
    
    return UnitTest::RunAllTests();
}
