#ifndef _ASUS_NVRAM_H
#define _ASUS_NVRAM_H

char *nvram_get(const char *name);
char *nvram_safe_get(const char *name);
int nvram_get_int(const char *name);
int nvram_match(const char *name, const char *value);
int nvarm_set_cmd(const char *arg);
int nvram_set(const char *name, const char *value);
int nvram_commit();

#endif
