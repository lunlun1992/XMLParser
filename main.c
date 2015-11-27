#include "common.h"
int main(int argc, char **argv)
{
	XMLParserContext *h;
	int i;
	printf("%d\n", argc);
	if(argc != 3)
	{
		printf("error input args\n");
		return 0;
	}
	h = read_XML_file(argv[1], argv[2]);
	printf("%I64d\n", h->i_count_data_sets);
	for(i = 0; i < h->i_count_data_sets; i++)
	{
		//load_data_set(h, i);
		//divide_events(h, i);
		//parse_events(h, i);
		//post_process(h);
	}
	//release_XML_file(h);
	return 0;
}
