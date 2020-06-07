#ifndef __POST_TEXT_INCLUDED__
#define __POST_TEXT_INCLUDED__

#include "Post.h"

class PostText : public Post
{
public:
    PostText(const char* content);
    virtual ~PostText() = default;
    virtual void writeInHTMLFile(std::ofstream& file, const char* poster) const override;
    virtual Post* clone() const override;
};

#endif // !__POST_IMAGE_INCLUDED__
