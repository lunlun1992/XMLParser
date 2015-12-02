#ifndef FILE_IO_H_
#define FILE_IO_H_

#include "common.h"
#include "divide_event.h"

XMLParserContext *read_XML_file(char *in, char *out);
void release_XML_file(XMLParserContext *h);

#endif
