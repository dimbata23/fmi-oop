#ifndef __ADMINISTRATOR_INCLUDED__
#define __ADMINISTRATOR_INCLUDED__

#include "Moderator.h"

class Administrator : public Moderator
{
public:
    Administrator(const char* nickname, short age);
    virtual ~Administrator() = default;

public:
    virtual bool isAdmin() const override;
    virtual bool isModerator() const override;
    virtual User* clone() const override;
};

#endif // !__ADMINISTRATOR_INCLUDED__
