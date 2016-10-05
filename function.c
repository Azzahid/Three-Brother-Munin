#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>


#ifndef HOST_NAME_MAX
  #define HOST_NAME_MAX 256
#endif

static char* host = "";

int function_call (char * line) {

		char* cmd;
		char* arg;

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
			/*
			  DIR* dirp = opendir(plugin_dir);
			if (dirp == NULL) {
				printf("# Cannot open plugin dir\n");
				return(0);
			}
			{
			struct dirent* dp;
			while ((dp = readdir(dirp)) != NULL) {
				char* plugin_filename = dp->d_name;;
*/
				//if (plugin_filename[0] == '.') {
					/* No dotted plugin */
				//	continue;
			//	}
/*
				snprintf(cmdline, LINE_MAX, "%s/%s", plugin_dir, plugin_filename);
				if (access(cmdline, X_OK) == 0) {
					if(extension_stripping) { */
						/* Strip after the last . */
				/*		char *last_dot_idx = strrchr(plugin_filename, '.');
						if (last_dot_idx != NULL) {
							*last_dot_idx = '\0';
						}
					}
					printf("%s ", plugin_filename);
				}
			}
			closedir(dirp);
			}
			putchar('\n');
		} else if (
				strcmp(cmd, "config") == 0 ||
				strcmp(cmd, "fetch") == 0
			) {
			char cmdline[LINE_MAX];
			char *argv[3] = { 0, };
			pid_t pid;
			if(arg == NULL) {
				printf("# no plugin given\n");
				continue;
			}
			if(arg[0] == '.' || strchr(arg, '/') != NULL) {
				printf("# invalid plugin character\n");
				continue;
			} */
		//	if (! extension_stripping || find_plugin_with_basename(cmdline, plugin_dir, arg) == 0) {
				/* extension_stripping failed, using the plain method */
			//	snprintf(cmdline, LINE_MAX, "%s/%s", plugin_dir, arg);
		//	}
	/*		if (access(cmdline, X_OK) == -1) {
				printf("# unknown plugin: %s\n", arg);
				continue;
			}
*/
			/* Now is the time to set environnement */
	/*		setenvvars_conf(arg);
			argv[0] = arg;
			argv[1] = cmd;
*/
			/* Using posix_spawnp() here instead of fork() since we will
			 * do a little more than a mere exec --> setenvvars_conf() */
	//		if (0 == posix_spawn(&pid, cmdline,
		//			NULL, /* const posix_spawn_file_actions_t *file_actions, */
					//NULL, /* const posix_spawnattr_t *restrict attrp, */
			//		argv, environ)) {

				/* Wait for completion */
				//waitpid(pid, NULL, 0);
			//} else {
			//	printf("# fork failed\n");
		//		continue;
	//		}
	//		printf(".\n");
		} else if (strcmp(cmd, "cap") == 0) {
			printf("cap multigraph dirtyconfig\n");
		} else if (strcmp(cmd, "spoolfetch") == 0) {
			printf("# not implem yet cmd: %s\n", cmd);
		} else {
			printf("# Unknown cmd: %s. Try cap, list, nodes, config, fetch, version or quit\n", cmd);
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

	function_call("config memory");
	return 0;
}
