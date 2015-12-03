#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef __GNUC__
#include <stdint.h>
#else
#include <sys/types.h>
#endif
#define DATA_SET_MAX 10240
#define MAX_COUNT_EVENTS 10000
#define MAX_COUNT_DATA_SETS 100000

typedef enum XMLEventLabel_t
{
	STAG = 1,
	ETAG = 2,
	PI = 3,
	COMMENT = 4,
	CDATA = 5
}XMLEventLabel;

typedef struct XMLEvents_t
{
	XMLEventLabel i_label;
	char *p_event_start;
	int64_t i_event_length;
	char *event_stream;
	int64_t i_event_stream_length;
}XMLEvents;

typedef struct XMLDataSet_t
{
	char *p_start_data_set;
	int64_t i_data_set_length;
	XMLEvents *events[MAX_COUNT_EVENTS];//TODO: use realloc to avoid static array length
	int64_t i_events;

	char *XMLstream;
	int64_t XMLstream_length;
}XMLDataSet;

typedef struct XMLParserContext_t
{
	FILE *XMLfilein;
	FILE *XMLstreamout;
	char *XMLbuf;
	int64_t XMLlength;

	int64_t i_count_data_sets;
	XMLDataSet *pp_data_sets[MAX_COUNT_DATA_SETS];

}XMLParserContext;

#endif
