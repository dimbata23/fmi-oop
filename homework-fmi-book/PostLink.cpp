#include <cstring>
#include "PostLink.h"


PostLink::PostLink(const char* content, const char* url)
    : Post(content)
    , url(nullptr)
{
    if (url != nullptr) {
        this->url = new char[strlen(url) + 1];
        strcpy(this->url, url);
    } else {
        throw std::invalid_argument("Post's url cannot be null");
    }
}


PostLink::PostLink(const PostLink& other)
    : Post(other)
{
    copyData(other);
}


PostLink::~PostLink()
{
    deleteData();
}


void PostLink::writeInHTMLFile(std::ofstream& file, const char* poster) const
{
    file << HTML_POST_BEGIN << poster << HTML_POST_CONTINUE << "posted a link"
        << HTML_POST_CONTINUE2;
    printDate(file);
    file << HTML_POST_CONTINUE3
        << "<a href=\"" << url << "\">" << content << "</a>"
        << HTML_POST_END;
}


Post* PostLink::clone() const
{
    return new (std::nothrow) PostLink(*this);
}


void PostLink::copyData(const PostLink& other)
{
    this->url = new char[strlen(other.url) + 1];
    strcpy(this->url, other.url);
}


void PostLink::deleteData()
{
    delete[] url;
    url = nullptr;
}
