#include "divide_event.h"

error_state = 0;
static  void divide_event_edge_etag(XMLDataSet *dataset, char *pdata, int *i)
{
	XMLEvents *event = (XMLEvents *)malloc(sizeof(XMLEvents));
	memset(event, 0, sizeof(XMLEvents));
	event->i_label = ETAG;
	event->p_event_start = pdata + *i;
	(*i)++;
	dataset->events[dataset->i_events++] = event;
}

static  void divide_event_edge_com_cdata(XMLDataSet *dataset, char *pdata, int *i)
{
	XMLEvents *event = (XMLEvents *)malloc(sizeof(XMLEvents));
	memset(event, 0, sizeof(XMLEvents));
	if(pdata[*i + 2] == '-' && pdata[*i + 3] == '-')
	{
		event->i_label = COMMENT;
		event->p_event_start = pdata + *i;
		while(pdata[*i - 2] != '-' || pdata[*i - 1] != '-' || pdata[*i] != '>')
			(*i)++;
		(*i)++;
		event->i_event_length = pdata + *i - event->p_event_start;
		dataset->events[dataset->i_events++] = event;
	}
	else if(pdata[*i + 2] == '[' && pdata[*i + 3] == 'C' && pdata[*i + 4] == 'D' && pdata[*i + 5] == 'A'
			&& pdata[*i + 6] == 'T' && pdata[*i + 7] == 'A' && pdata[*i + 8] == '[')
	{
		event->i_label = CDATA;
		event->p_event_start = pdata + *i;
		while(pdata[*i - 2] != ']' || pdata[*i - 1] != ']' || pdata[*i] != '>')
			(*i)++;
		(*i)++;
		event->i_event_length = pdata + *i - event->p_event_start;
		dataset->events[dataset->i_events++] = event;
	}
}

static  void divide_event_edge_pi(XMLDataSet *dataset, char *pdata, int *i)
{
	XMLEvents *event = (XMLEvents *)malloc(sizeof(XMLEvents));
	memset(event, 0, sizeof(XMLEvents));
	event->i_label = PI;
	event->p_event_start = pdata + *i;
	while(pdata[*i - 1] != '?' || pdata[*i] != '>')
		(*i)++;
	(*i)++;
	event->i_event_length = pdata + *i - event->p_event_start;
	dataset->events[dataset->i_events++] = event;
}

static  void divide_event_edge_stag(XMLDataSet *dataset, char *pdata, int *i)
{
	XMLEvents *event = (XMLEvents *)malloc(sizeof(XMLEvents));
	memset(event, 0, sizeof(XMLEvents));
	event->i_label = STAG;
	event->p_event_start = pdata + *i;
	(*i)++;
	dataset->events[dataset->i_events++] = event;
}


static  void divide_event_etag(XMLDataSet *dataset, char *pdata, int *i)
{
	XMLEvents *event = (XMLEvents *)malloc(sizeof(XMLEvents));
	memset(event, 0, sizeof(XMLEvents));
	event->i_label = ETAG;
	event->p_event_start = pdata + *i;
	(*i)++;
	dataset->events[dataset->i_events++] = event;
}
static void divide_event_com_cdata(XMLDataSet *dataset, char *pdata, int *i)
{
	XMLEvents *event = (XMLEvents *)malloc(sizeof(XMLEvents));
	memset(event, 0, sizeof(XMLEvents));
	if((*i + 3) < DATA_SET_MAX && pdata[*i + 2] == '-' && pdata[*i + 3] == '-')
	{
		event->i_label = COMMENT;
		event->p_event_start = pdata + *i;
		while (pdata[*i - 2] != '-' || pdata[*i - 1] != '-' || pdata[*i] != '>')
		{
			(*i)++;
			if ((*i) == DATA_SET_MAX)
			{
				printf("event size is too large\n");
				error_state = 1;
				return;
			}
		}
		if((*i) == DATA_SET_MAX)
		{
			dataset->events[dataset->i_events++] = event;
			return;
		}
		(*i)++;
		event->i_event_length = pdata + *i - event->p_event_start;
		dataset->events[dataset->i_events++] = event;
	}
	else if((*i + 3) >= DATA_SET_MAX)//not longer enough
	{
		event->i_label = COMMENT;
		event->p_event_start = pdata + *i;
		dataset->events[dataset->i_events++] = event;
		(*i) += 3;
	}
	else if((*i + 8) < DATA_SET_MAX && pdata[*i + 2] == '[' && pdata[*i + 3] == 'C' && pdata[*i + 4] == 'D' && pdata[*i + 5] == 'A'
			&& pdata[*i + 6] == 'T' && pdata[*i + 7] == 'A' && pdata[*i + 8] == '[')
	{
		event->i_label = CDATA;
		event->p_event_start = pdata + *i;
		while (pdata[*i - 2] != ']' || pdata[*i - 1] != ']' || pdata[*i] != '>')
		{
			(*i)++;
			if ((*i) == DATA_SET_MAX)
			{
				printf("event size is too large\n");
				error_state = 1;
				return;
			}
		}
		if((*i) == DATA_SET_MAX)
		{
			dataset->events[dataset->i_events++] = event;
			return;
		}
		(*i)++;
		event->i_event_length = pdata + *i - event->p_event_start;
		dataset->events[dataset->i_events++] = event;
	}
	else if((*i + 8) >= DATA_SET_MAX)//not longer enough
	{
		event->i_label = CDATA;
		event->p_event_start = pdata + *i;
		dataset->events[dataset->i_events++] = event;
		(*i) += 8;
	}
}
static  void divide_event_pi(XMLDataSet *dataset, char *pdata, int *i)
{
	XMLEvents *event = (XMLEvents *)malloc(sizeof(XMLEvents));
	memset(event, 0, sizeof(XMLEvents));
	event->i_label = PI;
	event->p_event_start = pdata + *i;
	while ((*i) < DATA_SET_MAX && (pdata[*i - 1] != '?' || pdata[*i] != '>'))
	{
		(*i)++;
		if ((*i) == DATA_SET_MAX)
		{
			printf("event size is too large\n");
			error_state = 1;
			return;
		}
	}
	if((*i) == DATA_SET_MAX)
	{
		dataset->events[dataset->i_events++] = event;
		return;
	}
	(*i)++;
	event->i_event_length = pdata + *i - event->p_event_start;
	dataset->events[dataset->i_events++] = event;
}
static  void divide_event_stag(XMLDataSet *dataset, char *pdata, int *i)
{
	XMLEvents *event = (XMLEvents *)malloc(sizeof(XMLEvents));
	memset(event, 0, sizeof(XMLEvents));
	event->i_label = STAG;
	event->p_event_start = pdata + *i;
	(*i)++;
	dataset->events[dataset->i_events++] = event;
}

void divide_event_edge(XMLDataSet *dataset, char *pdata, int64_t length)
{
	int i = 0;
	dataset->p_start_data_set = pdata;
	dataset->i_data_set_length = length;
	while(i < length)
	{
		if (pdata[i] == '<')
		{
			switch (pdata[i + 1])
			{
			case '/':
				divide_event_edge_etag(dataset, pdata, &i);
				break;
			case '!':
				divide_event_edge_com_cdata(dataset, pdata, &i);
				break;
			case '?':
				divide_event_edge_pi(dataset, pdata, &i);
				break;
			default:
				divide_event_edge_stag(dataset, pdata, &i);
			}
		}
		else
			i++;
		if (error_state == 1)
			return;
	}
}

int64_t divide_event(XMLDataSet *dataset, char *pdata)
{
	int i = 0;
	dataset->p_start_data_set = pdata;
	while(i < DATA_SET_MAX)
	{
		if (pdata[i] == '<')
		{
			if (i + 1 == DATA_SET_MAX)
			{
				dataset->i_data_set_length = DATA_SET_MAX - 1;
				pdata += DATA_SET_MAX - 1;
				return dataset->i_data_set_length;
			}
			switch (pdata[i + 1])
			{
			case '/':
				divide_event_etag(dataset, pdata, &i);
				break;
			case '!':
				divide_event_com_cdata(dataset, pdata, &i);
				break;
			case '?':
				divide_event_pi(dataset, pdata, &i);
				break;
			default:
				divide_event_stag(dataset, pdata, &i);
			}
		}
		else
			i++;
		if (error_state == 1)
			return 0;
	}
	dataset->i_data_set_length = dataset->events[dataset->i_events - 1]->p_event_start - pdata;
	pdata = dataset->events[dataset->i_events - 1]->p_event_start;
	free(dataset->events[dataset->i_events - 1]);//remount to the last TAG
	dataset->events[--dataset->i_events] = NULL;

	return dataset->i_data_set_length;
}
