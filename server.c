#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void ipv4() {
	int fd;
	struct ifreq ifr;

	char iface[] = "wlp2s0";

	fd = socket(AF_INET, SOCK_DGRAM, 0);

	ifr.ifr_addr.sa_family = AF_INET;

	strncpy(ifr.ifr_name, iface, IFNAMSIZ-1);

	ioctl(fd, SIOCGIFADDR, &ifr);

	close(fd);

	char ip = inet_ntoa(( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr);

	printf("IPv4: %s", ip);
}

int main() {
	// int fd;
	// struct ifreq ifr;

	// char iface[] = "wlp2s0";

	// fd = socket(AF_INET, SOCK_DGRAM, 0);

	// ifr.ifr_addr.sa_family = AF_INET;

	// strncpy(ifr.ifr_name, iface, IFNAMSIZ-1);

	// ioctl(fd, SIOCGIFADDR, &ifr);

	// close(fd);

	// printf("IPv4: %s", inet_ntoa(( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr) );
	ipv4();
}