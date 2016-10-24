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
    char * sendx = NULL;
    /*strcopy(sendx,"#munin node at ");
    strcon()*/

    send(client_accept,"# munin node at ", sizeof("# munin node at "), 0);
    send(client_accept,host, sizeof(host), 0);
    send(client_accept,"\n", sizeof("\n"), 0);
    while(recv(client_accept, buf, sizeof(buf),0)){
        char * cmd;
        char * arg;
        cmd = strtok(buf, " \t\n\r");
        if(cmd == NULL)
            arg = NULL;
        else
            arg = strtok(NULL, " \t\n\r");

        if (!cmd || strlen(cmd) == 0) {
            send(client_accept,"# empty cmd\n", sizeof("# empty cmd\n"), 0);
        } else if (strcmp(cmd, "version") == 0) {
            printf("%s version: 8.48\n", host);
            send(client_accept,"TBM node on ", sizeof("TBM node on "), 0);
            send(client_accept, host, sizeof(host), 0);
            send(client_accept, "version: 8.48\n", sizeof("version: 8.48\n"), 0);
        } else if (strcmp(cmd, "nodes") == 0) {
            send(client_accept, strcat(strcat(host,"\n"),".\n"), sizeof(strcat(strcat(host,"\n"),".\n")), 0);
        } else if (strcmp(cmd, "quit") == 0) {
            close(sock_listen);
            return(0);
        } else if (strcmp(cmd, "list") == 0) {
            send(client_accept, "memory\n", sizeof("memory\n"), 0);
        } else if (strcmp(cmd, "config") == 0 || strcmp(cmd, "fetch") == 0) {
                long int total_memory = get_phys_pages()*getpagesize();
                long int free_memory = get_avphys_pages()*getpagesize();
                long int used_memory = total_memory - free_memory;
                char  use_memory[100];
                sprintf(use_memory, "%ld",used_memory);
                char  fre_memory[100];
                sprintf(fre_memory, "%ld\n",free_memory);
                char  tot_memory[150];
                sprintf(tot_memory, "%ld\n",total_memory);
            if (strcmp(cmd,"fetch")==0) {
                if(arg != NULL){
                    if(strcmp(arg, "memory") == 0){
                        send(client_accept, "used.value ", sizeof("used.value "), 0);
                        send(client_accept, use_memory, strlen(use_memory),0);
                        send(client_accept, "\nfree.value ", sizeof("\nfree.value "), 0);
                        send(client_accept, fre_memory, strlen(fre_memory),0);
                    }else{
                        send(client_accept, "#Unknown argument for fetch. Try memory\n", sizeof("#Unknown argument for fetch. Try memory\n"), 0);
                    }
                }else{
                    send(client_accept, "#no argument\n", sizeof("#no argument\n"), 0);
                }
            } else {
               printf("used.value %ld\n", used_memory);
                send(client_accept, "used.value ", sizeof("used.value"), 0 );
                send(client_accept, use_memory, strlen(use_memory), 0 );
                printf("free.value %ld\n",free_memory);
                send(client_accept, "\nfree.value ", sizeof("\nfree.value"), 0 );
                send(client_accept, fre_memory, strlen(fre_memory), 0 );
                printf("graph_args --base 1024 -1 0 --upper-limit %ld\n",total_memory);
                send(client_accept, "\ngraph_args --base 1024 -1 0 --upper-limit ", sizeof("\ngraph_args --base 1024 -1 0 --upper-limit "), 0 );
                send(client_accept, tot_memory, strlen(tot_memory), 0 );
                printf("graph_vlabel Bytes\n");
                send(client_accept, "graph_vlabel Bytes\n", sizeof("graph_vlabel Bytes\n"), 0 );
                printf("graph_title Memory usage\n");
                send(client_accept, "graph_title Memory usage\n", sizeof("graph_title Memory usage\n"), 0 );
                printf("graph_category system\n");
                send(client_accept, "graph_category system\n", sizeof("graph_category system\n"), 0 );
                printf("graph_info This graph shows this machine memory.\n");
                send(client_accept, "graph_info This graph shows this machine memory.\n", sizeof("graph_info This graph shows this machine memory.\n"), 0 );
                printf("graph_order used free\n");
                send(client_accept, "graph_order used free\n", sizeof("graph_order used free\n"), 0 );
                printf("used.label used\n");
                send(client_accept, "used.label used\n", sizeof("used.label used\n"), 0 );
                printf("used.draw STACK\n"); 
                send(client_accept, "used.draw STACK\n", sizeof("used.draw STACK\n"), 0 );
                printf("used.info Used memory.\n"); 
                send(client_accept, "graph_order used free\n", sizeof("graph_order used free\n"), 0 );
                printf("free.label free\n"); 
                send(client_accept, "free.label free\n", sizeof("free.label free\n"), 0 );
                printf("free.draw STACK\n"); 
                send(client_accept, "free.draw STACK\n", sizeof("free.draw STACK\n"), 0 );
                printf("free.info Free memory.\n");
                send(client_accept, "free.info Free memory.\n.\n", sizeof("free.info Free memory.\n.\n"), 0 ); 
                printf(".\n");
            }
        } else if (strcmp(cmd, "cap") == 0) {
            send(client_accept, "cap multigraph dirtyconfig\n", sizeof("cap multigraph dirtyconfig\n"), 0); 
        } else {
            send(client_accept, "# Unknown command. Try cap, list, nodes, config, fetch, version or quit\n", sizeof("# Unknown command. Try cap, list, nodes, config, fetch, version or quit\n"), 0);
        }
	}
	return 0;
}

