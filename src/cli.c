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
#include <unistd.h>

#include "database.h"
#include "cli.h"

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

#define PROGRAM_NAME "meditrack"
#define PROGRAM_VERSION_MAJOR 0
#define PROGRAM_VERSION_MINOR 0
#define PROGRAM_VERSION_BUILD 5

static void welcome_print(void)
{
    printf("%s (%d.%d.%d)\n",
        PROGRAM_NAME,
        PROGRAM_VERSION_MAJOR,
        PROGRAM_VERSION_MINOR,
        PROGRAM_VERSION_BUILD);
}

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
                case MEDIA_TV_SEASON:
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
            char media_rate_str[15];
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

static void add_test_entries(void)
{
    db_add_media(MEDIA_MOVIE, "Scott Pilgrim vs. The World", 2011, STATUS_FINISHED, RATE_LIKE);
    db_add_media(MEDIA_MOVIE, "Man on The Moon", 1999, STATUS_FINISHED, RATE_LIKE);
    db_add_media(MEDIA_MOVIE, "Goodtimes", 2017, STATUS_PLANNED, RATE_NO_RATE);
    db_add_media(MEDIA_MOVIE, "Silent Hill", 2007, STATUS_DROPPED, RATE_DISLIKE);
    db_add_media(MEDIA_VIDEOGAME, "Minecraft", 2011, STATUS_IN_PROGRESS, RATE_LIKE);
    db_add_media(MEDIA_VIDEOGAME, "The Legend of Zelda: Ocarina of Time", 1998, STATUS_IN_PROGRESS, RATE_LIKE);
    db_add_media(MEDIA_VIDEOGAME, "The Legend of Zelda: Majora''s Mask", 1998, STATUS_PLANNED, RATE_NO_RATE);
    db_add_media(MEDIA_TV_SEASON, "Better Call Saul (1 Season)", 2016, STATUS_FINISHED, RATE_LIKE);
    db_add_media(MEDIA_MUSIC_ALBUM, "Somewhere City", 2017, STATUS_FINISHED, RATE_LIKE);
    db_add_media(MEDIA_MUSIC_ALBUM, "Чугунный скороход", 1997, STATUS_DROPPED, RATE_NO_RATE);
}

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
        case MEDIA_TV_SEASON:
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
            strcpy(media_rate_str, BG_GREEN "+" COLOR_GREEN);
            break;
        case RATE_DISLIKE:
            strcpy(media_rate_str, BG_RED "-" COLOR_RED);
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

static int cli_print_single_cb(void* data, int argc, char** argv, char** col_name)
{
    (void) data;
    
    

    return 0;
}

static int cli_print_all_dbg_cb(void *data, int argc, char **argv, char **col_name)
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

void cli_print_help(char *name)
{
    welcome_print();
    printf( "usage: %s [options]\n"
        "\t arguments:\n"
        "\t-a\tPrint list of all media that is being tracked\n"
        "\t-s <id>\tPrint info on specific media\n",
        name
    );
}

void cli_handle_flags(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("For help enter:\n%s -h\n", argv[0]);
        return;
    }
    int opt;

    while((opt = getopt(argc, argv, "hTas:e:n")) != -1)
    {
        switch (opt)
        {
            case 'h':
                cli_print_help(argv[0]);
                break;
            case 'T':
                add_test_entries();
                break;
            case 'a':
                cli_print_media_list();
                break;
            case 's':
                cli_print_single_media(atoi(optarg));
                break;
        }
    }
}


void cli_print_media_list(void)
{
    db_get_all_entries(cli_print_media_list_cb);
}

void cli_print_single_media(int entry_id)
{
    db_get_entry_info(entry_id, cli_print_media_list_cb);
}

