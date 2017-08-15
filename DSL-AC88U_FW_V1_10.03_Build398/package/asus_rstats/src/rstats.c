/*

	rstats
	Copyright (C) 2006-2009 Jonathan Zarate


	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <sys/stat.h>
#include <stdint.h>
#include <syslog.h>
#include <ctype.h>

//#include <bcmnvram.h>
//#include <shutils.h>

//#define DEBUG_NOISY
//#define DEBUG_STIME

//#include <shared.h>

/******** Traffic Monitor ********/
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/file.h>
#include "asus_nvram.h"
#include "rstats.h"
/*********************************/

//#define DEBUG
#define K 1024
#define M (1024 * 1024)
#define G (1024 * 1024 * 1024)

#define SMIN	60
#define	SHOUR	(60 * 60)
#define	SDAY	(60 * 60 * 24)
#define Y2K		946684800UL

#define INTERVAL		30
#if defined(RTCONFIG_WANPORT2)
#define MAX_BW			2000
#else
#define MAX_BW			1000
#endif

#define MAX_NSPEED		((24 * SHOUR) / INTERVAL)
#define MAX_NDAILY		62
#define MAX_NMONTHLY	25
/* ETHERNETWAN:	1
 * ATM:			1
 * PTM:			1
 * WIRED:		4
 * BRIDGE:		1
 * WIFI_2G:		4
 * WIFI_5G:		4
 */
#define MAX_SPEED_IF	17

#define MAX_ROLLOVER	(MAX_BW * INTERVAL / 8ULL * M)

#define MAX_COUNTER	2
#define RX 			0
#define TX 			1

#define DAILY		0
#define MONTHLY		1

#define ID_V0		0x30305352
#define ID_V1		0x31305352
#define CURRENT_ID	ID_V1

#define HI_BACK		5

#define RA_OFFSET_ISP_METER	0x4FF00

#define MAX_IF_NUM 4

enum if_id {
	IFID_WIRED = 0,		/* WIRED: eth1, eth2, eth3, eht4 */
	IFID_WIRELESS0,		/* WIRELESS0, WIRELESS0.1, WIRELESS0.2, WIRELESS0.3 */
	IFID_WIRELESS1,		/* WIRELESS1, WIRELESS1.1, WIRELESS1.2, WIRELESS1.3 */
	IFID_ATM,			/* ADSL(ATM) */
	IFID_PTM,			/* VDSL(PTM) */
	IFID_ETHERNETWAN,	/* ETHERNETWAN: eth0 */
	IFID_BRIDGE,		/* BRIDGE: br0 */

	IFID_MAX
};

typedef struct {
	uint32_t xtime;
	uint64_t counter[MAX_COUNTER];
} data_t;

typedef struct {
	uint32_t id;

	data_t daily[MAX_NDAILY];
	int dailyp;

	data_t monthly[MAX_NMONTHLY];
	int monthlyp;
} history_t;

typedef struct {
	uint32_t id;

	data_t daily[62];
	int dailyp;

	data_t monthly[12];
	int monthlyp;
} history_v0_t;

typedef struct {
	char ifname[12];
	long utime;
	unsigned long long speed[MAX_NSPEED][MAX_COUNTER];
	unsigned long long last[MAX_COUNTER];
	int tail;
	int sync;
} speed_t;

history_t history;
speed_t speed[MAX_SPEED_IF];
int speed_count;
long save_utime;
char save_path[96];
long current_uptime;

volatile int gothup = 0;
volatile int gotuser = 0;
volatile int gotterm = 0;

const char history_fn[] = "/var/tmp/rstats-history";
const char speed_fn[] = "/var/tmp/rstats-speed";
const char uncomp_fn[] = "/var/tmp/rstats-uncomp";
const char source_fn[] = "/var/tmp/rstats-source";

static int get_stime(void)
{
#ifdef DEBUG_STIME
	return 90;
#else
	int t;
	t = nvram_get_int("rstats_stime");
	if (t < 1) t = 1;
		else if (t > 8760) t = 8760;
	return t * SHOUR;
#endif
}

static int comp(const char *path, void *buffer, int size)
{
	char s[256];

	if (f_write(path, buffer, size, 0, 0) != size) return 0;

	sprintf(s, "%s.gz", path);
	unlink(s);

	sprintf(s, "gzip %s", path);
	return system(s) == 0;
}

static void save(int quick)
{
	int i;
	char *bi, *bo;
	int n;
	int b;
	char hgz[256];
	char tmp[256];
	char bak[256];
	char bkp[256];
	time_t now;
	struct tm *tms;
	static int lastbak = -1;

	//_dprintf("%s: quick=%d\n", __FUNCTION__, quick);

	f_write("/var/tmp/rstats-stime", &save_utime, sizeof(save_utime), 0, 0);

	comp(speed_fn, speed, sizeof(speed[0]) * speed_count);

/*
	if ((now = time(0)) < Y2K) {
		_dprintf("%s: time not set\n", __FUNCTION__);
		return;
	}
*/

	comp(history_fn, &history, sizeof(history));

	//_dprintf("%s: write source=%s\n", __FUNCTION__, save_path);
	f_write_string(source_fn, save_path, 0, 0);

	if (quick) {
		return;
	}

	sprintf(hgz, "%s.gz", history_fn);

	if (strcmp(save_path, "*nvram") == 0) {
		if (!wait_action_idle(10)) {
			_dprintf("%s: busy, not saving\n", __FUNCTION__);
			return;
		}

		if ((n = f_read_alloc(hgz, &bi, 20 * 1024)) > 0) {
			if ((bo = malloc(base64_encoded_len(n) + 1)) != NULL) {
				n = base64_encode((unsigned char *) bi, bo, n);
				bo[n] = 0;
				nvram_set("rstats_data", bo);
				if (!nvram_match("debug_nocommit", "1")) nvram_commit();

				_dprintf("%s: nvram commit\n", __FUNCTION__);

				free(bo);
			}
		}
		free(bi);
	}
	else if (save_path[0] != 0) {
		strcpy(tmp, save_path);
		strcat(tmp, ".tmp");

		for (i = 15; i > 0; --i) {
			if (!wait_action_idle(10)) {
				_dprintf("%s: busy, not saving\n", __FUNCTION__);
			}
			else {
				_dprintf("%s: cp %s %s\n", __FUNCTION__, hgz, tmp);
				if (eval("cp", hgz, tmp) == 0) {
					_dprintf("%s: copy ok\n", __FUNCTION__);

					if (!nvram_match("rstats_bak", "0")) {
						now = time(0);
						tms = localtime(&now);
						if (lastbak != tms->tm_yday) {
							strcpy(bak, save_path);
							n = strlen(bak);
							if ((n > 3) && (strcmp(bak + (n - 3), ".gz") == 0)) n -= 3;
							strcpy(bkp, bak);
							for (b = HI_BACK-1; b > 0; --b) {
								sprintf(bkp + n, "_%d.bak", b + 1);
								sprintf(bak + n, "_%d.bak", b);
								rename(bak, bkp);
							}
							if (eval("cp", "-p", save_path, bak) == 0) lastbak = tms->tm_yday;
						}
					}

					_dprintf("%s: rename %s %s\n", __FUNCTION__, tmp, save_path);
					if (rename(tmp, save_path) == 0) {
						_dprintf("%s: rename ok\n", __FUNCTION__);
						break;
					}
				}
			}

			// might not be ready
			sleep(3);
			if (gotterm) break;
		}
	}
}

static int decomp(const char *fname, void *buffer, int size, int max)
{
	char s[256];
	int n;
	FILE *fp;
	long file_size = 0;

	_dprintf("%s: fname=%s\n", __FUNCTION__, fname);

	unlink(uncomp_fn);

	n = 0;
	sprintf(s, "gzip -dc %s > %s", fname, uncomp_fn);
	if (system(s)) {
		_dprintf("%s: %s != 0\n", __func__, s);
		goto exit_decomp;
	}
	if (!(fp = fopen(uncomp_fn, "r")))
		goto exit_decomp;

	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);
	fclose(fp);
	if ((size * max) != file_size) {
		_dprintf("%s: filesize mismatch! (%ld/%ld)\n", __FUNCTION__, (long)(size * max), file_size);
		goto exit_decomp;
	}

	n = f_read(uncomp_fn, buffer, size * max);
	_dprintf("%s: n=%d\n", __func__, n);
	if (n <= 0)
		n = 0;
	else
		n = n / size;

exit_decomp:
	unlink(uncomp_fn);
	memset((char *)buffer + (size * n), 0, (max - n) * size);

	return n;
}

static void clear_history(void)
{
	memset(&history, 0, sizeof(history));
	history.id = CURRENT_ID;
}

static int load_history(const char *fname)
{
	history_t hist;

	_dprintf("%s: fname=%s\n", __FUNCTION__, fname);

	if ((decomp(fname, &hist, sizeof(hist), 1) != 1) || (hist.id != CURRENT_ID)) {
		history_v0_t v0;

		if ((decomp(fname, &v0, sizeof(v0), 1) != 1) || (v0.id != ID_V0)) {
			_dprintf("%s: load failed\n", __FUNCTION__);
			return 0;
		}
		else {
			// --- temp conversion ---
			clear_history();

			// V0 -> V1
			history.id = CURRENT_ID;
			memcpy(history.daily, v0.daily, sizeof(history.daily));
			history.dailyp = v0.dailyp;
			memcpy(history.monthly, v0.monthly, sizeof(v0.monthly));	// v0 is just shorter
			history.monthlyp = v0.monthlyp;
		}
	}
	else {
		memcpy(&history, &hist, sizeof(history));
	}

	_dprintf("%s: dailyp=%d monthlyp=%d\n", __FUNCTION__, history.dailyp, history.monthlyp);
	return 1;
}

/* Try loading from the backup versions.
 * We'll try from oldest to newest, then
 * retry the requested one again last.  In case the drive mounts while
 * we are trying to find a good version.
 */
static int try_hardway(const char *fname)
{
	char fn[256];
	int n, b, found = 0;

	strcpy(fn, fname);
	n = strlen(fn);
	if ((n > 3) && (strcmp(fn + (n - 3), ".gz") == 0))
		n -= 3;
	for (b = HI_BACK; b > 0; --b) {
		sprintf(fn + n, "_%d.bak", b);
		found |= load_history(fn);
	}
	found |= load_history(fname);

	return found;
}

static void load_new(void)
{
	char hgz[256];

	sprintf(hgz, "%s.gz.new", history_fn);
	if (load_history(hgz)) save(0);
	unlink(hgz);
}

static void load(int new)
{
	int i;
	long t;
	char *bi, *bo;
	int n;
	char hgz[256];
	char sp[sizeof(save_path)];
	unsigned char mac[6];

	current_uptime = uptime();

	strlcpy(save_path, nvram_safe_get("rstats_path"), sizeof(save_path) - 32);
	if (((n = strlen(save_path)) > 0) && (save_path[n - 1] == '/')) {
#ifdef RTCONFIG_RGMII_BRCM5301X
		ether_atoe(nvram_safe_get("et1macaddr"), mac);
#else
		ether_atoe(nvram_safe_get("et0macaddr"), mac);
#endif
#ifdef RTCONFIG_GMAC3
        	if(nvram_match("gmac3_enable", "1"))
			ether_atoe(nvram_safe_get("et2macaddr"), mac);
#endif
		sprintf(save_path + n, "tomato_rstats_%02x%02x%02x%02x%02x%02x.gz",
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	}

	if (f_read("/var/tmp/rstats-stime", &save_utime, sizeof(save_utime)) != sizeof(save_utime)) {
		save_utime = 0;
	}
	t = current_uptime + get_stime();
	if ((save_utime < current_uptime) || (save_utime > t)) save_utime = t;
	//_dprintf("%s: uptime = %ldm, save_utime = %ldm\n", __FUNCTION__, (current_uptime / 60), (save_utime / 60));

	//

	sprintf(hgz, "%s.gz", speed_fn);
	speed_count = decomp(hgz, speed, sizeof(speed[0]), MAX_SPEED_IF);
	_dprintf("%s: speed_count = %d\n", __FUNCTION__, speed_count);

	for (i = 0; i < speed_count; ++i) {
		if (speed[i].utime > current_uptime) {
			speed[i].utime = current_uptime;
			speed[i].sync = 1;
		}
	}

	//

	sprintf(hgz, "%s.gz", history_fn);

	if (new) {
		unlink(hgz);
		save_utime = 0;
		return;
	}

	f_read_string(source_fn, sp, sizeof(sp));	// always terminated
	_dprintf("%s: read source=%s save_path=%s\n", __FUNCTION__, sp, save_path);
	if ((strcmp(sp, save_path) == 0) && (load_history(hgz))) {
		_dprintf("%s: using local file\n", __FUNCTION__);
		return;
	}

	if (save_path[0] != 0) {
		if (strcmp(save_path, "*nvram") == 0) {
			if (!wait_action_idle(60)) exit(0);

			bi = nvram_safe_get("rstats_data");
			if ((n = strlen(bi)) > 0) {
				if ((bo = malloc(base64_decoded_len(n))) != NULL) {
					n = base64_decode(bi, (unsigned char *) bo, n);
					_dprintf("%s: nvram n=%d\n", __FUNCTION__, n);
					f_write(hgz, bo, n, 0, 0);
					free(bo);
					load_history(hgz);
				}
			}
		}
		else {
			i = 1;
			while (1) {
				if (wait_action_idle(10)) {

					// cifs quirk: try forcing refresh
					eval("ls", save_path);

					/* If we can't access the path, keep trying - maybe it isn't mounted yet.
					 * If we can, and we can sucessfully load it, oksy.
					 * If we can, and we cannot load it, then maybe it has been deleted, or
					 * maybe it's corrupted (like 0 bytes long).
					 * In these cases, try the backup files.
					 */
					if (load_history(save_path) || try_hardway(save_path)) {
						f_write_string(source_fn, save_path, 0, 0);
						break;
					}
				}

				// not ready...
				sleep(i);
				if ((i *= 2) > 900) i = 900;	// 15m

				if (gotterm) {
					save_path[0] = 0;
					return;
				}

				if (i > (3 * 60)) {
					syslog(LOG_WARNING, "Problem loading %s. Still trying...", save_path);
				}
			}
		}
	}
}

static void save_speedjs(long next)
{
	int i, j, k;
	speed_t *sp;
	int p;
	FILE *f;
	uint64_t total;
	uint64_t tmax;
	unsigned long n;
	char c;

	if ((f = fopen("/var/tmp/rstats-speed.js", "w")) == NULL) return;

	_dprintf("%s: speed_count = %d\n", __FUNCTION__, speed_count);

	fprintf(f, "\nspeed_history = {\n");

	for (i = 0; i < speed_count; ++i) {
		sp = &speed[i];
		fprintf(f, "%s'%s': {\n", i ? " },\n" : "", sp->ifname);
		for (j = 0; j < MAX_COUNTER; ++j) {
			total = tmax = 0;
			fprintf(f, "%sx: [", j ? ",\n t" : " r");
			p = sp->tail;
			for (k = 0; k < MAX_NSPEED; ++k) {
				p = (p + 1) % MAX_NSPEED;
				n = sp->speed[p][j];
				fprintf(f, "%s%lu", k ? "," : "", n);
				total += n;
				if (n > tmax) tmax = n;
			}
			fprintf(f, "],\n");

			c = j ? 't' : 'r';
			fprintf(f, " %cx_avg: %llu,\n %cx_max: %llu,\n %cx_total: %llu",
				c, total / MAX_NSPEED, c, tmax, c, total);
		}
	}
	fprintf(f, "%s_next: %ld};\n", speed_count ? "},\n" : "", ((next >= 1) ? next : 1));
	fclose(f);
	_dprintf("%s: write /var/tmp/rstats-speed.js end\n", __FUNCTION__);
	//rename("/var/tmp/rstats-speed.js", "/var/spool/rstats-speed.js");
}


static void save_datajs(FILE *f, int mode)
{
	data_t *data;
	int p;
	int max;
	int k, kn;

_dprintf("save_datajs:\n");
	fprintf(f, "\n%s_history = [\n", (mode == DAILY) ? "daily" : "monthly");

	if (mode == DAILY) {
		data = history.daily;
		p = history.dailyp;
		max = MAX_NDAILY;
_dprintf("DAILY: p= %d\n", p);
	}
	else {
		data = history.monthly;
		p = history.monthlyp;
		max = MAX_NMONTHLY;
_dprintf("MONTHLY: p= %d\n", p);
	}
	kn = 0;
	for (k = max; k > 0; --k) {
		p = (p + 1) % max;
		if (data[p].xtime == 0) continue;
		fprintf(f, "%s[0x%lx,0x%llx,0x%llx]", kn ? "," : "",
			(unsigned long)data[p].xtime, data[p].counter[0] / K, data[p].counter[1] / K);
		++kn;
_dprintf("%d:: [0x%lx,0x%llx,0x%llx]\n", p, 
	(unsigned long)data[p].xtime, data[p].counter[0] / K, data[p].counter[1] / K);
	}
	fprintf(f, "];\n");
}

static void save_histjs(void)
{
	FILE *f;

	if ((f = fopen("/var/tmp/rstats-history.js", "w")) != NULL) {
		save_datajs(f, DAILY);
		save_datajs(f, MONTHLY);
		fclose(f);
		//_dprintf("%s: write /var/tmp/rstats-history.js end\n", __FUNCTION__);
		//rename("/var/tmp/rstats-history.js", "/var/spool/rstats-history.js");
	}
}


static void bump(data_t *data, int *tail, int max, uint32_t xnow, unsigned long long *counter)
{
	int t, i;
	t = *tail;
	if (data[t].xtime != xnow) {
		for (i = max - 1; i >= 0; --i) {
			if (data[i].xtime == xnow) {
				t = i;
				break;
			}
		}
		if (i < 0) {
			*tail = t = (t + 1) % max;
			data[t].xtime = xnow;
			memset(data[t].counter, 0, sizeof(data[0].counter));
		}
	}
	for (i = 0; i < MAX_COUNTER; ++i) {
		data[t].counter[i] += counter[i];
	}
}

/**
 * Convert ifname_desc returned by netdev_calc() to if_id enumeration.
 * @desc:	Pointer to "INTERNET", "WIRED", "BRIDGE", etc
 * 		Ref to netdev_calc().
 * @return:
 * 	-1:	invalid parameter
 *  if_id	enumeration
 */
static enum if_id desc_to_id(char *desc)
{
	enum if_id id = IFID_MAX;

	if (!desc)
		return -1;

	if (!strcmp(desc, "ETHERNETWAN"))
		id = IFID_ETHERNETWAN;
	else if (!strcmp(desc, "ATM"))
		id = IFID_ATM;
	else if (!strcmp(desc, "PTM"))
		id = IFID_PTM;
	else if (!strcmp(desc, "WIRED"))
		id = IFID_WIRED;
	else if (!strcmp(desc, "BRIDGE"))
		id = IFID_BRIDGE;
	else if (!strncmp(desc, "WIRELESS0", 9))
		id = IFID_WIRELESS0;
	else if (!strncmp(desc, "WIRELESS1", 9))
		id = IFID_WIRELESS1;

	if (id < 0 || id == IFID_MAX)
		_dprintf("%s: Unknown desc [%s]\n", __func__, desc);

	return id;

}

static void calc(void)
{
	FILE *f;
	char buf[256], cmd[128];
	char *ifname;
	char ifname_desc[12], ifname_desc2[12];
	char *p;
	unsigned long long counter[MAX_COUNTER];
	unsigned long long rx2, tx2;
	speed_t *sp;
	int i, j, t;
	time_t now;
	time_t mon;
	struct tm *tms;
	uint32_t c;
	uint32_t sc;
	unsigned long long diff;
	long tick;
	int n;
	char exclude[512];
	enum if_id id;
	struct tmp_speed_s {
		char desc[20];
		unsigned long long counter[MAX_COUNTER];
	} tmp_speed[IFID_MAX], *tmp;

	rx2 = 0;
	tx2 = 0;
	now = time(0);
	snprintf(exclude, sizeof(exclude), "%s", nvram_safe_get("rstats_exclude"));

	memset(tmp_speed, 0, sizeof(tmp_speed));
	/* for DSL-AC87VG WiFi 5GHz */
#ifdef SUPPORT_QTN_CMD
	for (j = 0; j < MAX_IF_NUM; j++) 
	{
		memset(ifname_desc, 0, sizeof(ifname_desc));
		counter[0] = 0;
		counter[1] = 0;
	
		strcpy(ifname_desc, "WIRELESS1");

		snprintf(cmd, sizeof(cmd), "call_qcsapi_sockrpc get_interface_stats wifi%d", j);
		if ((f = popen(cmd, "r")) == NULL) continue;
		while (fgets(buf, sizeof(buf), f) != NULL)
		{
			if (!strncmp(buf, "rx_bytes:", 9))
			{
				sscanf(buf, "rx_bytes:%llu", &counter[0]);
			}
			else if (!strncmp(buf, "tx_bytes:", 9))
			{
				sscanf(buf, "tx_bytes:%llu", &counter[1]);
			}
		}
		pclose(f);
		_dprintf(">>> calc_5g[wifi%d]: %s, %llu, %llu <<<\n", j, ifname_desc, counter[0], counter[1]);

		id = desc_to_id(ifname_desc);
		if (id < 0 || id >= IFID_MAX)
			continue;
		tmp = &tmp_speed[id];
		strcpy(tmp->desc, ifname_desc);
		for (i = 0; i < ARRAY_SIZE(tmp->counter); ++i){
			tmp->counter[i] += counter[i];
		}
	}
#endif
	if ((f = fopen("/proc/net/dev", "r")) == NULL) return;
	fgets(buf, sizeof(buf), f);	// header
	fgets(buf, sizeof(buf), f);	// "
	while (fgets(buf, sizeof(buf), f)) {
		if ((p = strchr(buf, ':')) == NULL) continue;
			//_dprintf("=== %s", buf);
		*p = 0;
		if ((ifname = strrchr(buf, ' ')) == NULL) ifname = buf;
			else ++ifname;
		if ((strcmp(ifname, "lo") == 0) || (find_word(exclude, ifname))){
			_dprintf("%s: [%s] is lo or exclude\n", __func__, ifname);
			continue;
		}

		// <rx bytes, packets, errors, dropped, fifo errors, frame errors, compressed, multicast><tx ...>
		if (sscanf(p + 1, "%llu%*u%*u%*u%*u%*u%*u%*u%llu", &counter[0], &counter[1]) != 2){
			//_dprintf("%s: %s can't find counter[0] and counter[1]\n", __func__, ifname);
			continue;
		}

		if (!netdev_calc(ifname, ifname_desc, (unsigned long*) &counter[0], (unsigned long*) &counter[1], ifname_desc2, (unsigned long*) &rx2, (unsigned long*) &tx2))
			continue;
		_dprintf(">>> %s, %s, %llu, %llu, %s, %llu, %llu <<<\n",ifname, ifname_desc, counter[0], counter[1], ifname_desc2, rx2, tx2);

		id = desc_to_id(ifname_desc);
		if (id < 0 || id >= IFID_MAX)
			continue;
		tmp = &tmp_speed[id];
		strcpy(tmp->desc, ifname_desc);
		for (i = 0; i < ARRAY_SIZE(tmp->counter); ++i){
			tmp->counter[i] += counter[i];
		}
	}
	fclose(f);

	for (t = 0, tmp = tmp_speed; t < ARRAY_SIZE(tmp_speed); ++t, ++tmp) {
		/* skip unused item. */
		if (tmp->desc[0] == '\0')
			continue;

		sp = speed;
		for (i = speed_count; i > 0; --i) {
			if (strcmp(sp->ifname, tmp->desc) == 0) break;
			++sp;
		}

		if (i == 0) {
			if (speed_count >= MAX_SPEED_IF) continue;

			_dprintf("%s: add %s as #%d\n", __FUNCTION__, tmp->desc, speed_count);

			i = speed_count++;
			sp = &speed[i];
			memset(sp, 0, sizeof(*sp));
			strcpy(sp->ifname, tmp->desc);
			sp->sync = 1;
			sp->utime = current_uptime;
		}
		if (sp->sync) {
			_dprintf("%s: sync %s\n", __FUNCTION__, tmp->desc);
			sp->sync = -1;

			memcpy(sp->last, tmp->counter, sizeof(sp->last));
			memset(tmp->counter, 0, sizeof(tmp->counter));
		}
		else {

			sp->sync = -1;

			tick = (current_uptime - sp->utime);
			n = (tick / INTERVAL);
			if (n < 1) {
				_dprintf("%s: %s is a little early... %d < %d\n", __FUNCTION__, tmp->desc, tick, INTERVAL);
				continue;
			}
	
			sp->utime += (n * INTERVAL);
			_dprintf("%s: %s n=%d tick=%ld, sp->utime=%ld\n", __FUNCTION__, sp->ifname, n, tick, sp->utime);

			for (i = 0; i < MAX_COUNTER; ++i) {
				c = tmp->counter[i];
				sc = sp->last[i];
				if (c < sc) {
					diff = ((0xFFFFFFFF - sc + 1) + c);
					if (diff > MAX_ROLLOVER) diff = 0;
				}
				else {
					 diff = (c - sc);
				}
				sp->last[i] = c;
				tmp->counter[i] = diff;
			}

			for (j = 0; j < n; ++j) {
				sp->tail = (sp->tail + 1) % MAX_NSPEED;
				for (i = 0; i < MAX_COUNTER; ++i) {
					sp->speed[sp->tail][i] = (tmp->counter[i] / n);
				}
			}
		}		

		// todo: split, delay

		if (now > Y2K && (strcmp(tmp->desc, "ETHERNETWAN")==0 || 
							strcmp(tmp->desc, "ATM")==0 || 
							strcmp(tmp->desc, "PTM")==0)) {
			/* Skip this if the time&date is not set yet */
			/* Skip non-INTERNET interface only 	     */
			tms = localtime(&now);
			bump(history.daily, &history.dailyp, MAX_NDAILY,
				(tms->tm_year << 16) | ((uint32_t)tms->tm_mon << 8) | tms->tm_mday, tmp->counter);
			//n = nvram_get_int("rstats_offset");
			n = 1; /* DSL-AC87VG not support rstats_offset */
			if ((n < 1) || (n > 31)) n = 1;
			mon = now + ((1 - n) * (60 * 60 * 24));
			tms = localtime(&mon);
			bump(history.monthly, &history.monthlyp, MAX_NMONTHLY,
				(tms->tm_year << 16) | ((uint32_t)tms->tm_mon << 8), tmp->counter);
		}
	}
			
	// cleanup stale entries
	for (i = 0; i < speed_count; ++i) {
		sp = &speed[i];
		if (sp->sync == -1) {
			sp->sync = 0;
			continue;
		}
		if (((current_uptime - sp->utime) > (10 * SMIN)) || (find_word(exclude, sp->ifname))) {
			_dprintf("%s: #%d removing. > time limit or excluded\n", __FUNCTION__, i);
			--speed_count;
			memcpy(sp, sp + 1, (speed_count - i) * sizeof(speed[0]));
		}
		else {
			_dprintf("%s: %s not found setting sync=1, #%d\n", __FUNCTION__, sp->ifname, i);
			sp->sync = 1;
		}
	}

	// todo: total > user
	if (current_uptime >= save_utime) {
		save(0);
		save_utime = (current_uptime + get_stime());
		_dprintf("%s: uptime = %ldm, save_utime = %ldm\n", __FUNCTION__, (current_uptime / 60), (save_utime / 60));
	}
}

static void sig_handler(int sig)
{
	switch (sig) {
	case SIGTERM:
	case SIGINT:
		gotterm = 1;
		break;
	case SIGHUP:
		gothup = 1;
		break;
	case SIGUSR1:
		gotuser = 1;
		break;
	case SIGUSR2:
		gotuser = 2;
		break;
	}
}

int main(int argc, char *argv[])
{
	struct sigaction sa;
	long z;
	int new;

	printf("rstats\nCopyright (C) 2006-2009 Jonathan Zarate\n\n");

	if (fork() != 0) return 0;

	openlog("rstats", LOG_PID, LOG_USER);

	new = 0;
	if (argc > 1) {
		if (strcmp(argv[1], "--new") == 0) {
			new = 1;
			_dprintf("new=1\n");
		}
	}

	clear_history();
	unlink("/var/tmp/rstats-load");

	sa.sa_handler = sig_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	sigaction(SIGHUP, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	sigaction(SIGINT, &sa, NULL);
	load(new);

	z = current_uptime = uptime();

	while (1) {
		while (current_uptime < z) {
			sleep(z - current_uptime);
			if (gothup) {
				if (unlink("/var/tmp/rstats-load") == 0) load_new();
					else save(0);
				gothup = 0;
			}
			if (gotterm) {
				//save(!nvram_match("rstats_sshut", "1"));
				save(0); /* DSL-AC87VG not support rstats_sshut */
				exit(0);
			}
			if (gotuser == 1) {
				save_speedjs(z - uptime());
				gotuser = 0;
			}
			else if (gotuser == 2) {
				save_histjs();
				gotuser = 0;
			}
			current_uptime = uptime();
		}
		_dprintf("%s: current_uptime=%ld, z=%ld\n", __FUNCTION__, current_uptime, z);
		calc();
		/* just in order to do in while loop for INTERVAL time. And fix the calc() hanging issue. */
		if (current_uptime > z)
			z = (current_uptime + INTERVAL);
		else
			z += INTERVAL;
	}

	return 0;
}

/******** Traffic Monitor ********/
unsigned long f_size(const char *path)	// 4GB-1	-1 = error
{
	struct stat st;
	if (stat(path, &st) == 0) return st.st_size;
	return (unsigned long)-1;
}

int f_read_excl(const char *path, void *buffer, int max)
{
	int f;
	int n;

	if ((f = open(path, O_RDONLY)) < 0) return -1;
	flock(f, LOCK_EX);
	n = read(f, buffer, max);
	flock(f, LOCK_UN);
	close(f);
	return n;
}

int f_read(const char *path, void *buffer, int max)
{
	int f;
	int n;

	if ((f = open(path, O_RDONLY)) < 0) return -1;
	n = read(f, buffer, max);
	close(f);
	return n;
}

int f_write(const char *path, const void *buffer, int len, unsigned flags, unsigned cmode)
{
	static const char nl = '\n';
	int f;
	int r = -1;
	mode_t m;

	m = umask(0);
	if (cmode == 0) cmode = 0666;
	if ((f = open(path, (flags & FW_APPEND) ? (O_WRONLY|O_CREAT|O_APPEND) : (O_WRONLY|O_CREAT|O_TRUNC), cmode)) >= 0) {
		if ((buffer == NULL) || ((r = write(f, buffer, len)) == len)) {
			if (flags & FW_NEWLINE) {
				if (write(f, &nl, 1) == 1) ++r;
			}
		}
		close(f);
	}
	umask(m);
	return r;
}

int f_read_string(const char *path, char *buffer, int max)
{
	if (max <= 0) return -1;
	int n = f_read(path, buffer, max - 1);
	buffer[(n > 0) ? n : 0] = 0;
	return n;
}

int f_write_string(const char *path, const char *buffer, unsigned flags, unsigned cmode)
{
	return f_write(path, buffer, strlen(buffer), flags, cmode);
}

static int _f_read_alloc(const char *path, char **buffer, int max, int z)
{
	unsigned long n;

	*buffer = NULL;
	if (max >= 0) {
		if ((n = f_size(path)) != (unsigned long)-1) {
			if (n < max) max = n;
			if ((!z) && (max == 0)) return 0;
			if ((*buffer = malloc(max + z)) != NULL) {
				if ((max = f_read(path, *buffer, max)) >= 0) {
					if (z) *(*buffer + max) = 0;
					return max;
				}
				free(buffer);
			}
		}
	}
	return -1;
}

int f_read_alloc(const char *path, char **buffer, int max)
{
	return _f_read_alloc(path, buffer, max, 0);
}

static int __check_action(struct action_s *pa)
{
	int r = 3;
	struct action_s act;

	while (f_read_excl(ACTION_LOCK, &act, sizeof(act)) != sizeof(act)) {
		sleep(1);
		if (--r == 0) return ACT_UNKNOWN;
	}
	if (pa)
		*pa = act;
	_dprintf("%d: check_action %d\n", getpid(), act.action);

	return act.action;
}

int process_exists(pid_t pid)
{
	return (kill(pid, 0) == 0 || errno != ESRCH);
}

int wait_action_idle(int n)
{
	int r;
	struct action_s act;

	while (n-- > 0) {
		act.pid = 0;
		if (__check_action(&act) == ACT_IDLE) return 1;
		if (act.pid > 0 && !process_exists(act.pid)) {
			if (!(r = unlink(ACTION_LOCK)) || errno == ENOENT) {
				_dprintf("Terminated process, pid %d %s, hold action lock %d !!!\n",
					act.pid, act.comm, act.action);
				return 1;
			}
			_dprintf("Remove " ACTION_LOCK " failed. errno %d (%s)\n", errno, strerror(errno));
		}
		sleep(1);
	}
	_dprintf("pid %d %s hold action lock %d !!!\n", act.pid, act.comm, act.action);
	return 0;
}

static const char base64_xlat[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
int base64_encode(const unsigned char *in, char *out, int inlen)
{
	char *o;

	o = out;
	while (inlen >= 3) {
		*out++ = base64_xlat[*in >> 2];
		*out++ = base64_xlat[((*in << 4) & 0x3F) | (*(in + 1) >> 4)];
		++in;	// note: gcc complains if *(++in)
		*out++ = base64_xlat[((*in << 2) & 0x3F) | (*(in + 1) >> 6)];
		++in;
		*out++ = base64_xlat[*in++ & 0x3F];
		inlen -= 3;
	}
	if (inlen > 0) {
		*out++ = base64_xlat[*in >> 2];
		if (inlen == 2) {
			*out++ = base64_xlat[((*in << 4) & 0x3F) | (*(in + 1) >> 4)];
			++in;
			*out++ = base64_xlat[((*in << 2) & 0x3F)];
		}
		else {
			*out++ = base64_xlat[(*in << 4) & 0x3F];
			*out++ = '=';
		}
		*out++ = '=';
	}
	return out - o;
}

int base64_decode(const char *in, unsigned char *out, int inlen)
{
	char *p;
	int n;
	unsigned long x;
	unsigned char *o;
	char c;

	o = out;
	n = 0;
	x = 0;
	while (inlen-- > 0) {
		if (*in == '=') break;
		if ((p = strchr(base64_xlat, c = *in++)) == NULL) {
//			printf("ignored - %x %c\n", c, c);
			continue;	// note: invalid characters are just ignored
		}
		x = (x << 6) | (p - base64_xlat);
		if (++n == 4) {
			*out++ = x >> 16;
			*out++ = (x >> 8) & 0xFF;
			*out++ = x & 0xFF;
			n = 0;
			x = 0;
		}
	}
	if (n == 3) {
		*out++ = x >> 10;
		*out++ = (x >> 2) & 0xFF;
	}
	else if (n == 2) {
		*out++ = x >> 4;
	}
	return out - o;
}

int base64_encoded_len(int len)
{
	return ((len + 2) / 3) * 4;
}

int base64_decoded_len(int len)
{
	return ((len + 3) / 4) * 3;
}

long uptime(void)
{
	struct sysinfo info;
	sysinfo(&info);
	
	return info.uptime;
}

void chld_reap(int sig)
{
	while (waitpid(-1, NULL, WNOHANG) > 0) {}
}

/*
 * Concatenates NULL-terminated list of arguments into a single
 * commmand and executes it
 * @param	argv	argument list
 * @param	path	NULL, ">output", or ">>output"
 * @param	timeout	seconds to wait before timing out or 0 for no timeout
 * @param	ppid	NULL to wait for child termination or pointer to pid
 * @return	return value of executed command or errno
 *
 * Ref: http://www.open-std.org/jtc1/sc22/WG15/docs/rr/9945-2/9945-2-28.html
 */
int _eval(char *const argv[], const char *path, int timeout, int *ppid)
{
	sigset_t set, sigmask;
	__sighandler_t chld = SIG_IGN;
	pid_t pid, w;
	int status = 0;
	int fd;
	int flags;
	int sig;
	int n;
	const char *p;
	char s[256];
	//char *cpu0_argv[32] = { "taskset", "-c", "0"};
	//char *cpu1_argv[32] = { "taskset", "-c", "1"};

	if (!ppid) {
		// block SIGCHLD
		sigemptyset(&set);
		sigaddset(&set, SIGCHLD);
		sigprocmask(SIG_BLOCK, &set, &sigmask);
		// without this we cannot rely on waitpid() to tell what happened to our children
		chld = signal(SIGCHLD, SIG_DFL);
	}

	pid = fork();
	if (pid == -1) {
		perror("fork");
		status = errno;
		goto EXIT;
	}
	if (pid != 0) {
		// parent
		if (ppid) {
			*ppid = pid;
			return 0;
		}
		do {
			if ((w = waitpid(pid, &status, 0)) == -1) {
				status = errno;
				perror("waitpid");
				goto EXIT;
			}
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));

		if (WIFEXITED(status)) status = WEXITSTATUS(status);
EXIT:
		if (!ppid) {
			// restore signals
			sigprocmask(SIG_SETMASK, &sigmask, NULL);
			signal(SIGCHLD, chld);
			// reap zombies
			chld_reap(0);
		}
		return status;
	}
	
	// child

	// reset signal handlers
	for (sig = 0; sig < (_NSIG - 1); sig++)
		signal(sig, SIG_DFL);

	// unblock signals if called from signal handler
	sigemptyset(&set);
	sigprocmask(SIG_SETMASK, &set, NULL);

	setsid();

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	open("/dev/null", O_RDONLY);
	open("/dev/null", O_WRONLY);
	open("/dev/null", O_WRONLY);

	if (nvram_match("debug_logeval", "1")) {
		pid = getpid();

		cprintf("_eval +%ld pid=%d ", uptime(), pid);
		for (n = 0; argv[n]; ++n) cprintf("%s ", argv[n]);
		cprintf("\n");
		
		if ((fd = open("/dev/console", O_RDWR | O_NONBLOCK)) >= 0) {
			dup2(fd, STDIN_FILENO);
			dup2(fd, STDOUT_FILENO);
			dup2(fd, STDERR_FILENO);
		}
		else {
			sprintf(s, "/tmp/eval.%d", pid);
			if ((fd = open(s, O_CREAT | O_RDWR | O_NONBLOCK, 0600)) >= 0) {
				dup2(fd, STDOUT_FILENO);
				dup2(fd, STDERR_FILENO);
			}
		}
		if (fd > STDERR_FILENO) close(fd);
	}

	// Redirect stdout & stderr to <path>
	if (path) {
		flags = O_WRONLY | O_CREAT | O_NONBLOCK;
		if (*path == '>') {
			++path;
			if (*path == '>') {
				++path;
				// >>path, append
				flags |= O_APPEND;
			}
			else {
				// >path, overwrite
				flags |= O_TRUNC;
			}
		}
		
		if ((fd = open(path, flags, 0644)) < 0) {
			perror(path);
		}
		else {
			dup2(fd, STDOUT_FILENO);
			dup2(fd, STDERR_FILENO);
			close(fd);
		}
	}

	// execute command

	p = nvram_safe_get("env_path");
	snprintf(s, sizeof(s), "%s%s/sbin:/bin:/usr/sbin:/usr/bin:/opt/sbin:/opt/bin", *p ? p : "", *p ? ":" : "");
	setenv("PATH", s, 1);

	alarm(timeout);
#if 1
	execvp(argv[0], argv);

	perror(argv[0]);
#elif 0
	for(n = 0; argv[n]; ++n)
		cpu0_argv[n+3] = argv[n];
	execvp(cpu0_argv[0], cpu0_argv);

	perror(cpu0_argv[0]);
#else
	for(n = 0; argv[n]; ++n)
		cpu1_argv[n+3] = argv[n];
	execvp(cpu1_argv[0], cpu1_argv);

	perror(cpu1_argv[0]);

#endif

	_exit(errno);
}

/*
 * Convert Ethernet address string representation to binary data
 * @param	a	string in xx:xx:xx:xx:xx:xx notation
 * @param	e	binary data
 * @return	TRUE if conversion was successful and FALSE otherwise
 */
int
ether_atoe(const char *a, unsigned char *e)
{
	char *c = (char *) a;
	int i = 0;

	memset(e, 0, ETHER_ADDR_LEN);
	for (;;) {
		e[i++] = (unsigned char) strtoul(c, &c, 16);
		if (!*c++ || i == ETHER_ADDR_LEN)
			break;
	}
	return (i == ETHER_ADDR_LEN);
}

const char *find_word(const char *buffer, const char *word)
{
	const char *p, *q;
	int n;

	n = strlen(word);
	p = buffer;
	while ((p = strstr(p, word)) != NULL) {
		if ((p == buffer) || (*(p - 1) == ' ') || (*(p - 1) == ',')) {
			q = p + n;
			if ((*q == ' ') || (*q == ',') || (*q == 0)) {
				return p;
			}
		}
		++p;
	}
	return NULL;
}

int backup_rx;
int backup_tx;
int backup_set = 0;

unsigned int netdev_calc(char *ifname, char *ifname_desc, unsigned long *rx, unsigned long *tx, char *ifname_desc2, unsigned long *rx2, unsigned long *tx2)
{
	strcpy(ifname_desc2, "");

	// find in LAN interface
	/* DSL-AC87VG ETHERNET LAN interface */
	if(!strncmp(ifname, "eth1", 4) || !strncmp(ifname, "eth2", 4) || !strncmp(ifname, "eth3", 4) || !strncmp(ifname, "eth4", 4))
	{
		strcpy(ifname_desc, "WIRED");
		return 1;
	}
	/* DSL-AC87VG WIRELESS LAN 2.4G interface */
	else if(!strncmp(ifname, "wl0", 3))
	{
		strcpy(ifname_desc, "WIRELESS0");
		return 1;
	}
	/* DSL-AC88U WIRELESS LAN 5G interface */
	else if(!strncmp(ifname, "wl1", 3))
	{
		strcpy(ifname_desc, "WIRELESS1");
		return 1;
	}
	// find bridge interface
	else if(nvram_match("lan_ifname", ifname))
	{
		strcpy(ifname_desc, "BRIDGE");
		return 1;
	}
	// find WAN interface
	/* DSL-AC87VG WAN interface */
	else if(!strcmp(ifname, "eth0"))
	{
		strcpy(ifname_desc, "ETHERNETWAN");
		return 1;
	}
	else if(!strcmp(ifname, "nas0"))
	{
		strcpy(ifname_desc, "ATM");
		return 1;
	}
	else if(!strcmp(ifname, "ptm0"))
	{
		strcpy(ifname_desc, "PTM");
		return 1;
	}

	//_dprintf("%s: unknown ifname %s\n", __func__, ifname);
	return 0;
}
/*********************************/
