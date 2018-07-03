#include "Moderator.h"



Moderator::Moderator(const char* nickname, short age) : User(nickname, age) {}


Moderator::~Moderator() {}

bool Moderator::isAdmin() const { return false; }

bool Moderator::isModerator() const { return true; }

User* Moderator::clone() const { return new (std::nothrow) Moderator(*this); }
