#include <stdio.h>
#include <sqlite3.h>

#include "database.h"

sqlite3* db_sqlite;

static int db_callback(void* data, int argc, char** argv, char** azColName)
{

    return 0;
}

static int create_table(void)
{
    const char* sql_create_table = 
    "CREATE TABLE IF NOT EXISTS media ("
        "id             INTEGER PRIMARY KEY,"
        "type           INTEGER NOT NULL,"              // 0 - Movie, 1 - VG, 2 - TV Season, 3 - Music Album
        "name           TEXT NOT NULL,"
        "year           INTEGER NOT NULL,"
        "status         INTEGER NOT NULL DEFAULT 0,"    // 0 - Planned, 1 - In Progress, 2 - Finished, 3 - Dropped
        "poster_file    TEXT,"                          // Name of an image that will be in local storage
        "rate           INTEGER NOT NULL DEFAULT 0"     // 0 - Didn't rate, 1 - Liked, 2 - Disliked
    ");";

    int status = sqlite3_exec(db_sqlite, sql_create_table, NULL, NULL, NULL);
    if (status) printf("Error: %s\n", sqlite3_errmsg(db_sqlite));
    return status;
}

int db_init(void)
{
    int status = 0;
    status = sqlite3_open("example.db", &db_sqlite);
    if (status)
    {
        printf("Error opening DB: %s\n", sqlite3_errmsg(db_sqlite));
        return -1;
    }
    status = create_table();
    if (status) return status;
    return 0;
}

int db_add_media(media_type_t media_type, const char* media_name, int media_year,  media_status_t media_status, media_rate_t media_rate)
{
    int status = 0;
    //  FIXME: string validation
    const char *sql_query_format = "INSERT INTO media (type, name, year, status, rate) VALUES (%d, '%s', %d, %d, %d);";
    char sql_query[300];
    sprintf(sql_query, sql_query_format, media_type, media_name, media_year, media_status, media_rate);
    status = sqlite3_exec(db_sqlite, sql_query, NULL, NULL, NULL);
    return status;
}

void db_close(void)
{
    sqlite3_close(db_sqlite);
}