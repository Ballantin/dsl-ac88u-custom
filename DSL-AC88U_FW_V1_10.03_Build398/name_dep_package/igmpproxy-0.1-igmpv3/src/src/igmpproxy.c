/*
**  igmpproxy - IGMP proxy based multicast router
**  Copyright (C) 2005 Johnny Egeland <johnny@rlo.org>
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**
**----------------------------------------------------------------------------
**
**  This software is derived work from the following software. The original
**  source code has been modified from it's original state by the author
**  of igmpproxy.
**
**  smcroute 0.92 - Copyright (C) 2001 Carsten Schill <carsten@cschill.de>
**  - Licensed under the GNU General Public License, version 2
**
**  mrouted 3.9-beta3 - COPYRIGHT 1989 by The Board of Trustees of
**  Leland Stanford Junior University.
**  - Original license can be found in the Stanford.txt file.
**
*/
/**
*   igmpproxy.c - The main file for the IGMP proxy application.
*
*   February 2005 - Johnny Egeland
*/

#include "igmpproxy.h"

static const char Usage[] =
"Usage: igmpproxy [-h] [-d] [-v [-v]] <configfile>\n"
"\n"
"   -h   Display this help screen\n"
"   -d   Run in debug mode. Output all messages on stderr\n"
"   -v   Be verbose. Give twice to see even debug messages.\n"
"   -c   print debug message to console.\n"
"\n"
PACKAGE_STRING "\n"
;

// Local function Prototypes
static void signalHandler(int);
int     igmpProxyInit();
void    igmpProxyCleanUp();
void    igmpProxyRun();

// Global vars...
static int sighandled = 0;
#define	GOT_SIGINT	0x01
#define	GOT_SIGHUP	0x02
#define	GOT_SIGUSR1	0x04
#define	GOT_SIGUSR2	0x08

// The upstream VIF index
int         upStreamVif;


/*Fix by Shawn, 2012-10-29
* Using gettimeofday will leading to crash because of NTP will give a large time leap.
* We can only use sysinfo.uptime, but it's in second unit. To avoid many  changes in 
* original code, we just construct a timeval with usec always be 0.
* This change will lead to the resolution degrade to 1s but it's 
* impact is minor.
*/
void getUpTime(struct timeval * time)
{
	struct sysinfo info;
	sysinfo(&info);
	time->tv_sec=info.uptime;
	time->tv_usec=0;
}


void longToByteArray(unsigned char *byte_value, unsigned long long_value)
{
	register unsigned char *l = (unsigned char *)&long_value;

	byte_value[0]=l[0];
	byte_value[1]=l[1];
	byte_value[2]=l[2];
	byte_value[3]=l[3];
}

/**
*   Program main method. Is invoked when the program is started
*   on commandline. The number of commandline arguments, and a
*   pointer to the arguments are recieved on the line...
*/
int main( int ArgCn, char *ArgVc[] ) {

    // Parse the commandline options and setup basic settings..
    for (int c; (c = getopt(ArgCn, ArgVc, "vdh")) != -1;) {
        switch (c) {
        case 'd':
            Log2Stderr = true;
            LogLevel = LOG_DEBUG; // bitonic test
            break;
        case 'v':
            if (LogLevel == LOG_INFO)
                LogLevel = LOG_DEBUG;
            else
                LogLevel = LOG_INFO;
            break;
        case 'h':
            fputs(Usage, stderr);
            exit(0);
            break;
        default:
            exit(1);
            break;
        }
    }

    if (optind != ArgCn - 1) {
		fputs("[igmpproxy main] You must specify the configuration file.\n", stderr);
		exit(1);
    }
    char *configFilePath = ArgVc[optind];

    // Check that we are root
    if (geteuid() != 0) {
       fprintf(stderr, "[igmpproxy main] igmpproxy: must be root\n");
       exit(1);
    }

    openlog("[igmpproxy main] igmpproxy", LOG_PID, LOG_USER);

    // Write debug notice with file path...
    my_log(LOG_DEBUG, 0, "[igmpproxy main] Searching for config file at '%s'" , configFilePath);

    do {

        // Loads the config file...
        if( ! loadConfig( configFilePath ) ) {
            my_log(LOG_ERR, 0, "[igmpproxy main] Unable to load config file...");
            break;
        }

////		loadConfigInterfaceSection();

        // Initializes the deamon.
        if ( !igmpProxyInit() ) {
            my_log(LOG_ERR, 0, "[igmpproxy main] Unable to initialize IGMPproxy.");
            break;
        }

        // Go to the main loop.
        igmpProxyRun();

        // Clean up
        igmpProxyCleanUp();

    } while ( false );

    // Inform that we are exiting.
    my_log(LOG_INFO, 0, "[igmpproxy main] Shutdown complete....");

    exit(0);
}



/**
*   Handles the initial startup of the daemon.
*/
int igmpProxyInit() {
    struct sigaction sa;
    int Err;


    sa.sa_handler = signalHandler;
    sa.sa_flags = 0;    /* Interrupt system calls */
    sigemptyset(&sa.sa_mask);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);

    // Loads configuration for Physical interfaces...
    buildIfVc();

    // Configures IF states and settings
    configureVifs();

    switch ( Err = enableMRouter() ) {
    case 0: break;
    case EADDRINUSE: my_log( LOG_ERR, EADDRINUSE, "MC-Router API already in use" ); break;
    default: my_log( LOG_ERR, Err, "MRT_INIT failed" );
    }

    /* create VIFs for all IP, non-loop interfaces
     */
    {
        unsigned Ix;
        struct IfDesc *Dp;
        int     vifcount = 0;
        upStreamVif = -1;

        for ( Ix = 0; (Dp = getIfByIx(Ix)); Ix++ ) {

            if ( Dp->InAdr.s_addr && ! (Dp->Flags & IFF_LOOPBACK) ) {
                if(Dp->state == IF_STATE_UPSTREAM) {
                    if(upStreamVif == -1) {
                        upStreamVif = Ix;
                    } else {
                        my_log(LOG_ERR, 0, "Vif #%d was already upstream. Cannot set VIF #%d as upstream as well.",
                            upStreamVif, Ix);
                    }
                }

                addVIF( Dp );
                vifcount++;
            }
        }

        // If there is only one VIF, or no defined upstream VIF, we send an error.
        if(vifcount < 2 || upStreamVif < 0) {
            my_log(LOG_ERR, 0, "There must be at least 2 Vif's where one is upstream.");
        }
    }

    // Initialize IGMP
    initIgmp();
    // Initialize Routing table
    initRouteTable();
    // Initialize timer
    callout_init();


    return 1;
}

/**
*   Clean up all on exit...
*/
void igmpProxyCleanUp() {

    my_log( LOG_DEBUG, 0, "clean handler called" );

    free_all_callouts();    // No more timeouts.
    clearAllRoutes();       // Remove all routes.
    disableMRouter();       // Disable the multirout API

}

/**
*   Main daemon loop.
*/
void igmpProxyRun() {
    // Get the config.
    //struct Config *config = getCommonConfig();
    // Set some needed values.
    register int recvlen;
    int     MaxFD, Rt, secs;
    fd_set  ReadFDS;
    socklen_t dummy = 0;
    struct  timeval  curtime, lasttime, difftime, tv;
    struct timeval curtime_sysup,lasttime_sysup,difftime_sysup;  //used as backup when NTP switchs
    int switch_times=0;  //a debug info for how many times we use sysup time
    // The timeout is a pointer in order to set it to NULL if nessecary.
    struct  timeval  *timeout = &tv;

    // Initialize timer vars
    difftime.tv_usec = 0;
    gettimeofday(&curtime, NULL);
    getUpTime(&curtime_sysup);
    lasttime = curtime;
    lasttime_sysup=curtime_sysup;

    // First thing we send a membership query in downstream VIF's...
    sendGeneralMembershipQuery();

    // Loop until the end...
    for (;;) {

        // Process signaling...
        if (sighandled) {
            if (sighandled & GOT_SIGINT) {
                sighandled &= ~GOT_SIGINT;
                my_log(LOG_NOTICE, 0, "[igmpProxyRun] Got a interupt signal. Exiting.");
                break;
            }
        }

        // Prepare timeout...
        secs = timer_nextTimer();
        if(secs == -1) {
            timeout = NULL;
        } else {
            timeout->tv_usec = 0;
            timeout->tv_sec = secs;
        }

        // Prepare for select.
        MaxFD = MRouterFD;

        FD_ZERO( &ReadFDS );
        FD_SET( MRouterFD, &ReadFDS );

        // wait for input
        Rt = select( MaxFD +1, &ReadFDS, NULL, NULL, timeout );

        // log and ignore failures
        if( Rt < 0 ) {
            my_log( LOG_WARNING, errno, "[igmpProxyRun] select() failure" );
            continue;
        }
        else if( Rt > 0 ) {

            // Read IGMP request, and handle it...
            if( FD_ISSET( MRouterFD, &ReadFDS ) ) {

                recvlen = recvfrom(MRouterFD, recv_buf, RECV_BUF_SIZE,
                                   0, NULL, &dummy);
                if (recvlen < 0) {
                    if (errno != EINTR) my_log(LOG_ERR, errno, "[igmpProxyRun] recvfrom");
                    continue;
                }


                acceptIgmp(recvlen);
            }
        }

        // At this point, we can handle timeouts...
        do {
            /*
             * If the select timed out, then there's no other
             * activity to account for and we don't need to
             * call gettimeofday.
             */
            if (Rt == 0) {
                curtime.tv_sec = lasttime.tv_sec + secs;
                curtime.tv_usec = lasttime.tv_usec;
                curtime_sysup.tv_sec = lasttime_sysup.tv_sec + secs;
                curtime_sysup.tv_usec = lasttime_sysup.tv_usec;
                Rt = -1; /* don't do this next time through the loop */
            } else {
                gettimeofday(&curtime, NULL);
                getUpTime(&curtime_sysup);
            }
            difftime.tv_sec = curtime.tv_sec - lasttime.tv_sec;
            difftime.tv_usec += curtime.tv_usec - lasttime.tv_usec;
            while (difftime.tv_usec > 1000000) {
                difftime.tv_sec++;
                difftime.tv_usec -= 1000000;
            }
            if (difftime.tv_usec < 0) {
                difftime.tv_sec--;
                difftime.tv_usec += 1000000;
            }

            //judge if difftime make sense. Otherwise use sysup time to get difftime
            //this supposed to only happen once when ntp set the time of router.
            difftime_sysup.tv_sec=curtime_sysup.tv_sec - lasttime_sysup.tv_sec;
            if (labs(difftime.tv_sec-difftime_sysup.tv_sec)>2)  //should be +-1
            {
                switch_times++;
                my_log( LOG_WARNING, 0, "[igmpProxyRun]Switched to sysup time [total %d],difftime.tv_sec=%d, difftime_sysup.tv_sec=%d\n", switch_times,difftime.tv_sec,  difftime_sysup.tv_sec);
                difftime.tv_sec=difftime_sysup.tv_sec;
                difftime.tv_usec=0;
            }

			
            lasttime = curtime;
            lasttime_sysup=curtime_sysup;
            if (secs == 0 || difftime.tv_sec > 0)
                age_callout_queue(difftime.tv_sec);
            secs = -1;
        } while (difftime.tv_sec > 0);
    }
}

/*
 * Signal handler.  Take note of the fact that the signal arrived
 * so that the main loop can take care of it.
 */
static void signalHandler(int sig) {
    switch (sig) {
    case SIGINT:
    case SIGTERM:
        sighandled |= GOT_SIGINT;
        break;
        /* XXX: Not in use.
        case SIGHUP:
            sighandled |= GOT_SIGHUP;
            break;

        case SIGUSR1:
            sighandled |= GOT_SIGUSR1;
            break;

        case SIGUSR2:
            sighandled |= GOT_SIGUSR2;
            break;
        */
    }
}








