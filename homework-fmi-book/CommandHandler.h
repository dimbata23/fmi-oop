#ifndef __COMMAND_HANDLER_INCLUDED__
#define __COMMAND_HANDLER_INCLUDED__

#include "System.h"

// Notice: It'd better if this enum
//         was part of the class
enum Commands
{
    INFO,
    QUIT,
    HELP,
    ADD_USER,
    ADD_MODERATOR,
    REMOVE_USER,
    BLOCK,
    POST,
    REMOVE_POST,
    VIEW_POST,
    VIEW_ALL_POSTS,
    RENAME
};


class CommandHandler
{
public:
    CommandHandler();
    bool operator()(System& sys, const char* command);
    void printHelp() const;
    void printUsage(Commands) const;

private:
    CommandHandler(const CommandHandler&) = delete;
    CommandHandler& operator=(const CommandHandler&) = delete;
    void clearMemory();

private:
    char** arguments;
    short numOfArgs;
};

#endif // !__COMMAND_HANDLER_INCLUDED__
