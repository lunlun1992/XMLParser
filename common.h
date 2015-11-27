#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#define DATA_SET_LENGTH 1024
typedef enum XMLEventLabel_t
{
	START_TAG = 1,
	END_TAG = 2,
	START_PI = 3,
	ENDPI = 4,
	START_COMMENT = 5,
	END_COMMENT = 6,
	START_CDATA = 7,
	END_CDATA = 8
}XMLEventLabel;

typedef struct XMLEvents_t
{
	XMLEventLabel i_label;
	char *p_event_start;
	char *p_event_end;
	int8_t b_finish_yet;
	int64_t i_event_length;
}XMLEvents;

typedef struct XMLDataSet_t
{
	char *p_start_data_set;
	int64_t i_data_set_length;
	XMLEvents **events;
	int64_t i_events;

	char *bufferl1;
	char *bufferl2;
}XMLDataSet;

typedef struct XMLParserContext_t
{
	FILE *XMLfilein;
	FILE *XMLstreamout;
	int64_t *p_data_set_starts;
	int64_t i_count_data_sets;

	XMLDataSet *p_current_data_set;

}XMLParserContext;

#endif
