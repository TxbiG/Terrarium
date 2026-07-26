#ifndef TERRARIUM_SYSTEM_TIME_CALENDAR_H
#define TERRARIUM_SYSTEM_TIME_CALENDAR_H

typedef struct terra_calendar_time {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
} terra_calendar_time_t;

int terra_calendar_from_epoch(long long epoch_seconds, terra_calendar_time_t *out_time);
long long terra_calendar_to_epoch(const terra_calendar_time_t *time);

#endif
