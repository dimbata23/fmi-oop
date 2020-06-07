#include <iostream>
#include <cstring>
#include "Utils.h"
#include "CommandHandler.h"
#include "PostImage.h"
#include "PostLink.h"
#include "PostText.h"

static const char UNKNOWN_COMMAND_MSG[] = "Unknown command! Try \n\t$: help\n";

static const char* commandArguments[][6] =
    {
        {"\1", "info"},
        {"\1", "quit"},
        {"\1", "help"},
        {"\4", "<issuer>", "add_user" , "<nickname>", "<age>"},
        {"\4", "<issuer>", "add_moderator" , "<nickname>", "<age>"},
        {"\3", "<issuer>", "remove_user" , "<nickname>"},
        {"\3", "<issuer>", "block" , "<nickname>"},
        {"\4", "<issuer>", "post" , "[image/link/text]", "<content>"},
        {"\3", "<issuer>", "remove_post" , "<post ID>"},
        {"\3", "<issuer>", "view_post" , "<post ID>"},
        {"\3", "<issuer>", "view_all_posts" , "<nickname>"},
        {"\3", "<issuer>", "rename" , "<new nickname>"},
    };


CommandHandler::CommandHandler()
    : arguments(nullptr)
    , numOfArgs(0)
{}


// Notice: Quite a long function, these should be avoided
bool CommandHandler::operator()(System& sys, const char* command)
{
    arguments = splitStringBy(command, ' ');
    if (!arguments) {
        std::cerr << "Something went wrong.. Try again later..\n";
        return false;
    }
    numOfArgs = arguments[0][0];

    if (numOfArgs == commandArguments[0][0][0]) {
        if (!strcmpi(arguments[1], commandArguments[INFO][1])) {
            sys.printInfo();
        } else if (!strcmpi(arguments[1], commandArguments[QUIT][1])) {
            clearMemory();
            return true;
        } else if (!strcmpi(arguments[1], commandArguments[HELP][1])) {
            printHelp();
        } else {
            std::cout << UNKNOWN_COMMAND_MSG;
        }
        clearMemory();
        return false;
    }

    // Now that we know it's neither info nor quit

    if (numOfArgs > 1) {
        if (!strcmpi(arguments[2], commandArguments[ADD_USER][2])) {
            if (numOfArgs == commandArguments[ADD_USER][0][0])
                sys.addUser(arguments[1], User(arguments[3], atoi(arguments[4])));
            else
                printUsage(ADD_USER);
        } else if (!strcmpi(arguments[2], commandArguments[ADD_MODERATOR][2])) {
            if (numOfArgs == commandArguments[ADD_MODERATOR][0][0])
                sys.addUser(arguments[1], Moderator(arguments[3], atoi(arguments[4])));
            else
                printUsage(ADD_MODERATOR);
        } else if (!strcmpi(arguments[2], commandArguments[REMOVE_USER][2])) {
            if (numOfArgs == commandArguments[REMOVE_USER][0][0])
                sys.removeUser(arguments[1], arguments[3]);
            else
                printUsage(REMOVE_USER);
        } else if (!strcmpi(arguments[2], commandArguments[BLOCK][2])) {
            if (numOfArgs == commandArguments[BLOCK][0][0])
                sys.toggleBlockUser(arguments[1], arguments[3]);
            else
                printUsage(BLOCK);
        } else if (!strcmpi(arguments[2], commandArguments[POST][2])) {
            if (numOfArgs > 2) {
                if (!strcmpi(arguments[3], "image")) {
                    if (numOfArgs == 4)
                        sys.addPost(arguments[1], PostImage(arguments[4]));
                    else
                        std::cout << "Usage:  <issuer> post image <imgPath>\n";
                } else if (!strcmpi(arguments[3], "link")) {
                    if (numOfArgs >= 4) {
                        const char* content = (command + strlen(arguments[1]) + strlen(arguments[2]) + strlen(arguments[3]) + strlen(arguments[4]) + 4);
                        sys.addPost(arguments[1], PostLink((numOfArgs > 4 ? content : arguments[4]), arguments[4]));
                    } else {
                        std::cout << "Usage:  <issuer> post link <url> [text]\n";
                    }
                } else if (!strcmpi(arguments[3], "text")) {
                    if (numOfArgs > 3) {
                        const char* content = (command + strlen(arguments[1]) + strlen(arguments[2]) + strlen(arguments[3]) + 3);
                        sys.addPost(arguments[1], PostText((numOfArgs > 3 ? content : "")));
                    } else {
                        std::cout << "Usage:  <issuer> post text <content>\n";
                    }
                } else {
                    std::cout << "Wrong post type! <image/link/text>\n";
                }
            } else {
                printUsage(POST);
            }
        } else if (!strcmpi(arguments[2], commandArguments[REMOVE_POST][2])) {
            if (numOfArgs == 3)
                sys.removePost(arguments[1], atoi(arguments[3]));
            else
                printUsage(REMOVE_POST);
        } else if (!strcmpi(arguments[2], commandArguments[VIEW_POST][2])) {
            if (numOfArgs == 3)
                sys.viewPost(arguments[1], atoi(arguments[3]));
            else
                printUsage(VIEW_POST);
        } else if (!strcmpi(arguments[2], commandArguments[VIEW_ALL_POSTS][2])) {
            if (numOfArgs == 3)
                sys.viewAllPostsBy(arguments[1], arguments[3]);
            else
                printUsage(VIEW_ALL_POSTS);
        } else if (!strcmpi(arguments[2], commandArguments[RENAME][2])) {
            if (numOfArgs == 3)
                sys.changeNickname(arguments[1], arguments[3]);
            else
                printUsage(RENAME);
        } else {
            std::cout << UNKNOWN_COMMAND_MSG;
        }
    } else {
        std::cout << UNKNOWN_COMMAND_MSG;
    }

    clearMemory();
    return false;
}


void CommandHandler::printHelp() const
{
    std::cout
        << " ---------------- Commands ---------------------------------------------- Info -------------------------------\n"
        << "  <issuer> add_user <nickname> <age>\t\t- User <issuer> creates a new user with <nickname> and <age>\n"
        << "  <issuer> add_moderator <nickname> <age>\t- User <issuer> creates a moderator with <nickname> and <age>\n"
        << "  <issuer> remove_user <nickname>\t\t- User <issuer> removes a user with <nickname>\n"
        << "  <issuer> post text <content>\t\t\t- User <issuer> creates a post\n"
        << "  <issuer> post image <imgPath>\t\t\t- User <issuer> creates a post with an image\n"
        << "  <issuer> post link <url> [urlText]\t\t- User <issuer> creates a post with a link\n"
        << "  <issuer> remove_post <post_id>\t\t- User <issuer> removes a post\n"
        << "  <issuer> view_post <post_id>\t\t\t- Create an html view for post with specific id\n"
        << "  <issuer> view_all_posts <nickname>\t\t- Create an html view for all posts by a person\n"
        << "  help\t\t\t\t\t\t- Displays this helpful message\n"
        << "  info\t\t\t\t\t\t- Displays info about the users\n"
        << "  quit\t\t\t\t\t\t- Exits the program\n"
        << " -------------------------------------------------------------------------------------------------------------\n";
}


void CommandHandler::printUsage(Commands cmd) const
{
    std::cout << "Usage:  ";
    short args = commandArguments[cmd][0][0];
    for (short i = 0; i < args; ++i)
        std::cout << commandArguments[cmd][i + 1] << ' ';

    std::cout << std::endl;
}


void CommandHandler::clearMemory()
{
    for (short i = 0; i <= numOfArgs; ++i)
        delete[] arguments[i];

    delete[] arguments;
    arguments = nullptr;
    numOfArgs = 0;
}
