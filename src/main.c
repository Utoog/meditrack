#include <stdio.h>
#include <sqlite3.h>

#include "database.h"

static void add_test_entries(void)
{
    db_add_media(MEDIA_MOVIE, "Scott Pilgrim vs. The World", 2011, STATUS_FINISHED, RATE_LIKE);
    db_add_media(MEDIA_VIDEOGAME, "Minecraft", 2011, STATUS_IN_PROGRESS, RATE_LIKE);
    db_add_media(MEDIA_TV_SEASON, "Better Call Saul (1 Season)", 2016, STATUS_FINISHED, RATE_LIKE);
    db_add_media(MEDIA_MUSIC_ALBUM, "Somewhere City", 2017, STATUS_FINISHED, RATE_LIKE);
}

int main(int argc, char **argv)
{
    if (db_init()) return -1;
    add_test_entries();
    db_close();
    return 0;
}