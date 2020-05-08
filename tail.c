#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

void tailbyte(char* path, int nBytes, int isOffset) {
	// printf(1,"Tailbyte called\n");
	int fd;
	fd = open(path, O_RDWR);
	if(fd < 0) {
		printf(1, "Error: Cannot open fd %s\n", path);
		return;
	}

	int totBytes=0;
	char chr[1];
	while(read(fd, chr, sizeof(char))) {
		totBytes++;
	}
	close(fd);

	// printf(1, "isOffset: %d\n", isOffset);
	int skipBytes;
	if (isOffset) {
		skipBytes=nBytes;
		if(skipBytes>=totBytes) {
		// If skip all
			return;
		}
	} else {
		skipBytes=totBytes-nBytes;
		if (skipBytes<0) {
			skipBytes=0;
		}
	}

	// printf(1, "Skipbytes: %d\n", skipBytes);
	fd = open(path, O_RDWR);
	while(read(fd, chr, sizeof(char))) {
		if (skipBytes == 0) {
			break;
		}

		skipBytes--;
	}

	printf(1, "%c", chr[0]);
	while(read(fd, chr, sizeof(char))) {
		printf(1, "%c", chr[0]);
	}
	close(fd);
}

void tailline(char* path, int nLines, int isOffset) {
	// printf(1,"Tailline called\n");
	int fd;
	fd = open(path, O_RDWR);
	if(fd < 0) {
		printf(1, "Error: Cannot open fd %s\n", path);
		return;
	}

	int totLines=0;
	char chr[1];
	while(read(fd, chr, sizeof(char))) {
		if (chr[0]=='\n') totLines++;
	}
	close(fd);

	// printf(1, "isOffset: %d\n", isOffset);
	int skipLines;
	if (isOffset) {
		skipLines=nLines;
		if(skipLines>=totLines) {
		// If skip all
			return;
		}
	} else {
		skipLines=totLines-nLines;
		if (skipLines<0) {
			skipLines=0;
		}
	}

	// printf(1, "Skiplines: %d\n", skipLines);
	fd = open(path, O_RDWR);
	while(read(fd, chr, sizeof(char))) {
		if (skipLines == 0) {
			break;
		}

		if (chr[0]=='\n') skipLines--;
	}

	printf(1, "%c", chr[0]);
	while(read(fd, chr, sizeof(char))) {
		printf(1, "%c", chr[0]);
	}
	close(fd);

}

int main(int argc, char* argv[]) {
	int i;

	int inNum;
	if(argc < 2) {
		printf(1, "Usage: tail <mode> <filename>");
		exit();
	}

	if(argv[1][0] == '-') {
		int isOfsset=0;
		if(argv[1][1] == 'n') {
			if(argc < 4) {
				printf(1, "Error: Usage tail -n (+)<NUM> <filename>\n");
				exit();
			}

			if (argv[2][0] == '+') {
				isOfsset=1;
				inNum=atoi(&argv[2][1]);
			} else {
				isOfsset=0;
				inNum=atoi(argv[2]);
			}

			for(i=3;i<argc;i++) {
				// printf(1, "inNum: %d\n",inNum);
				// printf(1, "isOfsset: %d\n",isOfsset);
				tailline(argv[i], inNum, isOfsset);
			}
		} else if(argv[1][1] == 'c') {
			if(argc < 4) {
				printf(1, "Error: Usage tail -c (+)<NUM> <filename>\n");
			}

			inNum=atoi(argv[2]);
			if (argv[2][0] == '+') {
				isOfsset=1;
				inNum=atoi(&argv[2][1]);
			} else {
				isOfsset=0;
				inNum=atoi(argv[2]);
			}

			for(i=3;i<argc;i++) {
				tailbyte(argv[i], inNum, isOfsset);
			}
		} else if(argv[1][1] == 'q') {
			for(i=2;i<argc;i++) {
				tailline(argv[i], 10, 0);
			}
		} else if(argv[1][1] == 'v') {
			for(i=2;i<argc;i++) {
				printf(1, "==> %s <==\n", argv[i]);
				tailline(argv[i], 10, 0);
			}
		} else {
			printf(1, "error: mode undefined\n");
		}
	}	else {
		for(i=1; i<argc; i++) {
			tailline(argv[i], 10, 0);
		}
	}

	exit();
}
