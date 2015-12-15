#include "post_process.h"

void push_stag(XMLParserContext *h, int dataset_index, int event_index)
{
	XMLSTagStack* tag = (XMLSTagStack*)malloc(sizeof(XMLSTagStack));
	tag->next = h->unresolved_stag_stack_head->next;
	tag->dataset_index = dataset_index;
	tag->event_index = event_index;
	h->unresolved_stag_stack_head->next = tag;
}

XMLSTagStack* pop_stag(XMLParserContext *h)
{
	XMLSTagStack* p = h->unresolved_stag_stack_head->next;
	h->unresolved_stag_stack_head->next = p->next;
	return p; // need free
}


//deal with STAG & ETAG match or not
void post_process(XMLParserContext *h)
{
	int i;
	XMLDataSet* cur_dataset = h->pp_data_sets[h->i_post];
	//cur_dataset->XMLstream_length = 0;
	//cur_dataset->XMLstream = (char*)malloc(sizeof(char));

	for (i = 0; i < cur_dataset->i_events; i++)
	{
		XMLEvents *cur_event = cur_dataset->events[i];
		/*
		int last_len = cur_dataset->XMLstream_length;
		cur_dataset->XMLstream_length += cur_event->i_event_stream_length;
		cur_dataset->XMLstream = (char*)realloc(cur_dataset->XMLstream, cur_dataset->XMLstream_length);
		memcpy(cur_dataset->XMLstream + last_len, cur_event->event_stream, cur_event->i_event_stream_length);
		//*/

		if (cur_event->event_stream[0] == 'S' && cur_event->event_stream[1] == 'E')
		{
			push_stag(h, h->i_post, i);
		}
		else if (cur_event->event_stream[0] == 'E' && cur_event->event_stream[1] == 'E')
		{
			char cur_name[STAG_NAME_LEN];
			sscanf(cur_event->event_stream + 3, "%[^ ]", cur_name);

			if (h->unresolved_stag_stack_head->next == NULL)
			{
				printf("Error: </%s> missing STAG\n", cur_name);
			}
			else
			{
				XMLSTagStack* pstag = pop_stag(h);
				char stag_name[STAG_NAME_LEN];
				sscanf(h->pp_data_sets[pstag->dataset_index]->events[pstag->event_index]->event_stream + 3, "%[^ ]", stag_name);
	
				if (strcmp(stag_name, cur_name) != 0)
					printf("Error: </%s> mismatch STAG\n", cur_name);

				free(pstag);
			}
		}
	}
}

void post_process_thread(XMLParserContext *h)
{
	while(h->i_post < h->i_count_data_sets)
	{
		pthread_mutex_lock(&pthread_param.mutex);
		if(h->i_post  == h->i_parse)
			pthread_cond_wait(&pthread_param.b_notempty, &pthread_param.mutex);
		post_process(h);
		h->i_post++;
		pthread_cond_signal(&pthread_param.b_notfull);
		pthread_mutex_unlock(&pthread_param.mutex);
	}
	pthread_exit(0);
}
