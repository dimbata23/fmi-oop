#include "PostImage.h"


PostImage::PostImage(const char* imageURL)
    : Post(imageURL)
{}


void PostImage::writeInHTMLFile(std::ofstream& file, const char* poster) const
{
    file << HTML_POST_BEGIN << poster << HTML_POST_CONTINUE << "posted an image"
        << HTML_POST_CONTINUE2;
    printDate(file);
    file << HTML_POST_CONTINUE3
        << "<img src=\"" << content << "\">"
        << HTML_POST_END;
}


Post* PostImage::clone() const
{
    return new (std::nothrow) PostImage(*this);
}
