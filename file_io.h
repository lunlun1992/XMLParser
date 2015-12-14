#ifndef FILE_IO_H_
#define FILE_IO_H_
#include "common.h"
#include "divide_event.h"
#include "post_process.h"

XMLParserContext *read_XML_file(char *in);
void release_XML_file(XMLParserContext *h, char* out);

#endif
