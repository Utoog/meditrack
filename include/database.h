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
void db_close(void);

#endif