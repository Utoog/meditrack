#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include "database.h"

sqlite3* db_sqlite;

static int db_callback(void* data, int argc, char** argv, char** azColName)
{
    (void) data;
    
    for (int i = 0; i < argc; i++) {
        if (!strcmp(azColName[i], "type"))
        {
            media_type_t mediatype = atoi(argv[i]);
            char media_type_str[20];
            switch (mediatype)
            {
                case MEDIA_MOVIE:
                    strcpy(media_type_str, "Movie");
                    break;
                case MEDIA_VIDEOGAME:
                    strcpy(media_type_str, "Videogame");
                    break;
                case MEDIA_TV_SEASON:
                    strcpy(media_type_str, "TV Season");
                    break;
                case MEDIA_MUSIC_ALBUM:
                    strcpy(media_type_str, "Music Album");
                    break;
            }
            printf("%s = %s\n", azColName[i], media_type_str);
        }
        else if (!strcmp(azColName[i], "status"))
        {
            media_status_t media_status = atoi(argv[i]);
            char media_status_str[15];
            switch (media_status)
            {
                case STATUS_PLANNED:
                    strcpy(media_status_str, "Planned");
                    break;
                case STATUS_IN_PROGRESS:
                    strcpy(media_status_str, "In Progress");
                    break;
                case STATUS_FINISHED:
                    strcpy(media_status_str, "Finished");
                    break;
                case STATUS_DROPPED:
                    strcpy(media_status_str, "Dropped");
                    break;
            }
            printf("%s = %s\n", azColName[i], media_status_str);
        }
        else if (!strcmp(azColName[i], "rate"))
        {
            media_rate_t media_rate = atoi(argv[i]);
            char media_rate_str[15];
            switch (media_rate)
            {
                case RATE_NO_RATE:
                    strcpy(media_rate_str, "-");
                    break;
                case RATE_LIKE:
                    strcpy(media_rate_str, "Liked");
                    break;
                case RATE_DISLIKE:
                    strcpy(media_rate_str, "Didn't like");
                    break;
            }
            printf("%s = %s\n", azColName[i], media_rate_str);
        }
        else printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "-");
    }
    
    printf("\n");

    return 0;
}

static int create_table(void)
{
    char* err;
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

    int status = sqlite3_exec(db_sqlite, sql_create_table, NULL, NULL, &err);
    if (status) printf("Error: %s\n", err);
    sqlite3_free(err);
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

int db_add_media(
    media_type_t media_type,
    const char* media_name,
    int media_year,
    media_status_t media_status,
    media_rate_t media_rate)
{
    char *err = 0;
    int status = 0;
    //  FIXME: string validation
    const char sql_query_format[] = "INSERT INTO media (type, name, year, status, rate) VALUES (%d, '%s', %d, %d, %d);";
    char sql_query[400];
    sprintf(sql_query, sql_query_format, media_type, media_name, media_year, media_status, media_rate);
    status = sqlite3_exec(db_sqlite, sql_query, 0, 0, &err);
    if (status) printf("Error: %s\n", err);
    sqlite3_free(err);
    return status;
}

int db_remove_entry(int entry_id)
{
    char *err = 0;
    int status = 0;
    const char sql_query_format[] = "DELETE FROM media WHERE id = %d;";
    char sql_query[40];
    sprintf(sql_query, sql_query_format, entry_id);
    status = sqlite3_exec(db_sqlite, sql_query, 0, 0, &err);
    if (status) printf("Error: %s\n", err);
    sqlite3_free(err);
    return status;
}

int db_change_status(int entry_id, media_status_t media_status)
{
    char *err = 0;
    int status = 0;
    const char sql_query_format[] = "UPDATE media SET status = %d WHERE id = %d;";
    char sql_query[50];
    sprintf(sql_query, sql_query_format, media_status, entry_id);
    status = sqlite3_exec(db_sqlite, sql_query, 0, 0, &err);
    if (status) printf("Error: %s\n", err);
    sqlite3_free(err);
    return status;
}

int db_change_rating(int entry_id, media_rate_t rate)
{
    char *err = 0;
    int status = 0;
    const char sql_query_format[] = "UPDATE media SET rate = %d WHERE id = %d;";
    char sql_query[50];
    sprintf(sql_query, sql_query_format, rate, entry_id);
    status = sqlite3_exec(db_sqlite, sql_query, 0, 0, &err);
    if (status) printf("Error: %s\n", err);
    sqlite3_free(err);
    return status;
}

int db_update_entry(
    int entry_id,
    media_type_t media_type,
    const char* media_name,
    int media_year, 
    media_status_t media_status,
    media_rate_t media_rate)
{
    char *err = 0;
    int status = 0;
    const char sql_query_format[] =
    "UPDATE media SET"
       "type = %d"
       "name = '%s'"
       "year = %d"
       "status = %d"
       "rate = %d"
       "WHERE id = %d;";
    char sql_query[400];
    sprintf(sql_query, sql_query_format,
        media_type,
        media_name,
        media_year,
        media_status,
        media_rate,
        entry_id);
    status = sqlite3_exec(db_sqlite, sql_query, 0, 0, &err);
    if (status) printf("Error: %s\n", err);
    sqlite3_free(err);
    return status;
}

int db_get_all_entries(void)
{
    char* err;
    int status = 0;
    const char sql_query[] = "SELECT * FROM media;";
    status = sqlite3_exec(db_sqlite, sql_query, db_callback, 0, &err);
    if (status) printf("Error: %s\n", err);
    sqlite3_free(err);
    return status;
}

int db_get_entry_info(int entry_id)
{
    char* err;
    int status = 0;
    const char sql_query_format[] = "SELECT * FROM media WHERE id = %d;";
    char sql_query[50];
    sprintf(sql_query, sql_query_format, entry_id);
    status = sqlite3_exec(db_sqlite, sql_query, db_callback, 0, &err);
    if (status) printf("Error: %s\n", err);
    sqlite3_free(err);
    return status;
}

void db_close(void)
{
    sqlite3_close(db_sqlite);
}