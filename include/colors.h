#ifndef __COLORS_H__
#define __COLORS_H__

#ifdef USE_COLOR
    #define COLOR_BLACK     "\e[0;30m"
    #define COLOR_RED       "\e[0;31m"
    #define COLOR_GREEN     "\e[0;32m"
    #define COLOR_YELLOW    "\e[0;33m"
    #define COLOR_BLUE      "\e[0;34m"
    #define COLOR_MAGENTA   "\e[0;35m"
    #define COLOR_CYAN      "\e[0;36m"
    #define COLOR_WHITE     "\e[0;37m"
    #define COLOR_RESET     "\e[0m"

    #define UNDERLINE_WHITE "\e[4;37m"

    #define BG_BLACK        "\e[40m"
    #define BG_RED          "\e[41m"
    #define BG_GREEN        "\e[42m"
    #define BG_YELLOW       "\e[43m"
    #define BG_BLUE         "\e[44m"
    #define BG_MAGENTA      "\e[45m"
    #define BG_CYAN         "\e[46m"
    #define BG_WHITE        "\e[47m"
#else
    #define UNDERLINE_WHITE

    #define COLOR_BLACK
    #define COLOR_RED
    #define COLOR_GREEN
    #define COLOR_YELLOW
    #define COLOR_BLUE
    #define COLOR_MAGENTA
    #define COLOR_CYAN
    #define COLOR_WHITE
    #define COLOR_RESET

    #define BG_BLACK
    #define BG_RED
    #define BG_GREEN
    #define BG_YELLOW
    #define BG_BLUE
    #define BG_MAGENTA
    #define BG_CYAN
    #define BG_WHITE
#endif // USE_COLOR

#endif
