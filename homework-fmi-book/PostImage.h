#ifndef __POST_IMAGE_INCLUDED__
#define __POST_IMAGE_INCLUDED__

#include "Post.h"

class PostImage : public Post
{
public:
	PostImage(const char* imageURL);
	virtual ~PostImage();
	virtual void writeInHTMLFile(std::ofstream& file, const char* poster) const;
	virtual Post* clone() const;
protected:
	virtual void copyData(const PostImage&);
	virtual void deleteData();
};

#endif // !__POST_IMAGE_INCLUDED__
