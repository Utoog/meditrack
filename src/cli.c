// MediTrack
// Copyright (C) 2026  Utoog

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "database.h"
#include "cli.h"
#include "common.h"

#define NAME_BUFFSIZE   128
#define TYPE_BUFFSIZE   24
#define STATUS_BUFFSIZE 16
#define RATE_BUFFSIZE STATUS_BUFFSIZE

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
#endif

// type, name, year, status, rate

static void get_strings_from_callback(
    int argc, char **argv, char **col_name,
    char media_type[TYPE_BUFFSIZE],
    char media_name[NAME_BUFFSIZE],
    int *media_year,
    char media_status[STATUS_BUFFSIZE],
    char media_rate[RATE_BUFFSIZE]
)
{
    for (int i = 0; i < argc; i++) {
        if (!strcmp(col_name[i], "type"))
        {
            if (!media_type) continue;
            media_type_t mediatype = atoi(argv[i]);
            switch (mediatype)
            {
                case MEDIA_MOVIE:
                    strncpy(media_type, "Movie", TYPE_BUFFSIZE);
                    break;
                case MEDIA_VIDEOGAME:
                    strncpy(media_type, "Videogame", TYPE_BUFFSIZE);
                    break;
                case MEDIA_TV_SERIES:
                    strncpy(media_type, "TV Season", TYPE_BUFFSIZE);
                    break;
                case MEDIA_MUSIC_ALBUM:
                    strncpy(media_type, "Music Album", TYPE_BUFFSIZE);
                    break;
            }
        }
        else if (!strcmp(col_name[i], "name"))
        {
            if (!media_name) continue;
            strncpy(media_name, argv[i], NAME_BUFFSIZE);
        }
        else if (!strcmp(col_name[i], "year"))
        {
            if (!media_year) continue;
            *media_year = atoi(argv[i]);
        }
        else if (!strcmp(col_name[i], "status"))
        {
            if (!media_status) continue;
            media_status_t media_status_id = atoi(argv[i]);
            switch (media_status_id)
            {
                case STATUS_PLANNED:
                    strncpy(media_status, "Planned", STATUS_BUFFSIZE);
                    break;
                case STATUS_IN_PROGRESS:
                    strncpy(media_status, "In Progress", STATUS_BUFFSIZE);
                    break;
                case STATUS_FINISHED:
                    strncpy(media_status, "Finished", STATUS_BUFFSIZE);
                    break;
                case STATUS_DROPPED:
                    strncpy(media_status, "Dropped", STATUS_BUFFSIZE);
                    break;
            }
        }
        else if (!strcmp(col_name[i], "rate"))
        {
            if (!media_rate) continue;
            media_rate_t media_rate_id = atoi(argv[i]);
            switch (media_rate_id)
            {
                case RATE_NO_RATE:
                    strncpy(media_rate, "-", RATE_BUFFSIZE);
                    break;
                case RATE_LIKE:
                    strncpy(media_rate, "Liked", RATE_BUFFSIZE);
                    break;
                case RATE_DISLIKE:
                    strncpy(media_rate, "Didn't like", RATE_BUFFSIZE);
                    break;
            }
        }
    }
}

static void get_numeric_from_callback(
    int argc, char **argv, char **col_name,
    int *id,
    media_type_t *media_type,
    media_status_t *media_status,
    media_rate_t *media_rate
)
{
    for (int i = 0; i < argc; i++)
    {
        if (!strcmp(col_name[i], "type"))
        {
            if (!media_type) continue;
            *media_type = atoi(argv[i]);
        }
        else if (!strcmp(col_name[i], "status"))
        {
            if (!media_status) continue;
            *media_status = atoi(argv[i]);
        }
        else if (!strcmp(col_name[i], "rate"))
        {
            if (!media_rate) continue;
            *media_rate = atoi(argv[i]);
        }
        else if (!strcmp(col_name[i], "id"))
        {
            if (!id) continue;
            *id = atoi(argv[i]);
        }
    }
}

#ifdef Debug
static void add_test_entries(void)
{
    db_add_media(MEDIA_MOVIE, "Scott Pilgrim vs. The World", 2011, STATUS_FINISHED, RATE_LIKE);
    db_add_media(MEDIA_MOVIE, "Man on The Moon", 1999, STATUS_FINISHED, RATE_LIKE);
    db_add_media(MEDIA_MOVIE, "Goodtimes", 2017, STATUS_PLANNED, RATE_NO_RATE);
    db_add_media(MEDIA_MOVIE, "Silent Hill", 2007, STATUS_DROPPED, RATE_DISLIKE);
    db_add_media(MEDIA_VIDEOGAME, "Minecraft", 2011, STATUS_IN_PROGRESS, RATE_LIKE);
    db_add_media(MEDIA_VIDEOGAME, "The Legend of Zelda: Ocarina of Time", 1998, STATUS_IN_PROGRESS, RATE_LIKE);
    db_add_media(MEDIA_VIDEOGAME, "The Legend of Zelda: Majora''s Mask", 1998, STATUS_PLANNED, RATE_NO_RATE);
    db_add_media(MEDIA_TV_SERIES, "Better Call Saul (1 Season)", 2016, STATUS_FINISHED, RATE_LIKE);
    db_add_media(MEDIA_MUSIC_ALBUM, "Somewhere City", 2017, STATUS_FINISHED, RATE_LIKE);
    db_add_media(MEDIA_MUSIC_ALBUM, "Чугунный скороход", 1997, STATUS_DROPPED, RATE_NO_RATE);
}
#endif

static int cli_print_media_list_cb(void *data, int argc, char **argv, char **col_name)
{
    (void) data;
    int media_id = 0;
    media_type_t media_type = 0;
    char media_type_str[RATE_BUFFSIZE];
    char media_name[NAME_BUFFSIZE];
    int media_year = 0;
    char media_status[STATUS_BUFFSIZE];
    media_rate_t media_rate;
    char media_rate_str[RATE_BUFFSIZE];
    
    get_strings_from_callback(argc, argv, col_name, NULL, media_name, &media_year, media_status, NULL);
    get_numeric_from_callback(argc, argv, col_name, &media_id, &media_type, NULL, &media_rate);
    
    switch (media_type) {
        case MEDIA_MOVIE:
            strcpy(media_type_str, "MV");
            break;
        case MEDIA_VIDEOGAME:
            strcpy(media_type_str, "VG");
            break;
        case MEDIA_TV_SERIES:
            strcpy(media_type_str, "TV");
            break;
        case MEDIA_MUSIC_ALBUM:
            strcpy(media_type_str, "MUS");
            break;
    }

    switch (media_rate) {
        case RATE_NO_RATE:
            strcpy(media_rate_str, BG_BLACK "/" COLOR_RESET);
            break;
        case RATE_LIKE:
            strcpy(media_rate_str, BG_GREEN "+" COLOR_RESET);
            break;
        case RATE_DISLIKE:
            strcpy(media_rate_str, BG_RED "-" COLOR_RESET);
            break;
    }

    printf(COLOR_YELLOW"[%s] " COLOR_RESET, media_type_str);
    printf(COLOR_CYAN "%s " COLOR_RESET, media_name );
    printf("(%d) ", media_year);
    printf(COLOR_WHITE "(id: %d)\n" COLOR_RESET, media_id);
    printf("\tStatus: " UNDERLINE_WHITE "%s\t\t" COLOR_RESET, media_status);
    printf("Rating: %s\n", media_rate_str);

    return 0;
}

#ifdef Debug
static int __attribute__((unused)) cli_print_all_dbg_cb(void *data, int argc, char **argv, char **col_name)
{
    (void) data;
    char media_type[TYPE_BUFFSIZE];
    char media_name[NAME_BUFFSIZE];
    int media_year;
    char media_status[STATUS_BUFFSIZE];
    char media_rate[STATUS_BUFFSIZE];

    get_strings_from_callback(argc, argv, col_name, media_type, media_name, &media_year, media_status, media_rate);
    
    printf("type = %s\n",  media_type);
    printf("name = %s\n",  media_name);
    printf("year = %d\n",  media_year);
    printf("status = %s\n", media_status);
    printf("rate = %s\n",  media_rate);
    putchar('\n');
    
    return 0;
}
#endif

static void cli_print_media_list(void)
{
    db_get_all_entries(cli_print_media_list_cb);
}

static void cli_print_single_media(int entry_id)
{
    db_get_entry_info(entry_id, cli_print_media_list_cb);
}

static void cli_print_help(const char *name)
{
    printf( "usage: %s <command> [options]\n"
        "commands:\n"
        "\thelp\t\t- Print this help message\n"
        "\tversion\t\t- Print version of the program\n"
        "\tadd <options>\t- Create new media entry\n"
        "\tedit <options>\t- Edit a media entry\n"
        "\tremove <options>\t- Remove a media entry\n"
        "\tlist <options>\t- Print all media entries\n"
        "\tshow <options>\t- Print specific media entry information\n"
        "\tsearch <text>\t- Search media which contains <text> in its name\n"
        "For additional help for every command you can type:\n"
        "%s <command> -h\n",
        name, name
    );
}

static void cli_print_version(void)
{
    printf("%s version (%d.%d.%d)\n",
        PROGRAM_NAME,
        PROGRAM_VERSION_MAJOR,
        PROGRAM_VERSION_MINOR,
        PROGRAM_VERSION_BUILD);
    printf("Copyright (C) %d %s\n"
    "%s comes with ABSOLUTELY NO WARRANTY;\n"
    "This is free software, and you are welcome to \n"
    "redistribute it under certain conditions.\n", 2026, "Utoog", PROGRAM_NAME);
}

static void cli_cmd_add(int argc, char **argv)
{
    // if not interactive then all -t -n and -y are required
    if (argc < 2)
    {
        printf("print help: %s add -h\n", argv[0]);
        return;
    }
    int not_interactive = 0;
    int opt;
    const char arguments[] = ":ht:n:y:";
    const int media_name_length = 128;
    media_type_t media_type;
    media_status_t media_status = STATUS_PLANNED;
    media_rate_t media_rating = RATE_NO_RATE;
    char media_name[media_name_length];
    int media_year;

    // TODO: add options to specify status and rating using options
    while ((opt = getopt(argc, argv, arguments)) != -1)
    {
        // printf("opt: %d\n", opt);
        // printf("%c: %s\n", optopt, optarg);
        // sleep(1);
        switch (opt)
        {
            case 'h':
                printf("usage: %s add <options> - Add a new media entry,\n"
                    "when no option is specified, interactive mode is invoked\n"
                    "options:\n"
                    "\t-h\t\t- print this message\n"
                    "\t-t <type>\t- Specify type of the media: <MV|VG|TV|MUS>\n"
                    "\t-n <name>\t- Specify name of the media\n"
                    "\t-y <year>\t- Specify year of the media\n"
                    "Note: MV - Movie, VG - Videogame, TV - TV Series, MUS - Music album\n",
                    argv[0]);
                return;
            case 't':
                not_interactive++;
                if (!strcmp(optarg, "MV")) media_type = MEDIA_MOVIE;
                else if (!strcmp(optarg, "VG")) media_type = MEDIA_VIDEOGAME;
                else if (!strcmp(optarg, "TV")) media_type = MEDIA_TV_SERIES;
                else if (!strcmp(optarg, "MUS")) media_type = MEDIA_MUSIC_ALBUM;
                else { printf("Unknown media type: %s\n", optarg); return; }
                break;
            case 'n':
                not_interactive++;
                strncpy(media_name, optarg, media_name_length);
                break;
            case 'y':
                not_interactive++;
                media_year = atoi(optarg);
                break;
            case ':':
                printf("option needs a value\n");
                return;
            case '?':
                printf("unknown option: %c\n", optopt);
                return;
        }
    }
    if (not_interactive)
    {
        if (not_interactive != 3)
        {
            printf("not_interactive: %d\n", not_interactive);
            puts("When using non-interactive adding, specifying type, name and year is REQUIRED!");
            return;
        }
    }
    else {
        int status;
        char rating_string[4];
        printf("[0] Movie\n"
            "[1] Videogame\n"
            "[2] TV Series\n"
            "[3] Music Album\n"
            "Type: ");
        status = scanf("%d", &media_type);
        if (status < 0)
        {
            printf("Error reading input: %d\n", status);
            return;
        }
        printf("Name: ");
        status = scanf("%127\[^n]", media_name);
        if (status <= 0)
        {
            printf("Error reading input: %d\n", status);
            return;
        }
        printf("Year: ");
        status = scanf("%d", &media_year);
        if (status <= 0)
        {
            printf("Error reading input: %d\n", errno);
            return;
        }
        printf("Status (+ for liked, - for disliked or / for neutral): ");
        status = scanf("%3s", rating_string);
        if (status <= 0)
        {
            printf("Error reading input: %d\n", errno);
            return;
        }
        switch (rating_string[0])
        {
            case '+':
                media_rating = RATE_LIKE;
                break;
            case '-':
                media_rating = RATE_DISLIKE;
                break;
            case '/':
                media_rating = RATE_NO_RATE;
                break;
            default:
                printf("Error reading input: %d\n", errno);
                return;
        }
        printf("[0] Planned\n"
            "[1] In Progress\n"
            "[2] Finished\n"
            "[3] Dropped\n"
            "Type: ");
        status = scanf("%d", &media_status);
        if (status <= 0)
        {
            printf("Error reading input: %d\n", errno);
            return;
        }
    }
    if (!db_add_media(media_type, media_name, media_year, media_status, media_rating))
    {
        puts("Media added succesfully!");
    }
    else
    {
        puts("Error adding new media.");
    }
}

static void cli_cmd_edit(int argc, char **argv)
{
    // TODO: **this**
    // -n <String>  -  Name
    // -y <Int>     -  Year
    // -m <Int>     -  Media ID
    if (argc < 3)
    {
        printf("usage: %s edit -h\n", argv[0]);
        return;
    }
    db_update_entry_flexible(BITMASK_MEDIA_YEAR | BITMASK_MEDIA_STATUS, 2012, STATUS_PLANNED, 1);
}

static void cli_cmd_remove(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("usage: %s remove <id> <id2> ...\n", argv[0]);
        return;
    }
    printf("Deleted media: ");
    for (int i = 2; i < argc; i++)
    {
        int media_id = atoi(argv[i]);
        printf("%d ", media_id);
        db_remove_entry(media_id);
    }
    putchar('\n');
}

static void cli_cmd_purge(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("type \"%s purge -y\" to confirm database purge\n", argv[0]);
        return;
    }
    if (!strcmp(argv[2], "-y"))
    {
        if (!db_purge_everything()) puts("Database purged.");
    }
}

static void cli_cmd_list(int argc, char **argv)
{
    // TODO: add filters
    (void) argc; (void) argv;
    cli_print_media_list();
}

//  Print information about a single entry
static void cli_cmd_show(int argc, char **argv)
{
    // <int>     - Media ID
    if (argc < 3)
    {
        printf("usage: %s show <id>\n", argv[0]);
        return;
    }
    int media_id = atoi(argv[2]);
    cli_print_single_media(media_id);
}

static void cli_cmd_search(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Usage: %s search \"<search query>\"\n", argv[0]);
        return;
    }
    // <String>  - Search name
    db_search_media(argv[2], cli_print_media_list_cb);
}

#ifdef Debug
static void cli_cmd_test(void)
{
    add_test_entries();
    // char testbuf[100] = "test string";
    // db_get_string_from_id(1, "name", testbuf, 100);
    // printf("%s\n", testbuf);
    // int testint = 0;
    // db_get_int_from_id(1, "year", &testint);
    // printf("%d\n", testint);
}
#endif

void cli_handle_flags(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("For help enter:\n%s help\n", argv[0]);
        return;
    }

    if (!strcmp(argv[1], "add"))
    {
        cli_cmd_add(argc, argv);
    }
    else if (!strcmp(argv[1], "edit"))
    {
        cli_cmd_edit(argc, argv);
    }
    else if (!strcmp(argv[1], "remove"))
    {
        cli_cmd_remove(argc, argv);
    }
    else if (!strcmp(argv[1], "purge"))
    {
        cli_cmd_purge(argc, argv);
    }
    else if (!strcmp(argv[1], "list"))
    {
        cli_cmd_list(argc, argv);
    }
    else if (!strcmp(argv[1], "show"))
    {
        cli_cmd_show(argc, argv);
    }
    else if (!strcmp(argv[1], "search"))
    {
        cli_cmd_search(argc, argv);
    }
    else if (!strcmp(argv[1], "help"))
    {
        cli_print_help(argv[0]);
    }
    else if (!strcmp(argv[1], "version"))
    {
        cli_print_version();
    }
#ifdef Debug
    else if (!strcmp(argv[1], "test"))
    {
        cli_cmd_test();
    }
#endif
    else
    {
        puts("Unknown command.");
        cli_print_help(argv[0]);
    }
}
