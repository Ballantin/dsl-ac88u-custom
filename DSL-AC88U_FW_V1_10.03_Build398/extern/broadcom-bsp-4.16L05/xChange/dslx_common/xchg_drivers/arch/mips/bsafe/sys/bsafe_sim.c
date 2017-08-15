/*
 * Bsafe uHSM sock simulator wrapper function 
 *
 * $Copyright Open Broadcom Corporation$      
 * $Id: bsafe_sim.c,v 1.1.1.1 2004/07/20 01:07:36 gigis Exp $
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close */

#include <bsafe_core_if.h>

#undef PRINTF
#define PRINTF	/*printf*/
//#define PRINTF	printf

int tcp_client_connect (char *servername) ;
#define tcp_client_close close

int bsafe_sock_initialize(int fd)
{
	bsafe_initialize(&fd);
}
int bsafe_sock_clean()
{
	bsafe_clean();
}

int bsafe_sock_isr_callback(void *param)
{
	bsafe_isr_callback(param);
}

static int mysock_first_time =0;

int bsafe_sock_open(char * devname)
{
	int fd=0;


	fd= tcp_client_connect(devname);	/* here the app provides the devcontext  */

	PRINTF("bsafe_open %d\n",fd);

	/* since this is not a module initialize is called the first time we open a socket */
	/* Anyway it can be done this way ...does not matter */
	{
	if (mysock_first_time ==0)
		bsafe_sock_initialize(fd);
		mysock_first_time++;
	}

	return fd;
}

int bsafe_sock_close(int fd)
{
	PRINTF("bsafe_close %d\n",fd);

#if 0 /* since this is an over kill since we have only one active fd always it cleans each time */
	/* should do it in module clean */
	/* since this is not a module clean is called the last time we close the socket */
	mysock_first_time--;
	if (mysock_first_time == 0)
		bsafe_sock_clean();
#endif

	tcp_client_close(fd);
	return 0;
}

#define LOCK()
#define UNLOCK()

int bsafe_sock_ioctl(int fd, int command, void *cmdio)
{
	int ret=0;

	LOCK();
		/* here fd is the context */
		ret= bsafe_ioctl((void *)&fd, command, cmdio);
	UNLOCK();
	return ret;
}


#define SERVER_PORT 1500
#define MAX_MSG 100

char APP_NAME[]={"CLIENT"};


int tcp_client_connect (char *servername) {

  int sd;
  int  rc, i;
  struct sockaddr_in localAddr, servAddr;
  struct hostent *h;


  h = gethostbyname(servername);
  if(h==NULL) {
    printf("%s: unknown host '%s'\n",APP_NAME,servername);
    exit(1);
  }

  servAddr.sin_family = h->h_addrtype;
  memcpy((char *) &servAddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
  servAddr.sin_port = htons(SERVER_PORT);

  /* create socket */
  sd = socket(AF_INET, SOCK_STREAM, 0);
  if(sd<0) {
    perror("cannot open socket ");
    exit(1);
  }

  /* bind any port number */
  localAddr.sin_family = AF_INET;
  localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  localAddr.sin_port = htons(0);
  
  rc = bind(sd, (struct sockaddr *) &localAddr, sizeof(localAddr));
  if(rc<0) {
    printf("%s: cannot bind port TCP %u\n",APP_NAME,SERVER_PORT);
    perror("error ");
    exit(1);
  }
				
  /* connect to server */
  rc = connect(sd, (struct sockaddr *) &servAddr, sizeof(servAddr));
  if(rc<0) {
    perror("cannot connect ");
    exit(1);
  }

return sd;
  
}

#include <bsafe_regs.h>

int bsafe_write_uint32(void * dev_context, int address, int *data_in)
{
	int n=0;
	int fd= *((int *)dev_context);

	/* simulation currently only handles data no ctrl's */
	if (address != GLB_CMD_DATA_IN)
		return 1;	/* just say we read something */

	n = send(fd, data_in,4,0);
	if (n < 4) {
		printf("%s: write_value failed %d\n",APP_NAME,n);
		return -1;
	}
		
    	PRINTF("%s: write_value %08x: %08x [%d]\n",APP_NAME, address, *data_in ,n);

	return n;
}
int bsafe_read_uint32(void * dev_context, int address, int *data_out)
{
	int n=0;
	int fd= *((int *)dev_context);

	/* simulation currently only handles data no ctrl's */
	if (address != GLB_CMD_DATA_OUT)
		return 1;	/* just say we read something */

	n = recv(fd,data_out,4,0);
	if (n < 4) {
		printf("%s: read_value(read) failed %d\n",APP_NAME,n);
		return -1;
	}
		
    	PRINTF("%s: read_value %08x: %08x [%d]\n",APP_NAME, address, *data_out ,n);

	return n;
}
