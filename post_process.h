#ifndef POST_PROCESS_H_
#define POST_PROCESS_H_
#include "common.h"

XMLSTagStack* pop_stag(XMLParserContext *h);
void post_process(XMLParserContext *h, int64_t data_set_index);

#endif
