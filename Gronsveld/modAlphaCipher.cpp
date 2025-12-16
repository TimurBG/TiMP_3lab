#include "modAlphaCipher.h"
#include <algorithm>
#include <locale>

// Конструктор
modAlphaCipher::modAlphaCipher(const std::wstring& skey) {
    for (unsigned i = 0; i < numAlpha.size(); i++) {
        alphaNum[numAlpha[i]] = i;
    }
    key = convert(getValidKey(skey));

    size_t n = 0;
    for (auto e:key) {
        if (e==0) {
            n++;
        }
    }
    if (2*n > key.size()) {
        throw cipher_error("WeakKey");
    }
}

// Метод шифрования
std::wstring modAlphaCipher::encrypt(const std::wstring& open_text) {
    std::vector<int> work = convert(getValidOpenText(open_text));
    for (unsigned i = 0; i < work.size(); i++) {
        work[i] = (work[i] + key[i % key.size()]) % alphaNum.size();
    }
    return convert(work);
}

// Метод дешифрования
std::wstring modAlphaCipher::decrypt(const std::wstring& cipher_text) {
    std::vector<int> work = convert(getValidCipherText(cipher_text));
    for (unsigned i = 0; i < work.size(); i++) {
        work[i] = (work[i] + alphaNum.size() - key[i % key.size()]) % alphaNum.size();
    }
    return convert(work);
}

// Конвертация строки в числа
std::vector<int> modAlphaCipher::convert(const std::wstring& s) {
    std::vector<int> result;
    for (auto c : s) {
        result.push_back(alphaNum[c]);
    }
    return result;
}

// Конвертация чисел в строку
std::wstring modAlphaCipher::convert(const std::vector<int>& v) {
    std::wstring result;
    for (auto i : v) {
        result.push_back(numAlpha[i]);
    }
    return result;
}

// Валидация ключа
std::wstring modAlphaCipher::getValidKey(const std::wstring& s) {
    if (s.empty()) {
        throw cipher_error("Пустой ключ");
    }
    
    std::wstring tmp(s);
    for (auto& c : tmp) {
        if (numAlpha.find(c) == std::wstring::npos) {
            throw cipher_error("Недопустимый символ в ключе");
        }
    }
    return tmp;
}

// Валидация открытого текста
std::wstring modAlphaCipher::getValidOpenText(const std::wstring& s) {
    std::wstring tmp;
    
    // Удаляем все символы, не являющиеся русскими буквами
    for (auto c : s) {
        if (numAlpha.find(c) != std::wstring::npos) {
            tmp.push_back(c);
        }
    }
    
    if (tmp.empty()) {
        throw cipher_error("Пустой открытый текст после удаления не-букв");
    }
    
    return tmp;
}

// Валидация зашифрованного текста
std::wstring modAlphaCipher::getValidCipherText(const std::wstring& s) {
    if (s.empty()) {
        throw cipher_error("Пустой зашифрованный текст");
    }
    
    for (auto c : s) {
        if (numAlpha.find(c) == std::wstring::npos) {
            throw cipher_error("Недопустимый символ в зашифрованном тексте");
        }
    }
    
    return s;
}