#ifndef __UTILS_HEADER_INCLUDED__
#define __UTILS_HEADER_INCLUDED__

char* toString(size_t num);
size_t countDigits(size_t num);
int countChar(const char* str, char ch);
// Splits a string by a character (space by default)
// Returns a dynamic char** which must be deleted ;)
// (short)char[0][0] is the number of pieces created
char** splitStringBy(const char* str, char ch = ' ');

#endif // !__UTILS_HEADER_INCLUDED__
