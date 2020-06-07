#include "PostText.h"


PostText::PostText(const char* content)
    : Post(content)
{}


void PostText::writeInHTMLFile(std::ofstream& file, const char* poster) const {
    file << HTML_POST_BEGIN << poster << HTML_POST_CONTINUE << "posted"
         << HTML_POST_CONTINUE2;
    printDate(file);
    file << HTML_POST_CONTINUE3
         << content
         << HTML_POST_END;
}


Post* PostText::clone() const
{
    return new (std::nothrow) PostText(*this);
}
