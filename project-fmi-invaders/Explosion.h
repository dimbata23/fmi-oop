#ifndef __EXPLOSION_HEADER_INCLUDED__
#define __EXPLOSION_HEADER_INCLUDED__

#include "Object.h"

enum ExplosionSize
{
    EXPLOSION_TINY = 1,
    EXPLOSION_NORMAL = 3,
    EXPLOSION_LARGE = 5
};

class Explosion : public Object
{
public:
    Explosion(unsigned short x, unsigned short y);
    Explosion(unsigned short x, unsigned short y, ExplosionSize size);
    Explosion(std::ifstream& in);
    ~Explosion();
            
    virtual void update();
    virtual void drawSelf() const;
    void virtual serialize(std::ofstream& out) const;

private:
    ExplosionSize size;     // size of the explosion
    unsigned lifetime;      // amount of ticks the object has existed
};

#endif // !__EXPLOSION_HEADER_INCLUDED__
