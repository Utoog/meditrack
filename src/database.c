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

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <sys/stat.h>

#include "database.h"
#include "common.h"

#define DEFAULT_DB_LOCATION "/.local/share/" PROGRAM_NAME
#define MEDITRACK_DB_FILENAME PROGRAM_NAME ".db"

sqlite3* db_sqlite;

static int create_table(void)
{
    char* err;
    const char sql_create_table[] = 
    "CREATE TABLE IF NOT EXISTS media ("
        "id             INTEGER PRIMARY KEY,"
        "type           INTEGER NOT NULL,"              // 0 - Movie, 1 - VG, 2 - TV Series, 3 - Music Album
        "name           TEXT NOT NULL,"
        "year           INTEGER NOT NULL,"
        "status         INTEGER NOT NULL DEFAULT 0,"    // 0 - Planned, 1 - In Progress, 2 - Finished, 3 - Dropped
        "poster_file    TEXT,"                          // Name of an image that will be in local storage  [TO BE REPLACED BY UUID OF A POSTER]
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

    strncpy(db_path, getenv("HOME"), db_path_length);
    strncat(db_path, DEFAULT_DB_LOCATION, db_path_length);
    mkdir(db_path, 0755);
    strncat(db_path, "/" MEDITRACK_DB_FILENAME, db_path_length);

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
    const int query_length = 400;
    char *err = 0;
    int status = 0;
    const char sql_query_format[] = "INSERT INTO media (type, name, year, status, rate) VALUES (%d, %Q, %d, %d, %d);";
    char sql_query[query_length];
    sqlite3_snprintf(query_length, sql_query, sql_query_format, media_type, media_name, media_year, media_status, media_rate);
    status = sqlite3_exec(db_sqlite, sql_query, 0, 0, &err);
    if (status) printf("Error: %s\n", err);
    sqlite3_free(err);
    return status;
}

int db_remove_entry(int media_id)
{
    const int query_length = 50;
    char *err = 0;
    int status = 0;
    const char sql_query_format[] = "DELETE FROM media WHERE id = %d;";
    char sql_query[query_length];
    sqlite3_snprintf(query_length, sql_query, sql_query_format, media_id);
    status = sqlite3_exec(db_sqlite, sql_query, 0, 0, &err);
    if (status) printf("Error: %s\n", err);
    sqlite3_free(err);
    return status;
}

int db_purge_everything(void)
{
    char *err = 0;
    int status = 0;
    const char sql_query[] = "DROP TABLE media;";
    status = sqlite3_exec(db_sqlite, sql_query, 0, 0, &err);
    if (status) printf("Error: %s\n", err);
    sqlite3_free(err);
    return status;
}

int db_change_status(int media_id, media_status_t media_status)
{
    const int query_length = 50;
    char *err = 0;
    int status = 0;
    const char sql_query_format[] = "UPDATE media SET status = %d WHERE id = %d;";
    char sql_query[query_length];
    sqlite3_snprintf(query_length, sql_query, sql_query_format, media_status, media_id);
    status = sqlite3_exec(db_sqlite, sql_query, 0, 0, &err);
    if (status) printf("Error: %s\n", err);
    sqlite3_free(err);
    return status;
}

int db_change_rating(int media_id, media_rate_t rate)
{
    const int query_length = 50;
    char *err = 0;
    int status = 0;
    const char sql_query_format[] = "UPDATE media SET rate = %d WHERE id = %d;";
    char sql_query[query_length];
    sqlite3_snprintf(query_length, sql_query, sql_query_format, rate, media_id);
    status = sqlite3_exec(db_sqlite, sql_query, 0, 0, &err);
    if (status) printf("Error: %s\n", err);
    sqlite3_free(err);
    return status;
}

int db_edit_entry(
    unsigned bitmask,
    int media_id,
    media_type_t media_type,
    const char* media_name,
    int media_year)
{
    const int query_length = 400;
    char *err = 0;
    int status = 0;
    char sql_query[query_length];

    strcpy(sql_query, "UPDATE media SET ");
    if (bitmask & BITMASK_MEDIA_TYPE)
    {
        const int tmp_query_length = 32;
        char tmp_query[tmp_query_length];
        sqlite3_snprintf(tmp_query_length, tmp_query, "type = %d ", media_type);
        strncat(sql_query, tmp_query, tmp_query_length);
        if (bitmask > BITMASK_MEDIA_TYPE) strcat(sql_query, ", ");
    }
    if (bitmask & BITMASK_MEDIA_NAME)
    {
        const int tmp_query_length = 256;
        char tmp_query[tmp_query_length];
        sqlite3_snprintf(tmp_query_length, tmp_query, "name = %Q ", media_name);
        strncat(sql_query, tmp_query, tmp_query_length);
        if (bitmask > BITMASK_MEDIA_NAME) strcat(sql_query, ", ");
    }
    if (bitmask & BITMASK_MEDIA_YEAR)
    {
        const int tmp_query_length = 32;
        char tmp_query[tmp_query_length];
        sqlite3_snprintf(tmp_query_length, tmp_query, "year = %d ", media_year);
        strncat(sql_query, tmp_query, tmp_query_length);
    }
    const int tmp_query_length = 32;
    char tmp_query[tmp_query_length];
    sqlite3_snprintf(tmp_query_length, tmp_query, "WHERE id = %d;", media_id);
    strncat(sql_query, tmp_query, tmp_query_length);
    printf("%s\n", sql_query);

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

int db_get_entry_info(int media_id, int (*callback)(void*,int,char**,char**))
{
    const int query_length = 50;
    char* err;
    int status = 0;
    const char sql_query_format[] = "SELECT * FROM media WHERE id = %d;";
    char sql_query[query_length];
    sqlite3_snprintf(query_length, sql_query, sql_query_format, media_id);
    status = sqlite3_exec(db_sqlite, sql_query, callback, 0, &err);
    if (status) printf("Error: %s\n", err);
    sqlite3_free(err);
    return status;
}

int db_search_media(const char *name, int (*callback)(void*,int,char**,char**))
{
    const int query_length = 512;
    char *err;
    int status = 0;
    const char sql_query_format[] =
    "SELECT * FROM media WHERE name LIKE '%%%q%%';";
    char sql_query[query_length];
    sqlite3_snprintf(query_length, sql_query, sql_query_format, name);
    status = sqlite3_exec(db_sqlite, sql_query, callback, 0, &err);
    if (status) printf("Error: %s\n", err);
    sqlite3_free(err);
    return status;
}

static int get_string_cb(void *data, int argc, char **argv, char **col_name)
{
    (void) argc; (void) col_name;
    sprintf(data, "%s", argv[0]);
    return 0;
}

static int get_int_cb(void *data, int argc, char **argv, char **col_name)
{
    (void) argc; (void) col_name;
    int *integer = data;
    *integer = atoi(argv[0]);
    return 0;
}

int db_get_string_from_id(int media_id, const char *field, char *buffer)
{
    const int query_length = 64;
    char *err;
    int status = 0;
    const char sql_query_format[] = "SELECT %q FROM media WHERE id = %d";
    char sql_query[query_length];
    sqlite3_snprintf(query_length, sql_query, sql_query_format, field, media_id);
    status = sqlite3_exec(db_sqlite, sql_query, get_string_cb, buffer, &err);
    if (status) printf("Error: %s\n", err);
    sqlite3_free(err);
    return status;
}

int db_get_int_from_id(int media_id, const char *field, int *value)
{
    const int query_length = 64;
    char *err;
    int status = 0;
    const char sql_query_format[] = "SELECT %q FROM media WHERE id = %d";
    char sql_query[query_length];
    sqlite3_snprintf(query_length, sql_query, sql_query_format, field, media_id);
    status = sqlite3_exec(db_sqlite, sql_query, get_int_cb, value, &err);
    if (status) printf("Error: %s\n", err);
    sqlite3_free(err);
    return status;
}

int db_set_media_status(int media_id, media_status_t media_status)
{
    const int query_length = 64;
    char *err;
    int status = 0;
    const char sql_query_format[] = "UPDATE media SET status = %d WHERE id = %d;";
    char sql_query[query_length];
    sqlite3_snprintf(query_length, sql_query, sql_query_format, media_id, media_status);
    status = sqlite3_exec(db_sqlite, sql_query, 0, 0, &err);
    if (status) printf("Error: %s\n", err);
    sqlite3_free(err);
    return status;
}

int db_set_media_rating(int media_id, media_rate_t media_rating)
{
    const int query_length = 64;
    char *err;
    int status = 0;
    const char sql_query_format[] = "UPDATE media SET rating = %d WHERE id = %d;";
    char sql_query[query_length];
    sqlite3_snprintf(query_length, sql_query, sql_query_format, media_id, media_rating);
    status = sqlite3_exec(db_sqlite, sql_query, 0, 0, &err);
    if (status) printf("Error: %s\n", err);
    sqlite3_free(err);
    return status;
}

void db_close(void)
{
    sqlite3_close(db_sqlite);
}