#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <string.h>

void ipv4(char **ip) {
	int fd;
	struct ifreq ifr;

	char iface[] = "wlp3s0";

	fd = socket(AF_INET, SOCK_DGRAM, 0);

	ifr.ifr_addr.sa_family = AF_INET;

	strncpy(ifr.ifr_name, iface, IFNAMSIZ-1);

	ioctl(fd, SIOCGIFADDR, &ifr);

	close(fd);

	*ip = inet_ntoa(( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr);

	//printf("IPv4: %s", ip);
}
