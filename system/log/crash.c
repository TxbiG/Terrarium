#include "crash.h"

#include <string.h>

#include "../status.h"

#define TERRA_CRASH_RING_MAX 32u

static terra_crash_report_t g_reports[TERRA_CRASH_RING_MAX];
static unsigned long g_report_count;

int terra_crash_write_report(const terra_crash_report_t *report) {
    if (!report || !report->process[0] || !report->reason[0])
        return TERRA_STATUS_INVAL;
    unsigned long slot = g_report_count % TERRA_CRASH_RING_MAX;
    g_reports[slot] = *report;
    ++g_report_count;
    return TERRA_STATUS_OK;
}

int terra_crash_read_report(unsigned long index, terra_crash_report_t *out_report) {
    if (!out_report)
        return TERRA_STATUS_INVAL;
    unsigned long visible = g_report_count < TERRA_CRASH_RING_MAX ? g_report_count : TERRA_CRASH_RING_MAX;
    if (index >= visible)
        return TERRA_STATUS_NOENT;
    unsigned long first = g_report_count > TERRA_CRASH_RING_MAX ? g_report_count % TERRA_CRASH_RING_MAX : 0;
    unsigned long slot = (first + index) % TERRA_CRASH_RING_MAX;
    *out_report = g_reports[slot];
    return TERRA_STATUS_OK;
}

int terra_crash_report_count(void) {
    return (int)(g_report_count < TERRA_CRASH_RING_MAX ? g_report_count : TERRA_CRASH_RING_MAX);
}
