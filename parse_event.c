#include "parse_event.h"

ErrorValue statues = NoError;

bool NameStartChar_judge(char c)
{
	if (c == ':' || c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return true;
	else
		return false;
}

bool NameChar_judge(char c)
{
	if (NameStartChar_judge(c))
		return true;
	else if (c == '-' || c == '.' || (c >= '0' && c <= '9'))
		return true;
	else
		return false;
}

bool Name_judge(char *c)
{
	int i = 1;
	if (!NameStartChar_judge(c[0]))
		return false;
	while (c[i] != '>' && c[i] != ' ')
	{
		if (!NameChar_judge(c[i]))
			return false;
		else
			i++;
	}
	return true;
}

bool Char_judge(char c)
{
	if (c == '\n' || c == '\r' || c == '\t')
		return true;
	else if (c >= ' ' && c <= 127)
		return true;
	else
		return false;
}

bool CDEnd_judge(char *c)
{
	int i = 0;
	if (c[i] == ']' && c[i + 1] == ']' && c[i + 2] == '>')
		return true;
	else
		return false;
}
/*
bool CharData_judge(char *c)
{
int i = 0;
while (c[i] != '-')
{
if (c[i] != '<' && c[i] != '&')
i++;
else
return false;
}
i++;
while (c[i] != ']')
{
if (c[i] != '<' && c[i] != '&')
i++;
else
return false;
}
if (c[i + 1] != ']' && c[i + 2] != '>')
return false;
else
{
i += 3;
while (c[i] != '<')                     //CharData end when meet '<' ????
{
if (c[i] != '<' && c[i] != '&')
i++;
else
return false;
}
}
return true;

}*/

bool CharRef_judge(char *c)
{
	int i = 0;
	if (c[i] == '&' && c[i + 1] == '#' && c[i + 2] != 'x')
	{
		i = 2;
		while (c[i] != ';')
		{
			if (c[i] > '0' && c[i] < '9')
				i++;
			else
				return false;
		}
	}
	else if (c[i] == '&' && c[i + 1] == '#' && c[i + 2] == 'x')
	{
		i = 3;
		while (c[i] != ';')
		{
			if ((c[i] > '0' && c[i] < '9') || (c[i] > 'a' && c[i] < 'f') || (c[i] > 'A' && c[i] < 'F'))
				i++;
			else
				return false;
		}
	}
	return true;
}

bool EntityRef_judge(char *c)
{
	int i = 2;
	if (c[0] == '&')
	{
		if (!NameStartChar_judge(c[1]))
			return false;
		while (c[i] != ';')
		{
			if (!NameChar_judge(c[i]))
				return false;
			else
				i++;
		}
		return true;
	}
	else
		return false;
}

bool Reference_judge(char *c)
{
	if (!CharRef_judge(c) && !EntityRef_judge(c))
		return false;
	else
		return true;
}

bool AttValue_judge(char *c)
{
	int i = 1;
	if (c[0] == '"')
	{
		while (c[i] != '"')
		{
			if (Reference_judge(c + i))
				return true;
			else
			{
				if (c[i] != '<' && c[i] != '&' && c[i] != '"')
					i++;
				else
					return false;
			}
		}
		i++;
		if (c[i] == '"')
			return false;
		else if (c[i] != ' ' && c[i] != '\n' && c[i] != '\t' && c[i] != '\r' && c[i] != '>')
			return false;
		else
			return true;
	}
	else if (c[0] == '\'')
	{
		while (c[i] != '\'')
		{
			if (Reference_judge(c + i))
				return true;
			else
			{
				if (c[i] != '<' && c[i] != '&' && c[i] != '\'')
					i++;
				else
					return false;
			}
		}
		i++;
		if (c[i] == '\'')
			return false;
		else if (c[i] != ' ' && c[i] != '\n' && c[i] != '\t' && c[i] != '\r' && c[i] != '>')
			return false;
		else
			return true;
	}
	else
		return false;
}

bool Attribute_judge(char *c)
{
	int i = 1;
	if (!NameStartChar_judge(c[0]))
		return false;
	while (c[i] != ' ' && c[i] != '\r' && c[i] != '\t' &&
		c[i] != '\n' && c[i] != '=')
	{
		if (!NameChar_judge(c[i]))
			return false;
		else
			i++;
	}
	if ((c[i] == ' ' || c[i] == '\r' || c[i] == '\t' || c[i] == '\n') && c[i + 1] == '=')  //S=
	{
		if (c[i + 2] != '"' && c[i + 2] != '\'')  //S=S
		{
			if (c[i + 2] != ' ' && c[i + 2] != '\r' && c[i + 2] != '\t' && c[i + 2] != '\n')
				return false;
		}
		else
			return AttValue_judge(c + 2);
	}
	else if (c[i] == '=') // =
	{
		if (c[i + 1] != '"' && c[i + 1] != '\'')  //=S
		{
			if (c[i + 1] != ' ' && c[i + 1] != '\r' && c[i + 1] != '\t' && c[i + 1] != '\n')
				return false;
		}
		else
			return AttValue_judge(c + i + 1);
	}
	return true;
}

void CDATA_Parse(XMLEvents *current_event)
{
	int i, k, n = 2;
	current_event->event_stream = (char*)malloc(sizeof(char) * Event_Stream_Size);    //outstream buffer
	memset(current_event->event_stream, 0, sizeof(char) * Event_Stream_Size);
	current_event->event_stream[0] = 'C';
	current_event->event_stream[1] = 'D';
	current_event->event_stream[2] = ' ';
	for (k = 9, i = 3; k < current_event->i_event_length - 3; k++, i++)
	{
		if (!Char_judge(current_event->p_event_start[k]))
		{
			statues = CharError;
			break;
		}
		else if (current_event->event_stream[i] == 0)     //current_event->event_stream buf not full
			current_event->event_stream[i] = current_event->p_event_start[k];
		else    //current_event->event_stream is full
		{
			current_event->event_stream = (char*)realloc(current_event->event_stream, (sizeof(char) * Event_Stream_Size * n));
			current_event->event_stream[i] = current_event->p_event_start[k];
			n++;
		}
	}

	current_event->i_event_stream_length = i;
	//printf("%s\n", current_event->event_stream);
	if (statues != NoError)
		printf("CDATA Parse is wrong, the error value is %d\n", statues);
}

void PI_Parse(XMLEvents *current_event)
{
	bool haveS = false;
	int i = 0;
	int k = 0;
	int n = 2;
	current_event->event_stream = (char*)malloc(sizeof(char) * Event_Stream_Size);    //outstream buffer
	memset(current_event->event_stream, 0, sizeof(char) * Event_Stream_Size);
	current_event->event_stream[0] = 'P';
	current_event->event_stream[1] = 'I';
	current_event->event_stream[2] = ' ';
	if (!NameStartChar_judge(current_event->p_event_start[3]))
		statues = PITargetError;
	else
	{
		i = 4;
		k = 4;
		current_event->event_stream[3] = current_event->p_event_start[3];
		while (!(current_event->p_event_start[k] == '?' && current_event->p_event_start[k + 1] == '>'))     // not end
		{
			if (current_event->p_event_start[k] == ' ' || current_event->p_event_start[k] == '\r' ||
				current_event->p_event_start[k] == '\t' || current_event->p_event_start[k] == '\n')
				haveS = true;
			else
				haveS = false;
			k++;
		}
		i = 4;
		k = 4;
		if (!haveS)    //<? PITarget ?>
		{
			for (k = 4, i = 4; k < current_event->i_event_length - 2; k++, i++)
			{
				if (NameChar_judge(current_event->p_event_start[k]))
				{
					if (current_event->event_stream[i] == 0)     //current_event->event_stream buf not full
						current_event->event_stream[i] = current_event->p_event_start[k];
					else    //current_event->event_stream is full
					{
						current_event->event_stream = (char*)realloc(current_event->event_stream, (sizeof(char) * Event_Stream_Size * n));
						current_event->event_stream[i] = current_event->p_event_start[k];
						n++;
					}
				}
				else
				{
					statues = PITargetError;
					break;
				}
			}
		}
		else      //<? PITarget (S(...))..?>
		{
			for (k = 4, i = 4; k < current_event->i_event_length - 2;)
			{
				while (current_event->p_event_start[k] != ' ' && current_event->p_event_start[k] != '\r' &&
					current_event->p_event_start[k] != '\n' && current_event->p_event_start[k] != '\t')
				{
					if (NameChar_judge(current_event->p_event_start[k]))
					{
						if (current_event->event_stream[i] == 0)     //current_event->event_stream buf not full
							current_event->event_stream[i++] = current_event->p_event_start[k++];
						else    //current_event->event_stream is full
						{
							current_event->event_stream = (char*)realloc(current_event->event_stream, (sizeof(char) * Event_Stream_Size * n));
							current_event->event_stream[i++] = current_event->p_event_start[k++];
							n++;
						}
					}
					else
					{
						statues = PITargetError;
						break;
					}
				}
				while (k < current_event->i_event_length - 3)  //avoid current_event->p_event_start[k + 3] out of length
				{
					if (Char_judge(current_event->p_event_start[k]) && current_event->p_event_start[k + 1] == '?' &&
						current_event->p_event_start[k + 2] == '>' &&  Char_judge(current_event->p_event_start[k + 3]))
						statues = PIError;
					else
					{
						if (current_event->event_stream[i] == 0)     //current_event->event_stream buf not full
							current_event->event_stream[i++] = current_event->p_event_start[k++];
						else    //current_event->event_stream is full
						{
							current_event->event_stream = (char*)realloc(current_event->event_stream, (sizeof(char) * Event_Stream_Size * n));
							current_event->event_stream[i++] = current_event->p_event_start[k++];
							n++;
						}
					}
				}

				if (current_event->event_stream[i] == 0)     //current_event->event_stream buf not full
					current_event->event_stream[i++] = current_event->p_event_start[k++];
				else    //current_event->event_stream is full
				{
					current_event->event_stream = (char*)realloc(current_event->event_stream, (sizeof(char) * Event_Stream_Size * n));
					current_event->event_stream[i++] = current_event->p_event_start[k++];
					n++;
				}
			}
		}
	}
	
	current_event->i_event_stream_length = i;
	//printf("%s\n", current_event->event_stream);
	if (statues != NoError)
		printf("PI Parse is wrong, the error value is %d\n", statues);
}

void Comment_Parse(XMLEvents *current_event)
{
	int i, k, n = 2;
	current_event->event_stream = (char*)malloc(sizeof(char) * Event_Stream_Size);    //outstream buffer
	memset(current_event->event_stream, 0, sizeof(char) * Event_Stream_Size);
	current_event->event_stream[0] = 'C';
	current_event->event_stream[1] = ' ';
	k = 4;
	i = 2;
	while (!(current_event->p_event_start[k] == '-' && current_event->p_event_start[k + 1] == '-' && current_event->p_event_start[k + 2] == '>'))
	{
		if (Char_judge(current_event->p_event_start[k]))
		{
			if (current_event->event_stream[i] == 0)     //current_event->event_stream buf not full
				current_event->event_stream[i++] = current_event->p_event_start[k++];
			else    //current_event->event_stream is full
			{
				current_event->event_stream = (char*)realloc(current_event->event_stream, (sizeof(char) * Event_Stream_Size * n));
				current_event->event_stream[i++] = current_event->p_event_start[k++];
				n++;
			}
		}
		else
		{
			statues = CommentError;
			break;;
		}
	}
	current_event->i_event_length = k + 2;

	current_event->i_event_stream_length = i;
	//printf("%s\n", current_event->event_stream);
	if (statues != NoError)
		printf("Comment Parse is wrong, the error value is %d\n", statues);
}

void STAG_Parse(XMLEvents *current_event)
{
	int i, k, n = 2;
	current_event->event_stream = (char*)malloc(sizeof(char) * Event_Stream_Size);    //outstream buffer
	memset(current_event->event_stream, 0, sizeof(char) * Event_Stream_Size);
	if (!NameStartChar_judge(current_event->p_event_start[1]))
	{
		statues = 2;
		return;
	}
	else
	{
		k = 0;
		while (current_event->p_event_start[k] != '>')
			k++;
		if (current_event->p_event_start[k - 1] == '/')
		{
			current_event->event_stream[0] = 'E';
			current_event->event_stream[1] = 'M';
			current_event->event_stream[2] = 'P';
			current_event->event_stream[3] = 'T';
			current_event->event_stream[4] = 'Y';
			current_event->event_stream[5] = ' ';
			i = 6;   //event_stream index
			k = 1;   //p_event_start offset
			while (current_event->p_event_start[k] != ' ' && current_event->p_event_start[k] != '\t' &&
				current_event->p_event_start[k] != '\r' && current_event->p_event_start[k] != '\n' &&
				current_event->p_event_start[k] != '/' && current_event->p_event_start[k] != '>')
			{
				if (!NameChar_judge(current_event->p_event_start[k]))
				{
					statues = NameCharError;
					break;
				}
				else if (current_event->event_stream[i] == 0)     //current_event->event_stream buf not full
					current_event->event_stream[i++] = current_event->p_event_start[k++];
				else    //current_event->event_stream is full
				{
					current_event->event_stream = (char*)realloc(current_event->event_stream, (sizeof(char) * Event_Stream_Size * n));
					current_event->event_stream[i++] = current_event->p_event_start[k++];
					n++;
				}
			}
			if ((current_event->p_event_start[k] == ' ' && current_event->p_event_start[k + 1] == '/' &&
				current_event->p_event_start[k + 2] == '>') || (current_event->p_event_start[k + 1] == '/' &&
				current_event->p_event_start[k + 2] == '>')) //  S?> & >
				current_event->event_stream[i] = ' ';
			else
			{
				current_event->event_stream[i++] = ' ';
				current_event->event_stream[i++] = 'A';
				current_event->event_stream[i++] = ' ';
				k++;
				while (current_event->p_event_start[k] != '/')
				{
					if (!Attribute_judge((current_event->p_event_start + k)))
					{
						statues = AttributeError;
						break;
					}
					while (current_event->p_event_start[k] != '"' && current_event->p_event_start[k] != '\'')   // start " & '
					{
						if (current_event->event_stream[i] == 0)
						{
							if (current_event->p_event_start[k] == '=')
							{
								current_event->event_stream[i++] = ' ';
								k++;
							}
							else
								current_event->event_stream[i++] = current_event->p_event_start[k++];
						}
						else    //current_event->event_stream is full
						{
							current_event->event_stream = (char*)realloc(current_event->event_stream, (sizeof(char) * Event_Stream_Size * n));
							if (current_event->p_event_start[k] == '=')
							{
								current_event->event_stream[i++] = ' ';
								k++;
							}
							else
								current_event->event_stream[i++] = current_event->p_event_start[k++];
							n++;
						}
					}
					k++;
					while (current_event->p_event_start[k] != '"' && current_event->p_event_start[k] != '\'')  //end " & '
					{
						if (current_event->event_stream[i] == 0)
							current_event->event_stream[i++] = current_event->p_event_start[k++];
						else    //current_event->event_stream is full
						{
							current_event->event_stream = (char*)realloc(current_event->event_stream, (sizeof(char) * Event_Stream_Size * n));
							current_event->event_stream[i++] = current_event->p_event_start[k++];
							n++;
						}
					}
					current_event->event_stream[i++] = ' ';
					if (!(current_event->p_event_start[k + 1] == '/' && current_event->p_event_start[k + 2] == '>'))
						k += 2;
					else
						k++;
				}
			}
		}
		else
		{
			current_event->event_stream[0] = 'S';
			current_event->event_stream[1] = 'E';
			current_event->event_stream[2] = ' ';
			i = 3;   //event_stream index
			k = 1;   //p_event_start offset
			while (current_event->p_event_start[k] != ' ' && current_event->p_event_start[k] != '\t' &&
				current_event->p_event_start[k] != '\r' && current_event->p_event_start[k] != '\n' &&
				current_event->p_event_start[k] != '>')
			{
				if (!NameChar_judge(current_event->p_event_start[k]))
				{
					statues = NameCharError;
					break;
				}
				else if (current_event->event_stream[i] == 0)     //current_event->event_stream buf not full
					current_event->event_stream[i++] = current_event->p_event_start[k++];
				else    //current_event->event_stream is full
				{
					current_event->event_stream = (char*)realloc(current_event->event_stream, (sizeof(char) * Event_Stream_Size * n));
					current_event->event_stream[i++] = current_event->p_event_start[k++];
					n++;
				}
			}
			if ((current_event->p_event_start[k] == ' ' && current_event->p_event_start[k + 1] == '>') || current_event->p_event_start[k] == '>') //  S?> & >
				current_event->event_stream[i] = ' ';
			else
			{
				current_event->event_stream[i++] = ' ';
				current_event->event_stream[i++] = 'A';
				current_event->event_stream[i++] = ' ';
				k++;
				while (current_event->p_event_start[k] != '>')
				{
					if (!Attribute_judge((current_event->p_event_start + k)))
					{
						statues = AttributeError;
						break;
					}
					while (current_event->p_event_start[k] != '"' && current_event->p_event_start[k] != '\'')   // start " & '
					{
						if (current_event->event_stream[i] == 0)
						{
							if (current_event->p_event_start[k] == '=')
							{
								current_event->event_stream[i++] = ' ';
								k++;
							}
							else
								current_event->event_stream[i++] = current_event->p_event_start[k++];
						}
						else    //current_event->event_stream is full
						{
							current_event->event_stream = (char*)realloc(current_event->event_stream, (sizeof(char) * Event_Stream_Size * n));
							if (current_event->p_event_start[k] == '=')
							{
								current_event->event_stream[i++] = ' ';
								k++;
							}
							else
								current_event->event_stream[i++] = current_event->p_event_start[k++];
							n++;
						}
					}
					k++;
					while (current_event->p_event_start[k] != '"' && current_event->p_event_start[k] != '\'')  //end " & '
					{
						if (current_event->event_stream[i] == 0)
							current_event->event_stream[i++] = current_event->p_event_start[k++];
						else    //current_event->event_stream is full
						{
							current_event->event_stream = (char*)realloc(current_event->event_stream, (sizeof(char) * Event_Stream_Size * n));
							current_event->event_stream[i++] = current_event->p_event_start[k++];
							n++;
						}
					}
					current_event->event_stream[i++] = ' ';
					if (current_event->p_event_start[k + 1] != '>')
						k += 2;
					else
						k++;
				}
			}
		}
	}
	current_event->i_event_length = k;

	current_event->i_event_stream_length = i;
	//printf("%s\n", current_event->event_stream);
	if (statues != NoError)
		printf("STAG Parse is wrong, the error value is %d\n", statues);
}

void ETAG_Parse(XMLEvents *current_event)
{
	int i, k, n = 2;
	current_event->event_stream = (char*)malloc(sizeof(char) * Event_Stream_Size);    //outstream buffer
	memset(current_event->event_stream, 0, sizeof(char) * Event_Stream_Size);
	//NameStartChart judge
	if (!NameStartChar_judge(current_event->p_event_start[2]))
	{
		statues = 2;
		return;
	}
	else
	{
		//EE->ETAG
		current_event->event_stream[0] = 'E';
		current_event->event_stream[1] = 'E';
		current_event->event_stream[2] = ' ';
		i = 3;   //event_stream index
		k = 2;   //p_event_start offset
		while ((current_event->p_event_start[k] != ' ' && current_event->p_event_start[k] != '\t' &&
			current_event->p_event_start[k] != '\n' && current_event->p_event_start[k] != '\r' &&
			current_event->p_event_start[k] != '>') || ((current_event->p_event_start[k] == ' '|| 				current_event->p_event_start[k] == '\t' || current_event->p_event_start[k] == '\n' || 				current_event->p_event_start[k] == '\r') && current_event->p_event_start[k + 1] != '>' ))
		{
			if (!NameChar_judge(current_event->p_event_start[k]))
			{
				statues = NameCharError;
				break;
			}
			else if (current_event->event_stream[i] == 0)     //current_event->event_stream buf not full
				current_event->event_stream[i++] = current_event->p_event_start[k++];
			else    //current_event->event_stream is full
			{
				current_event->event_stream = (char*)realloc(current_event->event_stream, (sizeof(char) * Event_Stream_Size * n));
				current_event->event_stream[i++] = current_event->p_event_start[k++];
				n++;
			}
		}
		if (statues == NoError)
		{
			if (current_event->p_event_start[k] != '>')
			{
				current_event->event_stream[i++] = current_event->p_event_start[k++];
			}
		}
		current_event->event_stream[i++] = ' ';
	}
	
	current_event->i_event_length = k;
	
	current_event->i_event_stream_length = i;
	//printf("%s\n", current_event->event_stream);
	if (statues != NoError)
		printf("ETAG Parse is wrong, the error value is %d\n", statues);
}

//deal with data sets and sequance them
void parse_events(XMLParserContext *h, int64_t data_set_index)
{
	int j;
	for (j = 0; j < h->pp_data_sets[data_set_index]->i_events; j++)
	{
		XMLEvents *current_event = h->pp_data_sets[data_set_index]->events[j];
		switch (current_event->i_label)
		{
		case STAG:
			STAG_Parse(current_event);
			break;
		case ETAG:
			ETAG_Parse(current_event);
			break;
		case COMMENT:
			Comment_Parse(current_event);
			break;
		case PI:
			PI_Parse(current_event);
			break;
		case CDATA:
			CDATA_Parse(current_event);
			break;
		}
	}
}

