#ifndef __SYSTEM_HEADER_INCLUDED__
#define __SYSTEM_HEADER_INCLUDED__

#include "User.h"
#include "Moderator.h"
#include "Administrator.h"
//#include "Post.h"
//#include "PostLink.h"
//#include "PostText.h"
//#include "PostImage.h"

class System
{
public:
	System();
	~System();
	void addUser(const char* issuer, const User&);
	void removeUser(const char* issuer, const char* target);
	void toggleBlockUser(const char* issuer, const char* target);
	void addPost(const char* issuer, const Post&) const;
	void printInfo() const;
	void printUsersInfo() const;
	void printBlockedUsers() const;
	void sortUsersByAge() const;
	void removePost(const char* issuer, int postID) const;
	void viewPost(const char* issuer, int postID) const;
	void viewAllPostsBy(const char* issuer, const char* target) const;
	void changeNickname(const char* issuer, const char* newNickname) const;
private:
	System(const System&) = delete;
	System& operator=(const System&) = delete;
private:
	User* getUser(const char*) const;
	Post* getPost(int ID) const;
	int getPostIndex(int ID) const;
	int getUserIndex(const char*) const;
	bool userExists(const char*) const;
	void deleteData();
	bool resizeUsers();
private:
	User** users;
	int numOfUsers;
	int usersCapacity;
	int numOfBlockedUsers;
};

#endif
