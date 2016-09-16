/* xsh_hello.c - xsh_hello */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_hello - say hello to the user whose name is passed
 *------------------------------------------------------------------------
 */
shellcmd xsh_hello(int nargs, char *args[]) {

	/* Check argument count */
	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {

		printf("Use:\n");
		printf("\t%s [command]\n", args[0]);
		printf("Description:\n");
		printf("\tProvides a list of shell commands or\n");
		printf("\thelp information for a specific command\n");
		printf("Options:\n");
		printf("\tcommand\tspecific command for which to\n");
		printf("\t\tdisplay help information\n");
		printf("\t--help\tdisplay this help and exit\n");
		return 0;
	}

	if (nargs > 2) {
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		return 1;
	}
	
	if (nargs < 2) {
		printf("Welcome\n");
		printf("Consider adding one more argument to get a customised message\n");
	}

	if (nargs == 2) {
		printf("Hello %s, Welcome to the world of Xinu!!\n", args[1]);
	}
	
	return 0;
}
