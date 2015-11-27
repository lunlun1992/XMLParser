#include "common.h"

XMLParserContext *read_XML_file(char *in, char *out)
{
	int64_t length;
	int i;
	XMLParserContext *h = (XMLParserContext *)malloc(sizeof(XMLParserContext));
	memset(h, 0, sizeof(XMLParserContext));
	h->XMLfilein = fopen(in, "rb");
	if(NULL == h->XMLfilein)
	{
		printf("file open error 1\n");
	}
	h->XMLstreamout = fopen(out, "ab");
	if(NULL == h->XMLstreamout)
	{
		printf("file open error 2\n");
	}
	fseek(h->XMLfilein, 0, SEEK_END);
	length = ftell(h->XMLfilein);
	fseek(h->XMLfilein, 0, SEEK_SET);
	h->i_count_data_sets = (length + DATA_SET_LENGTH - 1) / DATA_SET_LENGTH;
	h->p_data_set_starts = (int64_t *)malloc(sizeof(int64_t) * h->i_count_data_sets);
	for(i = 0; i < h->i_count_data_sets; i++)
	{
		h->p_data_set_starts[i] = i * DATA_SET_LENGTH;
	}
	return h;
}
