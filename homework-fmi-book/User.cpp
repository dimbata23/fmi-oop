#include <iostream>
#include <cstring>
#include "User.h"

const unsigned short DEFAULT_CAPACITY = 4;

User::User(const char* nickname, short age)
    : posts(nullptr)
    , nickname(nullptr)
    , numOfPosts(0)
    , postsCapacity(DEFAULT_CAPACITY)
    , age(age)
    , blocked(false)
{
    if (!nickname)
        throw std::invalid_argument("User cannot be constructed with a null name!");

    if (age <= 0)
        throw std::invalid_argument("User cannot be constructed with a non-positive integer!");

    this->nickname = new char[strlen(nickname) + 1];
    strcpy(this->nickname, nickname);
    posts = new Post*[postsCapacity];
}


User::User(const User& other)
{
    copyData(other);
}


User& User::operator=(const User& other)
{
    if (this != &other) {
        // should probably check if there's enough memory before deleting..
        deleteData();
        copyData(other);
    }
    return *this;
}


User::~User()
{
    deleteData();
}


bool User::toggleBlock()
{
    return blocked = !blocked;
}


bool User::isBlocked()
{
    return blocked;
}


void User::print() const
{
    std::cout
        << '\t' << nickname << " - "
        << (isAdmin() ? "Administrator" : (isModerator() ? "Moderator" : "User"))
        << ", " << numOfPosts << " posts.\n";
}


const char* User::getNickname() const
{
    return nickname;
}


short User::getAge() const
{
    return age;
}


bool User::addPost(const Post& post)
{
    if (numOfPosts >= postsCapacity) {
        if (!resizePosts()) {
            std::cerr << "Not enough memory to add post!\n";
            return false;
        }
    }

    posts[numOfPosts] = post.clone();
    ++numOfPosts;
    return true;
}


Post* User::getPost(int ID) const
{
    for (int i = 0; i < numOfPosts; ++i)
        if (posts[i]->getId() == ID)
            return posts[i];

    return nullptr;
}


bool User::containsPost(int ID) const
{
    return getPost(ID);
}


bool User::removePost(int postIndex)
{
    delete posts[postIndex];
    posts[postIndex] = posts[numOfPosts - 1];
    --numOfPosts;
    return true;
}


bool User::createViewFor(const char* viewer)
{
    const char viewTxt[] = "'s view of all of ";
    const char addText[] = "'s posts.html";
    char* fileName = new (std::nothrow) char[strlen(viewer) + strlen(viewTxt) + strlen(this->nickname) + strlen(addText) + 1];
    if (!fileName) {
        std::cerr << "Not enough memory.. Couldn't generate the file...\n";
        return false;
    }

    strcpy(fileName, viewer);
    strcat(fileName, viewTxt);
    strcat(fileName, this->nickname);
    strcat(fileName, addText);

    std::ofstream out(fileName);
    if (!out)
        return false;

    out << HTML_BEGIN_FILE << viewer << HTML_BEGIN_FILE2;
    for (int i = 0; i < numOfPosts; ++i)
            posts[i]->writeInHTMLFile(out, this->nickname);

    out << HTML_END;
    out.close();
    delete[] fileName;

    return true;
}


bool User::changeNickname(const char* nickname)
{
    char* temp = new (std::nothrow) char[strlen(nickname) + 1];
    if (!temp)
        return false;

    strcpy(temp, nickname);
    delete[] this->nickname;
    this->nickname = temp;
    return true;
}


bool User::isAdmin() const
{
    return false;
}


bool User::isModerator() const
{
    return false;
}


User* User::clone() const
{
    return new (std::nothrow) User(*this);
}


bool User::resizePosts()
{
    int newCapacity = postsCapacity * 2;
    Post** temp = new (std::nothrow) Post*[newCapacity];
    while (!temp) {
        newCapacity -= (newCapacity - postsCapacity) / 2;
        temp = new (std::nothrow) Post*[newCapacity];
        if (!temp && newCapacity <= postsCapacity + 1)
            return false;
    }
    postsCapacity = newCapacity;

    for (int i = 0; i < numOfPosts; ++i)
        temp[i] = posts[i];

    delete[] posts;
    posts = temp;

    return true;
}


void User::copyData(const User& other)
{
    nickname = new char[strlen(other.nickname) + 1];

    strcpy(nickname, other.nickname);
    age = other.age;
    blocked = other.blocked;
    numOfPosts = other.numOfPosts;
    postsCapacity = other.postsCapacity;
    posts = new Post*[postsCapacity];

    for (int i = 0; i < numOfPosts; ++i)
        posts[i] = other.posts[i]->clone();
}


void User::deleteData()
{
    delete[] nickname;
    nickname = nullptr;
    for (int i = 0; i < numOfPosts; ++i)
        delete posts[i];

    delete[] posts;
    posts = nullptr;
}
