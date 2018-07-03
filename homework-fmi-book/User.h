#ifndef __USER_HEADER_INCLUDED__
#define __USER_HEADER_INCLUDED__

#include "Post.h"

static size_t CURRENT_USERS;

class User
{
public:
	User(const char* nickname, short age);
	User(const User& other);
	User& operator=(const User& other);
	~User();
public:
	bool toggleBlock();
	bool isBlocked();
	void print() const;
	const char* getNickname() const;
	short getAge() const;
	bool addPost(const Post&);
	Post* getPost(int ID) const;
	bool containsPost(int ID) const;
	bool removePost(int PostIndex);
	bool createViewFor(const char* viewer);
	bool changeNickname(const char* nickname);
	virtual bool isAdmin() const;
	virtual bool isModerator() const;
	virtual User* clone() const;
protected:
	bool resizePosts();
	bool copyData(const User& other);
	void deleteData();
protected:
	Post** posts;
	char* nickname;
	int numOfPosts;
	int postsCapacity;
	short age;
	bool blocked;
};

#endif
