#ifndef PARSE_EVENT_H_
#define PARSE_EVENT_H_
#include "common.h"

//define bool 
#define bool int
#define true 1
#define false 0

void parse_events(XMLParserContext *h);
void parse_events_thread(XMLParserContext *h);
#endif
