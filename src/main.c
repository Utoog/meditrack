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
#include <sqlite3.h>

#include "database.h"

#define PROGRAM_NAME "meditrack"
#define PROGRAM_VERSION_MAJOR 0
#define PROGRAM_VERSION_MINOR 0
#define PROGRAM_VERSION_BUILD 3

static void welcome_print(void)
{
    printf("%s (%d.%d.%d)\n",
        PROGRAM_NAME,
        PROGRAM_VERSION_MAJOR,
        PROGRAM_VERSION_MINOR,
        PROGRAM_VERSION_BUILD);
}

static void add_test_entries(void)
{
    db_add_media(MEDIA_MOVIE, "Scott Pilgrim vs. The World", 2011, STATUS_FINISHED, RATE_LIKE);
    db_add_media(MEDIA_VIDEOGAME, "Minecraft", 2011, STATUS_IN_PROGRESS, RATE_LIKE);
    db_add_media(MEDIA_TV_SEASON, "Better Call Saul (1 Season)", 2016, STATUS_FINISHED, RATE_LIKE);
    db_add_media(MEDIA_MUSIC_ALBUM, "Somewhere City", 2017, STATUS_FINISHED, RATE_LIKE);
}

int main(int argc, char **argv)
{
    welcome_print();
    if (db_init()) return -1;
    add_test_entries();
    db_get_all_entries();
    db_close();
    return 0;
}
