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

typedef enum {
    MEDIA_MOVIE,
    MEDIA_VIDEOGAME,
    MEDIA_TV_SEASON,
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

int db_init(void);
int db_add_media(media_type_t media_type, const char* media_name, int media_year, media_status_t media_status,  media_rate_t media_rate);
int db_remove_entry(int entry_id);
int db_change_status(int entry_id, media_status_t media_status);
int db_change_rating(int entry_id, media_rate_t rate);
int db_update_entry(int entry_id, media_type_t media_type, const char* media_name, int media_year, media_status_t media_status, media_rate_t media_rate);
int db_get_all_entries(void);
int db_get_entry_info(int entry_id);
void db_close(void);

#endif