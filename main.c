#include "common.h"
#include "file_io.h"
#include "parse_event.h"
#include "post_process.h"

int main(int argc, char **argv)
{
	XMLParserContext *h;
	struct timeval start, end;
	int i;
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
	for(i = 0; i < h->i_count_data_sets; i++)
	{
		parse_events(h, i);
		post_process(h, i);
	}

	release_XML_file(h, argv[2]);
	gettimeofday(&end, NULL);
	timeuse  = 1000000*(end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
	timeuse /= 1000;
	printf("%.4lf ms time used\n", timeuse);
	return 0;
}
