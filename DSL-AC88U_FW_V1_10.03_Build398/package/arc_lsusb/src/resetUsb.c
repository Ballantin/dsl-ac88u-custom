#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/usbdevice_fs.h>

int main(int argc, char **argv)
{
    const char *filename;
    int fd;
    int rc = 1;

    if (argc != 2) {
        fprintf(stderr, "Usage: usbreset device-filename\n");
        return 1;
    }
    filename = argv[1];

    fd = open(filename, O_WRONLY);
    if (fd > 0) {
		printf("Resetting USB device %s\n", filename);
		rc = ioctl(fd, USBDEVFS_RESET, 0);
		if (rc < 0) {
			perror("Error in ioctl");
		}
		close(fd);
	}
	if(rc>=0)
		printf("Reset successful\n");

    return rc;
}