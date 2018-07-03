#ifndef __POST_LINK_INCLUDED__
#define __POST_LINK_INCLUDED__

#include "Post.h"

class PostLink : public Post
{
public:
	PostLink(const char* content, const char* url);
	PostLink(const PostLink&);
	virtual ~PostLink();
	virtual void writeInHTMLFile(std::ofstream& file, const char* poster) const;
	virtual Post* clone() const;
protected:
	virtual void copyData(const PostLink&);
	virtual void deleteData();
protected:
	char* url;
};

#endif // !__POST_IMAGE_INCLUDED__
