#ifndef __HTML_CODE_HEADER_INCLUDED__
#define __HTML_CODE_HEADER_INCLUDED__

// Notice: It's better to create a class containing
//         all these as public static constants

const char HTML_BEGIN_FILE[] =
"<!DOCTYPE html>                                                                                                                                                                                                                                               \n"
"<html>                                                                                                                                                                                                                                                        \n"
"<head>                                                                                                                                                                                                                                                        \n"
"    <title> FMIBOOK </title>                                                                                                                                                                                                                                  \n"
"    <link rel=\"shortcut icon\" href=\"https://www.fmi.uni-sofia.bg/favicon.ico\" type=\"image/x-icon\">                                                                                                                                                      \n"
"    <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>                                                                                                                                                                \n"
"    <style>                                                                                                                                                                                                                                                   \n"
"        body { margin: 0; background-color: #eeebe9; text-align: center; font-family: Helvetica, Arial, sans-serif; font-size: 14px; }                                                                                                                        \n"
"        #kofa { width: 1012px; margin: 55px auto; padding: 0; }                                                                                                                                                                                               \n"
"        div#menuBar { position: fixed; top:0; width: 100%; margin: 0; padding: 9px; height: 24px; background-color: #961300; border-bottom: 1px solid #821000; }                                                                                              \n"
"        div#menuBar input { padding: 0; font-size: 14px; float: left; display: inline-block; margin: 0 0 0 6px; padding: 0 0 0 7px; width: 446px; height: 22px; border: 1px solid #821000; border-radius: 3px; }                                              \n"
"        div#menuBar img#logo { cursor: pointer;  float: left; border: 1px solid #821000; }                                                                                                                                                                    \n"
"        div#menuBar button { -webkit-transition: background-color 0.5s; transition: background-color 0.5s; cursor: pointer; background-color: #f9f7f6; float: left; width: 46px; height: 22px; margin: 1px 0 0 -47px; border: 0; border-radius: 0 3px 3px 0; }\n"
"        div#menuBar button:hover { background-color: #dddad8; }                                                                                                                                                                                               \n"
"        div#menuBar p#username { border-right: 1px solid #821000; float: right; padding: 1px 9px 3px 0; margin: 3px 0 0 6px; color: white; font-weight: bold; font-size: 12px; }                                                                              \n"
"        div#menuBar p#home { float: right; padding: 1px 9px 3px 0; margin: 3px 0 0 9px; color: white; font-weight: bold; font-size: 12px; }                                                                                                                   \n"
"        div.post { margin: 0 0 10px 0; width: 476px; padding: 12px; color: #1d2129; background-color: #fff; border: 1px solid #e2dfdd; border-radius: 3px; }                                                                                                  \n"
"        div.post div.content img { width: 100%; }\n"
"    </style>                                                                                                                                                                                                                                                  \n"
"</head>                                                                                                                                                                                                                                                       \n"
"<body>                                                                                                                                                                                                                                                        \n"
"    <script> function search() { window.open(\"https://www.fmi.uni-sofia.bg/bg/search/node/\" + $(\"#search-box\").val(),\"_self\"); } </script>                                                                                                              \n"
"    <div id=\"menuBar\">                                                                                                                                                                                                                                      \n"
"        <div id=\"container\" style=\"width: 1012px; margin: 0 auto; padding: 0;\">                                                                                                                                                                           \n"
"            <form onsubmit=\"search();return false;\">                                                                                                                                                                                                        \n"
"                <img id=\"logo\" src=\"http://www.math.bas.bg/complan/caa13/images/logo_FMI_crop.jpg\" height=\"22px\">                                                                                                                                       \n"
"                <input id=\"search-box\" autocomplete=\"off\"  placeholder=\"Search\">                                                                                                                                                                        \n"
"                <button type=\"submit\"> <img src=\"http://icons.iconarchive.com/icons/icons8/ios7/48/Very-Basic-Search-icon.png\" style=\"margin: 3px 0 0 0;\" width=\"14px\"> </button>                                                                     \n"
"            </form>                                                                                                                                                                                                                                           \n"
"            <img src=\"dropdown.png\" height=\"16px\" style=\"margin: 3px 0 0 10px; float: right;\">                                                                                                              \n"
"            <img src=\"question_mark.png\" height=\"20px\" style=\"margin: 2px 0 0 3px; float: right;\">                                              \n"
"            <img src=\"people.png\" height=\"20px\" style=\"margin: 3px 6px 0 15px; float: right;\">                                                                        \n"
"            <p id=\"home\"> Home </p>                                                                                                                                                                                                                         \n"
"            <p id=\"username\"> ";


const char HTML_BEGIN_FILE2[] =
" </p>                                                                                                                                        \n"
"            <img src=\"account.png\" height=\"24px\" style=\"float: right;\">\n"
"        </div>                                                                                                                               \n"
"    </div>                                                                                                                                   \n"
"    <div id=\"kofa\">                                                                                                                        \n";


const char HTML_POST_BEGIN[] =
"        <div class=\"post\">                                                                                                                    \n"
"            <div class=\"header\" style=\"height: 40px;\">                                                                                      \n"
"                <img src=\"account.png\" height=\"40px\" style=\"float: left;\">\n"
"                <div class=\"header-text\" style=\"float: left; text-align: left; margin: 2px 0 0 8px; color: #706761;\"> <div style=\"display: inline-block; font-weight: bold; color: #994936;\"> ";


const char HTML_POST_CONTINUE[] =
"</div> ";


const char HTML_POST_CONTINUE2[] =
" <br> <div style=\"display: inline-block; font-size: 12px; color: #706761;\">";


const char HTML_POST_CONTINUE3[] =
"</div></div>\n"
"            </div>\n"
"            <div class=\"content\" style=\"text-align: left; margin: 10px 0 0 0;\">\n"
"                ";


const char HTML_POST_END[] =
"\n            </div>\n"
"        </div>\n";


const char HTML_END[] =
"    </div>\n"
"</body>\n"
"</html>";

#endif // !__HTML_CODE_HEADER__
