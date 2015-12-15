#ifndef POST_PROCESS_H_
#define POST_PROCESS_H_
#include "common.h"

XMLSTagStack* pop_stag(XMLParserContext *h);
void post_process(XMLParserContext *h);
void post_process_thread(XMLParserContext *h);
#endif
