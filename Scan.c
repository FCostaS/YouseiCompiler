#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Scan.h"
#include "Globals.h"

char *copyString(char * s)
{
  int n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  t = malloc(n);
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else strcpy(t,s);
  return t;
}
