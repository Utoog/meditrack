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

#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <stddef.h>

#define BITMASK_MEDIA_TYPE   (1 << 0)
#define BITMASK_MEDIA_NAME   (1 << 1)
#define BITMASK_MEDIA_YEAR   (1 << 2)
#define BITMASK_MEDIA_STATUS (1 << 3)
#define BITMASK_MEDIA_RATE   (1 << 4)

typedef enum {
    MEDIA_MOVIE,
    MEDIA_VIDEOGAME,
    MEDIA_TV_SERIES,
    MEDIA_MUSIC_ALBUM
} media_type_t;

typedef enum {
    STATUS_PLANNED,
    STATUS_IN_PROGRESS,
    STATUS_FINISHED,
    STATUS_DROPPED
} media_status_t;

typedef enum {
    RATE_NO_RATE,
    RATE_LIKE,
    RATE_DISLIKE
} media_rate_t;

// Initialize database and table
int db_init(void);

// Add a new media entry
int db_add_media(media_type_t media_type, const char* media_name, int media_year, media_status_t media_status,  media_rate_t media_rate);

// Remove media entry by its id
int db_remove_entry(int entry_id);

//  Remove everything from database
int db_purge_everything(void);

// Change status of media entry
int db_change_status(int entry_id, media_status_t media_status);

// Change rating of media entry
int db_change_rating(int entry_id, media_rate_t rate);

// Update all fields of a media entry
int db_update_entry(int entry_id, media_type_t media_type, const char* media_name, int media_year, media_status_t media_status, media_rate_t media_rate);

// Update fields specified by a bitmask
// (REQUIRED to place entry_id as the last argument)
int db_update_entry_flexible(unsigned int bitmask, ...);

// Request everything from media table
int db_get_all_entries(int (*callback)(void*,int,char**,char**));

// Request specific media entry by its id
int db_get_entry_info(int entry_id, int (*callback)(void*,int,char**,char**));

// Search for string in media's name
int db_search_media(const char *name, int (*callback)(void*,int,char**,char**));

// Get string value from specific field of an entry
int db_get_string_from_id(int media_id, const char *field, char *buffer);

// Get decimal value from specific field of an entry
int db_get_int_from_id(int media_id, const char *field, int *value);

// Close db and free resources
void db_close(void);

#endif
