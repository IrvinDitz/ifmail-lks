#include <stdio.h>
#include "bread.h"
#include "config.h"

/* read short (16bit) integer in "standart" byte order */
int iread(fp)
FILE *fp;
{
	unsigned char lo,hi;

	fread(&lo,1,1,fp);
	fread(&hi,1,1,fp);
	return (hi<<8) | lo;
}

/* read long (32bit) integer in "standart" byte order */
long lread(fp)
FILE *fp;
{
	int c;
	unsigned char buf;
	long ret=0L;

	for (c=0;c<32;c+=8)
	{
		fread(&buf,1,1,fp);
		ret |= ((unsigned long)buf << c);
	}
	return ret;
}

static int at_zero=0;

char *aread(s,count,fp)
char *s;
int count;
FILE *fp;
{
	int i,c,next;

	if (feof(fp)) return(NULL);
	if (s == NULL) return NULL;
	if (at_zero)
	{
		at_zero=0;
		return NULL;
	}

	for (i=0,next=1;(i<count-1) && next;)
	switch (c=getc(fp))
	{
	case '\n':	break;
	case '\r':	s[i]='\n';
			i++;
			next=0;
			break;
	case 0x8d:	s[i]=' ';
			i++;
			break;
	case '\0':	at_zero=1;
			next=0;
			break;
	default:	s[i]=intab[c];
			i++;
			break;
	}
	s[i]='\0';
	return s;
}
