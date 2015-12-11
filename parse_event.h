#ifndef PARSE_EVENT_H_
#define PARSE_EVENT_H_
#include "common.h"

//define bool 
#define bool int
#define true 1
#define false 0

void parse_events(XMLParserContext *h, int64_t data_set_index);

bool NameStartChar_judge(char c);

bool NameChar_judge(char c);

bool Name_judge(char *c);

bool Char_judge(char c);

bool CDEnd_judge(char *c);

//bool CharData_judge(char *c);

bool PITarget_judge(char *c);

bool CharRef_judge(char *c);

bool EntityRef_judge(char *c);

bool Reference_judge(char *c);

bool AttValue_judge(char *c);

bool Attribute_judge(char *c);

void STAG_Parse(XMLEvents *event);
void ETAG_Parse(XMLEvents *event);
void Comment_Parse(XMLEvents *event);
void PI_Parse(XMLEvents *event);
void CDATA_Parse(XMLEvents *event);
//void EmptyElem_Parse(XMLEvents *event);


#endif
