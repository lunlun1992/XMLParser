#ifndef PARSE_EVENT_H_
#define PARSE_EVENT_H_
#include "common.h"
#include "thread_pool.h"

//define bool 
#define bool int
#define true 1
#define false 0
#define MAX_THREAD_NUM 5

void parse_events(XMLParserContext *h);

#endif
