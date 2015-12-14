#include "file_io.h"

XMLParserContext *read_XML_file(char *in)
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
		if (error_state == 1)
			return NULL;
		h->pp_data_sets[h->i_count_data_sets++] = dataset;
	}

	//h->unresolved_stag_num = 0;
	h->unresolved_stag_stack_head = (XMLSTagStack*)malloc(sizeof(XMLSTagStack));
	h->unresolved_stag_stack_head->next = NULL;
	h->unresolved_stag_stack_head->dataset_index = 0;
	h->unresolved_stag_stack_head->event_index = 0;

	return h;
}

void release_XML_file(XMLParserContext *h, char* outFile)
{
	while (h->unresolved_stag_stack_head->next != NULL)
	{
		XMLSTagStack* pstag = pop_stag(h);
		char stag_name[STAG_NAME_LEN];
		sscanf(h->pp_data_sets[pstag->dataset_index]->events[pstag->event_index]->event_stream + 3, "%[^ ]", stag_name);
		printf("Error: STAG %s missing ETAG\n", stag_name);
		free(pstag);
	}
	free(h->unresolved_stag_stack_head);

	h->XMLstreamout = fopen(outFile, "wb"); 
	if (NULL == h->XMLstreamout)
	{
		printf("file open error 2\n");
	}

	for (int64_t i = 0; i < h->i_count_data_sets; i++)
	{
		XMLDataSet *dataset = h->pp_data_sets[i];
		for (int j = 0; j < dataset->i_events; j++)
		{
			XMLEvents *cur_event = dataset->events[j];
			if (!fwrite(cur_event->event_stream, cur_event->i_event_stream_length, 1, h->XMLstreamout))
				printf("file write %I64d error\n", j);

			free(cur_event->event_stream);
			free(cur_event);
		}
		//if (!fwrite(dataset->XMLstream, dataset->XMLstream_length, 1, h->XMLstreamout))
		//	printf("file write %I64d error\n", i);

		free(dataset->XMLstream);
		free(dataset);
	}
	fclose(h->XMLstreamout);
	free(h->XMLbuf);
}

