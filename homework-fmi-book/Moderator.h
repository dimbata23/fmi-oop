#ifndef __MODERATOR_INCLUDED__
#define __MODERATOR_INCLUDED__

#include "User.h"

class Moderator : public User
{
public:
    Moderator(const char* nickname, short age);
    virtual ~Moderator() = default;

public:
    virtual bool isAdmin() const override;
    virtual bool isModerator() const override;
    virtual User* clone() const override;
};

#endif // !__MODERATOR_INCLUDED__
