#ifndef __UTILS_HEADER_INCLUDED__
#define __UTILS_HEADER_INCLUDED__

// Notice: It's better to create a class containing
//         all these functions as static methods

// Returns a dynamically allocated char* of
// the number converted to a string
char* toString(size_t num);

// Returns the digits count in the given number
size_t countDigits(size_t num);

// Returns the number of characters in
// the given str that are == to ch
int countChar(const char* str, char ch);

// Splits a string by a character (space by default)
// Returns a dynamic char** which must be deleted ;)
// (short)char[0][0] is the number of pieces created
char** splitStringBy(const char* str, char ch = ' ');

// Copies n characters from source to dest
void strcpyn(char* dest, const char* source, size_t n);

// Changes all capital letters in str to lower-case
char* tolower(char* str);

// Compares the strings case insensitively
int strcmpi(const char* str1, const char* str2);

#endif // !__UTILS_HEADER_INCLUDED__
