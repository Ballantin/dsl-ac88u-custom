#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

int file_lock(char *tag){
	char fn[64];
    struct flock lock;
    int lockfd = -1;
    pid_t lockpid;

    sprintf(fn, "/var/lock/%s.lock", tag);
    if ((lockfd = open(fn, O_CREAT | O_RDWR, 0666)) < 0)
        goto lock_error;

    pid_t pid = getpid();
    if (read(lockfd, &lockpid, sizeof(pid_t))) {
        // check if we already hold a lock
        if (pid == lockpid) {
            // don't close the file here as that will release all locks
            return -1;
        }
    }

    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    lock.l_pid = pid;

    if (fcntl(lockfd, F_SETLKW, &lock) < 0) {
        close(lockfd);
        goto lock_error;
    }

    lseek(lockfd, 0, SEEK_SET);
    write(lockfd, &pid, sizeof(pid_t));
    return lockfd;
lock_error:
    // No proper error processing
    syslog(LOG_DEBUG, "Error %d locking %s, proceeding anyway", errno, fn);
    return -1;
}

void file_unlock(int lockfd) {
	if (lockfd >= 0) {
		ftruncate(lockfd, 0);
		close(lockfd);
	}
}

void nvram_set(const char *fname, const char *value) 
{
	FILE *fp = NULL;
	char fpath[32];

	snprintf(fpath, 32, "/tmp/%s", fname);
	fprintf(stdout, "%s: path %s\n", __func__, fpath);

	if( (fp = fopen(fpath, "w")) ) {
		fprintf(fp,"%s", value);
		fclose(fp);
	}
}

char *nvram_get(const char *fname) 
{
	static char buf[32];
	char fpath[32];
	FILE *fp = NULL;

	snprintf(fpath,32,"/tmp/%s", fname);
	fprintf(stdout, "%s: path %s\n", __func__, fpath);
	memset(buf, 0, 32);
	if( (fp = fopen(fpath,"r"))) {
		fscanf(fp, "%s", buf);
		fclose(fp);
	}
	fprintf(stdout, "%s: strlen(buf) %d\n", __func__, (int)strlen(buf));
	return buf;
}

int nvram_match(const char *fname, const char *value) 
{
	//char buf[32];
	char *p = nvram_get(fname);

	//memset(buf,0,32);
	if(strlen(p)<=0) return 0;
	
	return !strncmp(p, value, strlen(value));
}

void nvram_invmatch(const char *fname, const char *value) 
{
	return !nvram_match(fname, value);
}

char *getIpAddr(const char *ifname) 
{
	int fd, r;
	struct ifreq ifr;
	char ipstr[16];
	memset(ipstr, 0, 16);

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);
	r = ioctl(fd, SIOCGIFADDR, &ifr);
	close(fd);
	if(r==0) {
		snprintf(ipstr, 16, "%s", inet_ntoa( ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr) );
	}
	fprintf(stdout, "%s: ipstr %s\n", __func__, ipstr);

	return ipstr;
}
