#include "post_process.h"

void push_stag(XMLParserContext *h, char* name)
{
	XMLSTagStack* tag = (XMLSTagStack*)malloc(sizeof(XMLSTagStack));
	tag->next = h->unresolved_stag_stack_head->next;
	strcpy(tag->name, name);
	h->unresolved_stag_stack_head->next = tag;
}

char* top_stag(XMLParserContext *h)
{
	return h->unresolved_stag_stack_head->next->name;
}

void pop_stag(XMLParserContext *h)
{
	XMLSTagStack* p = h->unresolved_stag_stack_head->next;
	h->unresolved_stag_stack_head->next = p->next;
	free(p);
}


//deal with STAG & ETAG match or not
void post_process(XMLParserContext *h, int64_t data_set_index)
{
	XMLDataSet* cur_dataset = h->pp_data_sets[data_set_index];
	cur_dataset->XMLstream_length = 0;
	cur_dataset->XMLstream = (char*)malloc(sizeof(char));

	for (int i = 0; i < cur_dataset->i_events; i++)
	{
		XMLEvents *cur_event = cur_dataset->events[i];
		int last_len = cur_dataset->XMLstream_length;
		cur_dataset->XMLstream_length += cur_event->i_event_stream_length;
		cur_dataset->XMLstream = (char*)realloc(cur_dataset->XMLstream, cur_dataset->XMLstream_length);
		memcpy(cur_dataset->XMLstream + last_len, cur_event->event_stream, cur_event->i_event_stream_length);

		if (cur_event->event_stream[0] == 'S' && cur_event->event_stream[1] == 'E')
		{
			char name[STAG_NAME_LEN];
			sscanf(cur_event->event_stream + 3, "%[^ ]", name);
			push_stag(h, name);
		}
		else if (cur_event->event_stream[0] == 'E' && cur_event->event_stream[1] == 'E')
		{
			if (h->unresolved_stag_stack_head->next == NULL)
				printf("Error: missing STAG\n");
			else
			{
				char name[STAG_NAME_LEN];
				sscanf(cur_event->event_stream + 3, "%[^ ]", name);
				if (strcmp(top_stag(h), name) != 0)
					printf("Error: mismatch STAG\n");
				else
					pop_stag(h);
			}
		}

		free(cur_event->event_stream);
		free(cur_event);
	}
}
