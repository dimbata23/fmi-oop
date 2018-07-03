#ifndef __POST_HEADER_INCLUDED__
#define __POST_HEADER_INCLUDED__

#include <fstream>
#include <ctime>
#include "HTMLCode.h"

class Post
{
public:
	Post(const char* content);
	Post(const Post&);
	Post& operator=(const Post&);
	virtual ~Post();
public:
	virtual void writeInHTMLFile(std::ofstream& file, const char* poster) const = 0;
	bool generateHTMLFile(const char* viewer, const char* poster) const;
	int getId() const;
	virtual Post* clone() const = 0;
	//int getIssuerId() const;
protected:
	char* content;
	time_t postDate;
	//int issuerId;
	int id;
protected:
	virtual void copyData(const Post& other);
	virtual void deleteData();
	void printDate(std::ofstream& file) const;
private:
	int getNextID() const;
	static int currID;
};

#endif // !__POST_HEADER_INCLUDED__
