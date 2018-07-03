#include "Administrator.h"



Administrator::Administrator(const char* nickname, short age) : Moderator(nickname, age) {}


Administrator::~Administrator() {}

bool Administrator::isAdmin() const { return true; }

bool Administrator::isModerator() const { return true; }

User* Administrator::clone() const { return new (std::nothrow) Administrator(*this); }
