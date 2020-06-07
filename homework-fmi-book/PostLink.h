#ifndef __POST_LINK_INCLUDED__
#define __POST_LINK_INCLUDED__

#include "Post.h"

class PostLink : public Post
{
public:
    PostLink(const char* content, const char* url);
    PostLink(const PostLink&);
    virtual ~PostLink();
    virtual void writeInHTMLFile(std::ofstream& file, const char* poster) const override;
    virtual Post* clone() const override;

protected:
    char* url;

private:
    void copyData(const PostLink&);
    void deleteData();
};

#endif // !__POST_IMAGE_INCLUDED__
