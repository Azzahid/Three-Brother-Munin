#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/sysinfo.h>


#ifndef HOST_NAME_MAX
  #define HOST_NAME_MAX 256
#endif

static char* host = "";

int function_call () {
	int LINE_MAX=100;
	char line[LINE_MAX];

	printf("# munin node at %s\n", host);
	while (fflush(stdout), fgets(line, LINE_MAX, stdin) != NULL) {
		char * cmd;
		char * arg;
		cmd = strtok(line, " \t\n\r");
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

int main() {
	
	if ('\0' == *host) {
		int idx;

		host = malloc(HOST_NAME_MAX + 1);
		gethostname(host, HOST_NAME_MAX);

		/* going to lowercase */
		for (idx = 0; host[idx] != '\0'; idx++) {
			host[idx] = tolower((int) host[idx]);
		}
	}

	function_call("config memory\n");
	return 0;
}
