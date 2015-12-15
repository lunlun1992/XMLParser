#include "common.h"
#include "file_io.h"
#include "parse_event.h"
#include "post_process.h"

void pthread_init()
{
	pthread_cond_init(&pthread_param.b_notempty, NULL);
	pthread_cond_init(&pthread_param.b_notfull, NULL);
	pthread_mutex_init(&pthread_param.mutex, NULL);
}

void pthread_destroy()
{
	pthread_cond_destroy(&pthread_param.b_notempty);
	pthread_cond_destroy(&pthread_param.b_notfull);
	pthread_mutex_destroy(&pthread_param.mutex);
}

int main(int argc, char **argv)
{
	XMLParserContext *h;
	int i;
	struct timeval start, end;
	double timeuse;
	gettimeofday(&start, NULL);
	if(argc != 3)
	{
		printf("error input args\n");
		return 0;
	}
	h = read_XML_file(argv[1]);
	if (h == NULL)
		return 0;
	h->i_parse = 0;
	h->i_post = 0;
	/*for(; h->i_parse < h->i_count_data_sets; h->i_parse++, h->i_post++)
	{
		parse_events(h);
		post_process(h);
	}*/
	pthread_init();
	pthread_create(&pthread_param.id_sender, NULL, (void *)parse_events_thread, h);
	pthread_create(&pthread_param.id_receiver, NULL, (void *)post_process_thread, h);
	pthread_join(pthread_param.id_sender, NULL);
	pthread_join(pthread_param.id_receiver, NULL);
	release_XML_file(h, argv[2]);
	pthread_destroy();

	gettimeofday(&end, NULL);
	timeuse  = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
	timeuse /= 1000;
	printf("%.4lf ms time used\n", timeuse);
	return 0;
}
