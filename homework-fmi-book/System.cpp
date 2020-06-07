#include <iostream>
#include "Utils.h"
#include "System.h"
#include "User.h"
#include "Moderator.h"
#include "Administrator.h"

const unsigned short DEFAULT_CAPACITY = 4;
const char DEFAULT_ADMIN_NAME[] = "Admin";
const short DEFAULT_ADMIN_AGE   = 1337;

const char* USER_NOT_FOUND_MSG  = "No user found with nickname ";
const char* POST_NOT_FOUND_MSG  = "No post found with id ";
const char* NO_PERMISSION_MSG   = "You don't have permission to do that!";

System::System()
    : users(nullptr)
    , numOfUsers(1)
    , usersCapacity(DEFAULT_CAPACITY)
    , numOfBlockedUsers(0)
{
    users = new User*[usersCapacity];
    users[0] = new Administrator("Admin", DEFAULT_ADMIN_AGE);
}


System::~System()
{
    deleteData();
}


// Notice: The virtual isAdmin() and isModerator() methods
//         can be avoided by using dynamic_cast<Admin*>
//         and dynamic_cast<Moderator*>


void System::addUser(const char* issuer, const User& user)
{
    User* issuerPtr = getUser(issuer);
    if (issuerPtr && issuerPtr->isAdmin()) {
        if (userExists(user.getNickname())) {
            std::cout << "User already exists!\n";
            return;
        }

        if (numOfUsers >= usersCapacity) {
            if (!resizeUsers()) {
                std::cerr << "Not enough memory to add user!\n";
                return;
            }
        }

        users[numOfUsers] = user.clone();
        std::cout << users[numOfUsers]->getNickname() << " created.\n";
        ++numOfUsers;
    } else {
        if (!issuerPtr)
            std::cout << USER_NOT_FOUND_MSG << issuer << std::endl;
        else
            std::cout << NO_PERMISSION_MSG;
    }
}


void System::removeUser(const char* issuer, const char* target)
{
    User* issuerPtr = getUser(issuer);
    if (!issuerPtr) {
        std::cout << USER_NOT_FOUND_MSG << issuer << std::endl;
        return;
    }

    if (!issuerPtr->isAdmin()) {
        std::cout << NO_PERMISSION_MSG;
        return;
    }

    int targetIndex = getUserIndex(target);
    if (targetIndex == -1) {
        std::cout << USER_NOT_FOUND_MSG << target << std::endl;
        return;
    }

    delete users[targetIndex];
    users[targetIndex] = users[numOfUsers - 1];
    --numOfUsers;

    std::cout << target << " removed.\n";
}


void System::toggleBlockUser(const char* issuer, const char* target) 
{
    User* issuerPtr = getUser(issuer);
    if (!issuerPtr) {
        std::cout << USER_NOT_FOUND_MSG << issuer << std::endl;
        return;
    }

    User* targetPtr = getUser(target);
    if (!targetPtr) {
        std::cout << USER_NOT_FOUND_MSG << target << std::endl;
        return;
    }

    if (issuerPtr->isModerator()) {
        if (targetPtr->toggleBlock()) {
            ++numOfBlockedUsers;
            std::cout << target << " blocked.\n";
        } else {
            --numOfBlockedUsers;
            std::cout << target << " unblocked.\n";
        }
    } else {
        std::cout << NO_PERMISSION_MSG << std::endl;
    }
}


void System::addPost(const char* issuer, const Post& post)
{
    User* issuerPtr = getUser(issuer);
    if (!issuerPtr) {
        std::cout << USER_NOT_FOUND_MSG << issuer << std::endl;
        return; 
    }

    if (issuerPtr->addPost(post))
        std::cout << "Post " << post.getId() << " created.\n";
}


void System::printInfo() const
{
    std::cout
        << " ------- System info -------\n"
        << "There" << (numOfUsers == 1 ? " is " : " are ") << numOfUsers << " user" << (numOfUsers == 1 ? "" : "s") << ":\n";

    printUsersInfo();
    int bUsers = numOfBlockedUsers;
    if (!bUsers) {
        std::cout << "There aren't any blocked users.\n";
    } else {
        std::cout << "There" << (bUsers == 1 ? " is " : " are ") << bUsers << " blocked user" << (bUsers == 1 ? "" : "s") << ":\n";
        printBlockedUsers();
    }

    sortUsersByAge();
    if (numOfUsers > 1)
        std::cout
        << "Oldest user is " << users[numOfUsers - 1]->getNickname() << ' ' << users[numOfUsers - 1]->getAge() << "\n"
        << "Youngest user is " << users[1]->getNickname() << ' ' << users[1]->getAge() << "\n";
    else
        std::cout
        << "Oldest and youngest user would be the Admin at the age of "
        << users[0]->getAge() << '\n';
    std::cout
        << " ---------------------------\n";
}


void System::printUsersInfo() const
{
    for (int i = 0; i < numOfUsers; ++i)
        users[i]->print();
}


void System::printBlockedUsers() const
{
    for (int i = 0; i < numOfUsers; ++i)
        if (users[i]->isBlocked())
            std::cout << '\t' << users[i]->getNickname() << '\n';
}

void System::sortUsersByAge() const
{
    for (int i = 1; i < numOfUsers - 1; ++i)
        if (users[i]->getAge() > users[i + 1]->getAge())
            std::swap(users[i], users[i + 1]);
}

void System::removePost(const char* issuer, int postID) const
{
    User* issuerPtr = getUser(issuer);
    if (!issuerPtr) {
        std::cout << USER_NOT_FOUND_MSG << issuer << std::endl;
        return;
    }

    int postIndex = getPostIndex(postID);
    if (postIndex == -1) {
        std::cout << POST_NOT_FOUND_MSG << postID << std::endl;
        return;
    }

    User* userPtr = nullptr;
    for (int i = 0; i < numOfUsers; ++i) {
        if (users[i]->containsPost(postID)) {
            userPtr = users[i];
        }
    }

    if (issuerPtr->isModerator() || issuerPtr->containsPost(postID))
        if (userPtr->removePost(postIndex))
            std::cout << "Post " << postID << " removed." << std::endl;
        else
            std::cout << "Couldn't remoev post " << postID << '.' << std::endl;
    else
        std::cout << NO_PERMISSION_MSG << std::endl;
}


void System::viewPost(const char* issuer, int postID) const
{
    User* issuerPtr = getUser(issuer);
    if (!issuerPtr) {
        std::cout << USER_NOT_FOUND_MSG << issuer << std::endl;
        return;
    }

    Post* postPtr = getPost(postID);
    if (!postPtr) {
        std::cout << POST_NOT_FOUND_MSG << postID << std::endl;
        return;
    }

    User* userPtr = nullptr;
    for (int i = 0; i < numOfUsers; ++i) {
        if (users[i]->containsPost(postID)) {
            userPtr = users[i];
        }
    }

    if (postPtr->generateHTMLFile(issuerPtr->getNickname(), userPtr->getNickname()))
        std::cout << "HTML view for post " << postID << " created.\n";
    else
        std::cerr << "Couldn't generate post " << postID << "!\n";
}


void System::viewAllPostsBy(const char* issuer, const char* target) const
{
    User* issuerPtr = getUser(issuer);
    if (!issuerPtr) {
        std::cout << USER_NOT_FOUND_MSG << issuer << std::endl;
        return;
    }

    User* targetPtr = getUser(target);
    if (!targetPtr) {
        std::cout << USER_NOT_FOUND_MSG << target << std::endl;
        return;
    }

    if (targetPtr->createViewFor(issuerPtr->getNickname()))
        std::cout << "HTML view for all " << issuerPtr->getNickname() << "'s posts created.\n";
    else
        std::cout << "Couldn't created HTML view for all " << target << "'s posts.\n";
}


void System::changeNickname(const char* issuer, const char* newNickname) const
{
    User* issuerPtr = getUser(issuer);
    if (!issuerPtr) {
        std::cout << USER_NOT_FOUND_MSG << issuer << std::endl;
        return;
    }

    if (userExists(newNickname)) {
        std::cout << "User " << newNickname << " already exists!" << std::endl;
        return;
    }

    if (issuerPtr->changeNickname(newNickname))
        std::cout << issuer << " is now known as " << newNickname << std::endl;
    else
        std::cout << "Couldn't change " << issuer << "'s nickname!" << std::endl;
}


User* System::getUser(const char* nickname) const
{
    for (int i = 0; i < numOfUsers; ++i)
        if (!strcmpi(nickname, users[i]->getNickname()))
            return users[i];
    return nullptr;
}


Post* System::getPost(int ID) const
{
    Post* result = nullptr;
    for (int i = 0; i < numOfUsers; ++i) {
        result = users[i]->getPost(ID);
        if (result)
            break;
    }
    return result;
}


int System::getPostIndex(int ID) const
{
    for (int i = 0; i < numOfUsers; ++i)
        if (users[i]->getPost(ID))
            return i;
    return -1;
}


int System::getUserIndex(const char* nickname) const
{
    for (int i = 0; i < numOfUsers; ++i)
        if (!strcmpi(nickname, users[i]->getNickname()))
            return i;
    return -1;
}


bool System::userExists(const char* nickname) const
{
    return getUser(nickname);
}

void System::deleteData()
{
    for (int i = 0; i < numOfUsers; ++i)
        delete users[i];
    delete[] users;
    users = nullptr;
}


bool System::resizeUsers()
{
    int newCapacity = usersCapacity * 2;
    User** temp = new (std::nothrow) User*[newCapacity];
    while (!temp) {
        newCapacity -= (newCapacity - usersCapacity) / 2;
        temp = new (std::nothrow) User*[newCapacity];
        if (!temp && newCapacity == usersCapacity + 1)
            return false;
    }
    usersCapacity = newCapacity;

    for (int i = 0; i < numOfUsers; ++i)
        temp[i] = users[i];
    delete[] users;
    users = temp;

    return true;
}
