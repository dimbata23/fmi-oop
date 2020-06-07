#include <iostream>
#include <cstring>
#include "Post.h"
#include "Utils.h"
#include "HTMLCode.h"

int Post::currID = 0;

Post::Post(const char* content)
    : content(nullptr)
{
    if (content != nullptr) {
        this->content = new char[strlen(content) + 1];
        strcpy(this->content, content);
    } else {
        throw std::invalid_argument("Post's content cannot be null");
    }

    postDate = time(0);
    id = getNextID();
}


Post::Post(const Post& other)
{
    copyData(other);
}


Post& Post::operator=(const Post& other)
{
    if (this != &other) {
        // probably should check before deleting
        deleteData();
        copyData(other);
    }
    return *this;
}


Post::~Post()
{
    deleteData();
}


bool Post::generateHTMLFile(const char* viewer, const char* poster) const
{
    const char viewTxt[] = "'s view of ";
    const char addTxt[] = "post-";
    const char fileFormat[] = ".html";
    char* idStr = toString(id);
    char* fileName = new (std::nothrow) char[strlen(viewer) + strlen(viewTxt) + strlen(addTxt) + strlen(idStr) + strlen(fileFormat) + 1];
    if (!fileName)
        return false;

    strcpy(fileName, viewer);
    strcat(fileName, viewTxt);
    strcat(fileName, addTxt);
    strcat(fileName, idStr);
    strcat(fileName, fileFormat);
    delete[] idStr;

    std::ofstream out(fileName);
    out << HTML_BEGIN_FILE << viewer << HTML_BEGIN_FILE2;
    writeInHTMLFile(out, poster);
    out << HTML_END;
    if (!out)
        return false;

    out.close();
    delete[] fileName;
    return true;
}


int Post::getId() const
{
    return id;
}


void Post::printDate(std::ofstream& file) const
{
    tm *ltm = localtime(&postDate);
    file << ltm->tm_mday << '/' << ltm->tm_mon + 1 << '/' << ltm->tm_year + 1900 << ' ' << ltm->tm_hour << ':' << ltm->tm_min;
}


void Post::deleteData()
{
    delete[] content;
    content = nullptr;
}


void Post::copyData(const Post& other)
{
    content = new char[strlen(other.content) + 1];
    strcpy(content, other.content);

    this->id = other.id;
    this->postDate = other.postDate;
}


int Post::getNextID() const
{
    return currID++;
}
