#define ACTION_LOCK		"/var/lock/action"
#define ACT_COMM_LEN	20

#ifndef ETHER_ADDR_LEN
#define	ETHER_ADDR_LEN		6
#endif

//#define DEBUG_RSTATS

#ifdef DEBUG_RSTATS
#define _dprintf		cprintf
#define csprintf		cprintf
#else
#define _dprintf(args...)	do { } while(0)
#define csprintf(args...)	do { } while(0)
#endif

#define cprintf(fmt, args...) do { \
					FILE *fp = fopen("/tmp/rstats.log", "a"); \
					if (fp) { \
						fprintf(fp, fmt, ## args); \
						fclose(fp); \
					} \
				} while (0)

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(ary) (sizeof(ary) / sizeof((ary)[0]))
#endif

enum {
	ACT_IDLE,
	ACT_TFTP_UPGRADE_UNUSED,
	ACT_WEB_UPGRADE,
	ACT_WEBS_UPGRADE_UNUSED,
	ACT_SW_RESTORE,
	ACT_HW_RESTORE,
	ACT_ERASE_NVRAM,
	ACT_NVRAM_COMMIT,
	ACT_REBOOT,
	ACT_UNKNOWN
};

struct action_s {
	char comm[ACT_COMM_LEN];
	pid_t pid;
	int action;
};

#define wif_to_vif(wif) (wif)

#define FW_CREATE	0
#define FW_APPEND	1
#define FW_NEWLINE	2

/* Simple version of _eval() (no timeout and wait for child termination) */
#define eval(cmd, args...) ({ \
	char * const argv[] = { cmd, ## args, NULL }; \
	_eval(argv, NULL, 0, NULL); \
})

/* Copy each token in wordlist delimited by space into word */
#define foreach(word, wordlist, next) \
		for (next = &wordlist[strspn(wordlist, " ")], \
				strncpy(word, next, sizeof(word)), \
				word[strcspn(word, " ")] = '\0', \
				word[sizeof(word) - 1] = '\0', \
				next = strchr(next, ' '); \
				strlen(word); \
				next = next ? &next[strspn(next, " ")] : "", \
				strncpy(word, next, sizeof(word)), \
				word[strcspn(word, " ")] = '\0', \
				word[sizeof(word) - 1] = '\0', \
				next = strchr(next, ' '))

unsigned long f_size(const char *path);
int f_read_excl(const char *path, void *buffer, int max);
int f_read(const char *path, void *buffer, int max);
int f_write(const char *path, const void *buffer, int len, unsigned flags, unsigned cmode);
int f_read_string(const char *path, char *buffer, int max);
int f_write_string(const char *path, const char *buffer, unsigned flags, unsigned cmode);
static int _f_read_alloc(const char *path, char **buffer, int max, int z);
int f_read_alloc(const char *path, char **buffer, int max);
static int __check_action(struct action_s *pa);
int process_exists(pid_t pid);
int wait_action_idle(int n);
int base64_encode(const unsigned char *in, char *out, int inlen);
int base64_decode(const char *in, unsigned char *out, int inlen);
int base64_encoded_len(int len);
int base64_decoded_len(int len);
long uptime(void);
void chld_reap(int sig);
int _eval(char *const argv[], const char *path, int timeout, int *ppid);
int ether_atoe(const char *a, unsigned char *e);
const char *find_word(const char *buffer, const char *word);
unsigned int netdev_calc(char *ifname, char *ifname_desc, unsigned long *rx, unsigned long *tx, char *ifname_desc2, unsigned long *rx2, unsigned long *tx2);
