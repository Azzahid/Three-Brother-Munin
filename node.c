#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/sysinfo.h>
#include "getip.c"

#define SERVER_PORT 4949

int main()
{
	//address
	struct sockaddr_in sin;
	//socket
	int sock_listen;
    int yes = 1;
    char * ip;
    static char* host = "";
    
    if ('\0' == *host) {
		int idx;

		host = malloc(HOST_NAME_MAX + 1);
		gethostname(host, HOST_NAME_MAX);

		/* going to lowercase */
		for (idx = 0; host[idx] != '\0'; idx++) {
			host[idx] = tolower((int) host[idx]);
		}
	}
    ipv4(&ip);
	//address
	sin.sin_family = AF_INET;
	sin.sin_port = htons(SERVER_PORT);
	sin.sin_addr.s_addr = inet_addr(ip);
	if((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("Error Create Socket\n");
		exit(errno);
	}

    if (setsockopt(sock_listen, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("setsockopt");
        exit(1);
    }
	
    if(bind(sock_listen, (struct sockaddr*) &sin, sizeof(sin))<0){
		printf("error binding\n");
		exit(errno);
	}
	
	listen(sock_listen, 3);
    int client_accept;
    int LINE_MAX=100;
    char buf[LINE_MAX];
    client_accept = accept(sock_listen, NULL, NULL);
    printf("# munin node at %s\n", host);
    while(1){
        recv(client_accept, buf, sizeof(buf),0);
        char * cmd;
        char * arg;
        cmd = strtok(buf, " \t\n\r");
        if(cmd == NULL)
            arg = NULL;
        else
            arg = strtok(NULL, " \t\n\r");

        if (!cmd || strlen(cmd) == 0) {
            printf("# empty cmd\n");
        } else if (strcmp(cmd, "version") == 0) {
            printf("zahid node on %s version: 8.48\n", host);
        } else if (strcmp(cmd, "nodes") == 0) {
            printf("%s\n", host);
            printf(".\n");
        } else if (strcmp(cmd, "quit") == 0) {
            close(sock_listen);
            return(0);
        } else if (strcmp(cmd, "list") == 0) {
            printf("memory\n");
        } else if (
                strcmp(cmd, "config") == 0 ||
                strcmp(cmd, "fetch") == 0
            ) {
                long int total_memory = get_phys_pages()*getpagesize();
                long int free_memory = get_avphys_pages()*getpagesize();
                long int used_memory = total_memory - free_memory;
                
            if (strcmp(cmd,"fetch")==0 && (strcmp(arg,"memory")==0)) {
                printf("used.value %ld\n", used_memory);
                printf("free.value %ld\n",free_memory);
            } else {
                printf("graph_args --base 1024 -1 0 --upper-limit %ld\n",total_memory);
                printf("graph_vlabel Bytes\n"); 
                printf("graph_title Memory usage\n"); 
                printf("graph_category system\n"); 
                printf("graph_info This graph shows this machine memory.\n"); 
                printf("graph_order used free\n"); 
                printf("used.label used\n");
                printf("used.draw STACK\n"); 
                printf("used.info Used memory.\n"); 
                printf("free.label free\n"); 
                printf("free.draw STACK\n"); 
                printf("free.info Free memory.\n"); 
                printf(".\n");
            }
        } else if (strcmp(cmd, "cap") == 0) {
            printf("cap multigraph dirtyconfig\n"); 
        } else {
            printf("# Unknown cmd: %s. Try cap, list, nodes, config, fetch, version or quit\n", cmd);
        }
	}
	return 0;
}

