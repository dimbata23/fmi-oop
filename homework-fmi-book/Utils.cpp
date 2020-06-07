#include <iostream>
#include <cstring>
#include "Utils.h"


char* toString(size_t num)
{
    int numOfDigits = countDigits(num);
    char* result = new (std::nothrow) char[numOfDigits + 1];
    if (result) {
        result[numOfDigits] = '\0';
        for (size_t i = numOfDigits; i > 0; --i) {
            result[i - 1] = '0' + num % 10;
            num /= 10;
        }
    }
    return result;
}


size_t countDigits(size_t num)
{
    size_t result = 1;
    while (num /= 10)
        ++result;
    return result;
}


int countChar(const char* str, char ch)
{
    int result = 0;

    while (*str) {
        if (*str == ch)
            ++result;
        ++str;
    }

    return result;
}


char** splitStringBy(const char* str, char ch)
{
    short pieces = countChar(str, ch) + 1;
    char** result = new (std::nothrow) char*[pieces + 1];
    if (!result)
        return nullptr;

    result[0] = new (std::nothrow) char[2];
    if (!result[0]) {
        delete[] result;
        return nullptr;
    }
    result[0][0] = pieces;
    result[0][1] = '\0';

    for (short i = 1; i <= pieces; ++i) {
        const char* currStartPos = str;
        while (*str && *str != ch)
            ++str;

        short c = str - currStartPos;
        result[i] = new (std::nothrow) char[c + 1];
        if (!result[i]) {
            while (i > 0)
                delete[] result[--i];
            delete[] result;
            return nullptr;
        }

        strcpyn(result[i], currStartPos, c);
        result[i][c] = '\0';
        if (*str)
            ++str;
    }

    return result;
}


void strcpyn(char* dest, const char* source, size_t n)
{
    while (*source && n > 0) {
        *dest++ = *source++;
        --n;
    }
}


char* tolower(char* str)
{
    char* res = str;

    while (*str) {
        if (*str >= 'A' && *str <= 'Z')
            *str += 'a' - 'A';
        ++str;
    }

    return res;
}


int strcmpi(const char* str1, const char* str2)
{
    char* lstr1 = new char[strlen(str1) + 1];
    char* lstr2 = new char[strlen(str2) + 1];
    strcpy(lstr1, str1);
    strcpy(lstr2, str2);

    int res = strcmp(tolower(lstr1), tolower(lstr2));
    delete[] lstr1;
    delete[] lstr2;

    return res;
}
