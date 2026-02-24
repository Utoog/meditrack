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
#include <sqlite3.h>
#include <sys/stat.h>

#include "database.h"

#define DEFAULT_DB_LOCATION "/.local/share/meditrack"
#define MEDITRACK_DB_FILENAME "meditrack.db"

sqlite3* db_sqlite;

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
    if (status) printf("Error (%d): %s\n", status, err);
    sqlite3_free(err);
    return status;
}

int db_init(void)
{
    const int db_path_length = 256;
    char db_path[db_path_length];
#if 0
    strncat(db_path, getenv("HOME"), db_path_length);
    strncat(db_path, DEFAULT_DB_LOCATION, db_path_length);
    mkdir(db_path, 0755);

    // FIXME: seems like older versions of sqlite have
    // to include file: prefix
    // also this whole block of code is terrible
    strncpy(db_path, "file:", db_path_length);
    strncat(db_path, getenv("HOME"), db_path_length);
    strncat(db_path, DEFAULT_DB_LOCATION, db_path_length);
    strncat(db_path, "/" MEDITRACK_DB_FILENAME, db_path_length);
#else
    strncat(db_path, getenv("HOME"), db_path_length);
    strncat(db_path, DEFAULT_DB_LOCATION, db_path_length);
    mkdir(db_path, 0755);
    strncat(db_path, "/" MEDITRACK_DB_FILENAME, db_path_length);
#endif
    int status = 0;
    status = sqlite3_open(db_path, &db_sqlite);
    if (status)
    {
        printf("Error opening DB (%d): %s\n", status, sqlite3_errmsg(db_sqlite));
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

int db_get_all_entries(int (*callback)(void*,int,char**,char**))
{
    char* err;
    int status = 0;
    const char sql_query[] = "SELECT * FROM media;";
    status = sqlite3_exec(db_sqlite, sql_query, callback, 0, &err);
    if (status) printf("Error: %s\n", err);
    sqlite3_free(err);
    return status;
}

int db_get_entry_info(int entry_id, int (*callback)(void*,int,char**,char**))
{
    char* err;
    int status = 0;
    const char sql_query_format[] = "SELECT * FROM media WHERE id = %d;";
    char sql_query[50];
    sprintf(sql_query, sql_query_format, entry_id);
    status = sqlite3_exec(db_sqlite, sql_query, callback, 0, &err);
    if (status) printf("Error: %s\n", err);
    sqlite3_free(err);
    return status;
}

void db_close(void)
{
    sqlite3_close(db_sqlite);
}