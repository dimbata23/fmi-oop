#ifndef __POST_TEXT_INCLUDED__
#define __POST_TEXT_INCLUDED__

#include "Post.h"

class PostText : public Post
{
public:
	PostText(const char* content);
	virtual ~PostText();
	virtual void writeInHTMLFile(std::ofstream& file, const char* poster) const;
	virtual Post* clone() const;
protected:
	virtual void copyData(const PostText&);
	virtual void deleteData();
};

#endif // !__POST_IMAGE_INCLUDED__
