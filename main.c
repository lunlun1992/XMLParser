#include "common.h"
#include "file_io.h"
#include "parse_event.h"
#include "post_process.h"

int main(int argc, char **argv)
{
	XMLParserContext *h;
	int i;
	if(argc != 3)
	{
		printf("error input args\n");
		system("pause");
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

	return 0;
}
