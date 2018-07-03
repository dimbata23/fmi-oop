#ifndef __ADMINISTRATOR_INCLUDED__
#define __ADMINISTRATOR_INCLUDED__

#include "Moderator.h"

class Administrator : public Moderator
{
public:
	Administrator(const char* nickname, short age);
	~Administrator();
public:
	virtual bool isAdmin() const;
	virtual bool isModerator() const;
	virtual User* clone() const;
};


#endif // !__ADMINISTRATOR_INCLUDED__
