#ifndef __POST_IMAGE_INCLUDED__
#define __POST_IMAGE_INCLUDED__

#include "Post.h"

class PostImage : public Post
{
public:
    PostImage(const char* imageURL);
    virtual ~PostImage() = default;
    virtual void writeInHTMLFile(std::ofstream& file, const char* poster) const override;
    virtual Post* clone() const override;
};

#endif // !__POST_IMAGE_INCLUDED__
