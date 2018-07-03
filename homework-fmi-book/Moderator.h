#ifndef __MODERATOR_INCLUDED__
#define __MODERATOR_INCLUDED__

#include "User.h"

class Moderator : public User
{
public:
	Moderator(const char* nickname, short age);
	~Moderator();
public:
	virtual bool isAdmin() const;
	virtual bool isModerator() const;
	virtual User* clone() const;
};

#endif // !__MODERATOR_INCLUDED__
