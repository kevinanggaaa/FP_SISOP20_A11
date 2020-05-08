#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

void headbyte(char* path, long long int nBytes) {
	int fd;
	fd = open(path, O_RDWR);

	if(fd < 0) {
		printf(1, "Error: Cannot open fd %s\n", path);
		return;
	}

	char chr[1];
	while(read(fd, chr, sizeof(char))) {
		if (nBytes == 0) {
			break;
		}
		printf(1, "%c", chr[0]);

		nBytes--;
	}
	close(fd);
}

void headline(char* path, long long int nLines) {
	// printf(1, "Headline Called: %s\n", path);
	int fd;
	fd = open(path, O_RDWR);

	// printf(1, "fd: %d\n", fd);
	if(fd < 0) {
		printf(1, "Error: Cannot open fd %s\n", path);
		return;
	}

	char chr[1];
	while(read(fd, chr, sizeof(char))) {
		if (nLines == 0) {
			break;
		}
		printf(1, "%c", chr[0]);

		if (chr[0] == '\n'){
			nLines--;
		}
	}
	close(fd);
}

int main(int argc, char* argv[]) {
	int i;

	long long int inNum;
	if(argc < 2) {
		printf(1, "Usage: head <mode> <filename>");
		exit();
	}

	if(argv[1][0] == '-') {
		if(argv[1][1] == 'n') {
			if(argc < 4) {
				printf(1, "Error: Usage head -n <NUM> <filename>\n");
				exit();
			}

			inNum=atoi(argv[2]);
			for(i=3;i<argc;i++) {
				headline(argv[i], inNum);
			}
		} else if(argv[1][1] == 'c') {
			if(argc < 4) {
				printf(1, "Error: Usage head -c <NUM> <filename>\n");
			}

			inNum=atoi(argv[2]);
			for(i=3;i<argc;i++) {
				headbyte(argv[i], inNum);
			}
		} else if(argv[1][1] == 'q') {
			for(i=2;i<argc;i++) {
				headline(argv[i], 10);
			}
		} else if(argv[1][1] == 'v') {
			for(i=2;i<argc;i++) {
				printf(1, "==> %s <==\n", argv[i]);
				headline(argv[i], 10);
			}
		} else {
			printf(1, "error: mode undefined\n");
		}
	}	else {
		for(i=1; i<argc; i++) {
			headline(argv[i], 10);
		}
	}

	exit();
}
