#ifndef _ARC_GPL_H
#define _ARC_GPL_H
#include <stdio.h>

typedef struct _CFGElm
{
	struct _CFGElm *_next;
	char Name[192];
	char *Value;
} CFGElm;

typedef struct _CFGList
{
	int nCount;
	CFGElm *head;		
} CFGList;

char *arcgpl_cfg_get(char *cfgName, char *cfgValue, int cfgValueSize);
int arcgpl_cfg_set(char *cfgName, char *cfgValue);
int arcgpl_cfg_commit();
int arcgpl_cfglist_free(CFGList *list);
int arcgpl_cfglist_get(char *cfgPrefix, CFGList *list);
char *arcgpl_cfglist_search(CFGList *list, char *cfgName, char *cfgValue, int cfgValueSize);
FILE *exec_open(int *ppid, char *args[]);
int exec_close(int pid, FILE *fp);

#endif

