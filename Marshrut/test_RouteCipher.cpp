#include "RouteCipher.h"
#include <UnitTest++/UnitTest++.h>
#include <iostream>

// ==================== ТЕСТЫ КОНСТРУКТОРА ====================

SUITE(ConstructorTests) {
    // Тест 1.1: Корректный ключ > 1
    TEST(ValidKeyGreaterThanOne) {
        RouteCipher cipher(3);
        std::wstring result = cipher.encrypt(L"ТЕСТ");
        CHECK(result != L"");
    }
    
    // Тест 1.2: Минимальный допустимый ключ
    TEST(ValidKeyEqualsTwo) {
        RouteCipher cipher(2);
        std::wstring result = cipher.encrypt(L"ТЕСТ");
        CHECK(result != L"");
    }
    
    // Тест 1.3: Ключ = 1 - ДОЛЖНО БЫТЬ ИСКЛЮЧЕНИЕ
    TEST(InvalidKeyEqualsOne) {
        CHECK_THROW(RouteCipher cipher(1), cipher_error);
    }
    
    // Тест 1.4: Ключ = 0 - ДОЛЖНО БЫТЬ ИСКЛЮЧЕНИЕ
    TEST(InvalidKeyEqualsZero) {
        CHECK_THROW(RouteCipher cipher(0), cipher_error);
    }
    
    // Тест 1.5: Отрицательный ключ - ДОЛЖНО БЫТЬ ИСКЛЮЧЕНИЕ
    TEST(InvalidNegativeKey) {
        CHECK_THROW(RouteCipher cipher(-5), cipher_error);
    }
    
    // Тест 1.6: Очень большой ключ
    TEST(VeryLargeKey) {
        RouteCipher cipher(1000);
        std::wstring result = cipher.encrypt(L"ТЕСТ");
        CHECK(result != L"");
    }
}

// ==================== ТЕСТЫ МЕТОДА ENCRYPT ====================

// Фикстура для тестов с ключом 3
struct Key3Fixture {
    RouteCipher* p;
    
    Key3Fixture() {
        p = new RouteCipher(3);
    }
    
    ~Key3Fixture() {
        delete p;
    }
};

// Фикстура для тестов с ключом 5
struct Key5Fixture {
    RouteCipher* p;
    
    Key5Fixture() {
        p = new RouteCipher(5);
    }
    
    ~Key5Fixture() {
        delete p;
    }
};

SUITE(EncryptTests) {
    // Тест 2.1: Без дополнения (текст кратен ключу)
    TEST_FIXTURE(Key3Fixture, NoPaddingTextMultipleOfKey) {
        std::wstring result = p->encrypt(L"ПРИВЕТМИР");
        CHECK(result == L"ИТРРЕИПВМ");
    }
    
    // Тест 2.2: С дополнением пробелами
    TEST_FIXTURE(Key5Fixture, WithPaddingSpaces) {
        std::wstring result = p->encrypt(L"ПРИВЕТ");
        CHECK(result == L"Е В И Р ПТ");
    }
    
    // Тест 2.3: Один символ, большой ключ
    TEST(OneCharacterLargeKey) {
        RouteCipher cipher(10);
        std::wstring result = cipher.encrypt(L"А");
        CHECK(result == L"         А");
    }
    
    // Тест 2.4: Текст с собственным пробелом
    TEST_FIXTURE(Key3Fixture, TextWithOwnSpace) {
        std::wstring result = p->encrypt(L"ПРИ МИР");
        CHECK(result == L"ИИ РМ П Р");
    }
    
    // Тест 2.5: Английский текст, некратный ключу
    TEST(EnglishTextNotMultiple) {
        RouteCipher cipher(4);
        std::wstring result = cipher.encrypt(L"HELLO");
        CHECK(result == L"L L E HO");
    }
    
    // Тест 2.6: Минимальный текст и ключ
    TEST(MinimalTextAndKey) {
        RouteCipher cipher(2);
        std::wstring result = cipher.encrypt(L"АБ");
        CHECK(result == L"БА");
    }
    
    // Тест 2.7: Все символы - пробелы
    TEST_FIXTURE(Key3Fixture, AllSpaces) {
        std::wstring result = p->encrypt(L"   ");
        CHECK(result == L"   ");
    }
    
    // Тест 2.8: пустой текст - ДОЛЖНО БЫТЬ ИСКЛЮЧЕНИЕ
    TEST_FIXTURE(Key3Fixture, EmptyTextException) {
        CHECK_THROW(p->encrypt(L""), cipher_error);
    }
}

// ==================== ТЕСТЫ МЕТОДА DECRYPT ====================

SUITE(DecryptTests) {
    // Тест 3.1: Без пробелов-заполнителей
    TEST_FIXTURE(Key3Fixture, NoPaddingSpaces) {
        std::wstring result = p->decrypt(L"ИТРРЕИПВМ");
        CHECK(result == L"ПРИВЕТМИР");
    }
    
    // Тест 3.2: С пробелами-заполнителями
    TEST_FIXTURE(Key5Fixture, WithPaddingSpaces) {
        std::wstring result = p->decrypt(L"Е В И Р ПТ");
        CHECK(result == L"ПРИВЕТ");
    }
    
    // Тест 3.3: Пробел внутри шифротекста
    TEST_FIXTURE(Key3Fixture, SpaceInsideCipherText) {
        std::wstring result = p->decrypt(L"ИИ РМ П Р");
        CHECK(result == L"ПРИ МИР");
    }
    
    // Тест 3.4: Только пробелы-заполнители (один символ)
    TEST(OnlyPaddingSpacesOneChar) {
        RouteCipher cipher(10);
        std::wstring result = cipher.decrypt(L"         А");
        CHECK(result == L"А");
    }
    
    // Тест 3.5: Все символы - пробелы
    TEST_FIXTURE(Key3Fixture, AllSpacesDecrypt) {
        std::wstring result = p->decrypt(L"   ");
        CHECK(result == L"");
    }
    
    // Тест 3.6: Пробелы в середине и конце
    TEST(SpacesMiddle) {
        RouteCipher cipher(4);
        std::wstring result = cipher.decrypt(L"L L E HO");
        CHECK(result == L"HELLO");
    }
    
    // Тест 3.7: Крайний случай: один символ
    TEST(OneCharacterCase) {
        RouteCipher cipher(2);
        std::wstring result = cipher.decrypt(L" А");
        CHECK(result == L"А");
    }
    
    // Тест 3.8: пустой шифротекст - ДОЛЖНО БЫТЬ ИСКЛЮЧЕНИЕ
    TEST_FIXTURE(Key3Fixture, EmptyCipherTextException) {
        CHECK_THROW(p->decrypt(L""), cipher_error);
    }
}

// ==================== ИНТЕГРАЦИОННЫЕ ТЕСТЫ ====================

SUITE(IntegrationTests) {
    // Проверка симметрии: encrypt -> decrypt
    TEST(SymmetryTestMultipleOfKey) {
        RouteCipher cipher(3);
        std::wstring original = L"ПРИВЕТМИР";
        std::wstring encrypted = cipher.encrypt(original);
        std::wstring decrypted = cipher.decrypt(encrypted);
        CHECK(original == decrypted);
    }
    
    TEST(SymmetryTestWithPadding) {
        RouteCipher cipher(5);
        std::wstring original = L"ПРИВЕТ";
        std::wstring encrypted = cipher.encrypt(original);
        std::wstring decrypted = cipher.decrypt(encrypted);
        CHECK(original == decrypted);
    }
    
    TEST(SymmetryTestWithSpaces) {
        RouteCipher cipher(3);
        std::wstring original = L"ПРИ МИР";
        std::wstring encrypted = cipher.encrypt(original);
        std::wstring decrypted = cipher.decrypt(encrypted);
        CHECK(original == decrypted);
    }
}

// ==================== ГЛАВНАЯ ФУНКЦИЯ ====================

int main() {
    std::cout << "=== Модульное тестирование шифра табличной маршрутной перестановки ===" << std::endl;
    std::cout << "Язык: C++, Фреймворк: UnitTest++" << std::endl;
    std::cout << "================================================================" << std::endl;
    
    return UnitTest::RunAllTests();
}
