#include "file_io.h"

XMLParserContext *read_XML_file(char *in, char *out)
{
	int64_t length;
	int64_t i;
	char *pdata;

	//read file
	XMLParserContext *h = (XMLParserContext *)malloc(sizeof(XMLParserContext));
	memset(h, 0, sizeof(XMLParserContext));
	h->XMLfilein = fopen(in, "rb");
	if(NULL == h->XMLfilein)
	{
		printf("file open error 1\n");
	}
	h->XMLstreamout = fopen(out, "wb"); // ab
	if(NULL == h->XMLstreamout)
	{
		printf("file open error 2\n");
	}
	fseek(h->XMLfilein, 0, SEEK_END);
	length = ftell(h->XMLfilein);
	fseek(h->XMLfilein, 0, SEEK_SET);

	h->XMLbuf = (char *)malloc(sizeof(char) * length);
	if(!fread(h->XMLbuf, length, 1, h->XMLfilein))
	{
		printf("file read error\n");
	}
	fclose(h->XMLfilein);

	//divide event
	pdata = h->XMLbuf;
	i = 0;
	h->i_count_data_sets = 0;
	while(i < length)
	{
		XMLDataSet *dataset = (XMLDataSet *)malloc(sizeof(XMLDataSet));
		memset(dataset, 0, sizeof(XMLDataSet));
		//when the rest is no more than max, deal in a separate way.
		if(length - i < DATA_SET_MAX)
		{
			divide_event_edge(dataset, pdata, length - i);
			i = length;
		}
		else
		{
			int64_t eventlen = divide_event(dataset, pdata);
			pdata += eventlen;
			i += eventlen;
		}
		h->pp_data_sets[h->i_count_data_sets++] = dataset;
	}

	//h->unresolved_stag_num = 0;
	h->unresolved_stag_stack_head = (XMLSTagStack*)malloc(sizeof(XMLSTagStack));
	h->unresolved_stag_stack_head->next = NULL;
	strcpy(h->unresolved_stag_stack_head->name, "");

	return h;
}

void release_XML_file(XMLParserContext *h)
{
	while (h->unresolved_stag_stack_head->next != NULL)
	{
		printf("Error: redundant STAG %d\n", top_stag(h));
		pop_stag(h);
	}
	free(h->unresolved_stag_stack_head);

	for (int64_t i = 0; i < h->i_count_data_sets; i++)
	{
		XMLDataSet *dataset = h->pp_data_sets[i];
		if (!fwrite(dataset->XMLstream, dataset->XMLstream_length, 1, h->XMLstreamout))
			printf("file write %I64d error\n", i);

		free(dataset->XMLstream);
		free(dataset);
	}
	fclose(h->XMLstreamout);
	free(h->XMLbuf);
}

