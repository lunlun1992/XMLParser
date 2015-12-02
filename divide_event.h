#ifndef DIVIDE_EVENT_H_
#define DIVIDE_EVENT_H_
#include "common.h"

void divide_event_edge(XMLDataSet *dataset, char *pdata, int64_t length);
int64_t divide_event(XMLDataSet *dataset, char *pdata);

#endif
