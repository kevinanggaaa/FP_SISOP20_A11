#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

int strstr(char *string, char *sub) {
	int res = 0;
	while(string[res]!='\0') {
		if (string[res] == sub[0]) {
			int subIter=0;
			int sameFlag=1;
			while (sub[subIter]!='\0') {
				if (string[res+subIter] != sub[subIter]) {
					sameFlag=0;
					break;
				}

				subIter++;
			}
			if (sameFlag) {
				return res;
			}
		}

		res++;
	}
	return -1;
}

int strrstr(char *string, char *sub) {
	int res=strlen(string)-1;
	int subLen=strlen(sub)-1;

	while(res>=0) {
		if (string[res] == sub[subLen]) {
			int offset=0;
			int sameFlag=1;
			while (subLen-offset>=0) {
				if (string[res-offset]!=sub[subLen-offset]) {
					sameFlag=0;
					break;
				}

				offset++;
			}
			if (sameFlag) {
				return res;
			}
		}

		res--;
	}
	return -1;
}

int strrchr(char *string, char sub) {
	int i = strlen(string)-1;
	while(i>=0) {
		if (string[i]==sub) {
			return i;
		}
		i--;
	}
	return -1;
}

void toLower(char *string) {
	char lowercase[] = "abcdefghijklmnopqrstuvwxyz";
	int len;
	if ((len=strrchr(string, '.')) == -1) {
		len=strlen(string);
	}

	int i;
	for (i=0; i<len; i++) {
		if (string[i]>='A' && string[i]<='Z') {
			string[i] = lowercase[string[i]-'A'];
		}
	}
}

void toUpper(char *string) {
	char uppercase[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int len;
	if ((len=strrchr(string, '.')) == -1) {
		len=strlen(string);
	}

	int i;
	for (i=0; i<len; i++) {
		if (string[i]>='a' && string[i]<='z') {
			string[i] = uppercase[string[i]-'a'];
		}
	}
}

int renameRaw(char *old, char *new) {
	int ret;
	ret = link(old, new); printf(1,"link: %d", ret);
	if (ret==-1) {
		printf(1,"link error: %s to %s",old,new);
		return ret;

	}

	ret = unlink(old); printf(1,"unlink: %d", ret);
	if (ret==-1) {
		printf(1,"unlink error: %s",old);
		return ret;

	}
	return ret;
}

int replaceStr(char *in, char *find, char*replace) {
	char tmp[500];
	strcpy(tmp,in);

	int start=strstr(tmp,find);
	if (start==-1) {
		return -1;
	}
	int end=start+strlen(find)-1;
	tmp[start]='\0';
	tmp[end]='\0';

	strcpy(in,tmp);
	strcpy(in+strlen(in),replace);
	strcpy(in+strlen(in),tmp+end+1);
	return 0;
}

char **splitter(char *in, int *nRes) {
	printf(1,"Splitter ran\n");
	char temp[500];
	strcpy(temp,in);
	char *end = strchr(temp,'}');
	*end='\0';
	end++;
	char *start = strchr(temp,'{')+1;
	char *next = strchr(temp,'/');

	printf(1,"Splitter checking\n");
	if (end==0 || start==0 || next==0) {
		return 0;
	}

	char **resStrs; *nRes=0;
	resStrs=malloc(10*sizeof(char *));
	char *resStr;
	while (1) {
		resStr=malloc(500*sizeof(char));
		if (next==0) {
			strcpy(resStr,start);
			strcpy(resStr+strlen(resStr),end);
			resStrs[(*nRes)++]=resStr;

			break;
		}

		*next='\0';
		next++;
		strcpy(resStr,start);
		strcpy(resStr+strlen(resStr),end);
		resStrs[(*nRes)++]=resStr;

		start=next;
		next=strchr(next,'|');
	}
	return resStrs;
}

void renameDir(char *argv1, char *argv2) {
	printf(1,"Testdir called\n");
  //char buff[1024];
  int fd0;
  struct dirent de;
  struct stat st;

  if ((fd0 = open(".", 0)) < 0) {
    printf(2, "rename: cannot open current directory\n");
    exit();
  }
  if (fstat(fd0, &st) < 0) {
    printf(2, "rename: cannot stat current directory\n");
    exit();
  }
	printf(1,"Starting read\n");
	int type;
	char *find=0, *replace=0, *end=0;

	printf(1,"getting ext filter\n");
	char *renameExt = strchr(argv2,'*')+1;
	printf(1,"ext filter: %s\n",renameExt);

	char tmp[500];
	strcpy(tmp, argv1);
	// Start rename
	if (strcmp(argv1,"y/a-z/A-Z/") == 0) {
		printf(1,"picked type 1\n");
		type=1;
	} else if (strcmp(argv1,"y/A-Z/a-z/") == 0) {
		printf(1,"picked type 2\n");
		type=2;
	} else {
		printf(1,"picked type 3\n");
		find = strchr(tmp,'/') + 1;
		// printf(1,"find\n");
		end=strrchr(find,'/')+find;
		*end='\0';
		replace=strrchr(find,'/')+find;
		// printf(1,"replace\n");
		*replace='\0';
		replace++;
		// printf(1,"replace redirected: %s\n", replace);
		// printf(1,"finding: %s\n",(char *)(tmp));
		printf(1,"end redirected\n");
		// Result from s/test/t/ s\0test\0t\0
		printf(1,"checking replace pattern\n");
		if (strcmp(find,replace)==0) {
		// Replace pattern is the same, there is no need to do replacemenet
			return;
		}
		printf(1,"replace patt: %s |to| %s\n", find, replace);
		type=3;
	}

	while (read(fd0, &de, sizeof(de)) != 0) {
		printf(1,"name: %s\n", de.name);
		// printf(1,"Reading: %s\n",de.name);
		
		fstat(open(de.name, 0), &st);
		// printf(1,"filetype: %d %X %o\n",st.type);
		if (st.type != T_FILE) {
			printf(1, "Not file\n");
			continue;
		}
		// printf(1,"substr: %s\n", strrstr(de.name,renameExt));
		if (strrstr(de.name,renameExt)==-1) {
			continue;
		}

		printf(1,"passed\n");
		// Copy to buffer
		char from[500],to[500];
		strcpy(from, de.name);
		strcpy(to, de.name);

		// Start rename
		if (type==1) {
		// If rename from UPPERCASE to lowercase
			toUpper(to);
		} else if (type==2) {
		// If rename from lowercase to UPPERCASE
			toLower(to);
		} else if (type==3) {
			int nRes;
			char **res=splitter(find, &nRes);
			
			if (nRes == 0) {
				printf(1,"No splitter\n");
				if (replaceStr(to, find, replace) == -1) {
					continue;
				}
			} else {
				int i;
				int replaceRan=0;
				for(i=0; i<nRes; i++) {
					if (replaceStr(to, res[i], replace) == 0) {
						replaceRan=1;
						break;
					}
				}
				if (!replaceRan) {
					continue;
				}
			}
		}

		if (strcmp(from,to)==0){
		// If string is no difference
			continue;
		}
		renameRaw(from, to);
	}
  close(fd0);
}

int main(int argc, char *argv[]) {
	printf(1,"argv1: %s\n",argv[1]);
	if (argc == 2) {
		printf(1,"Need 2 arguments!\n");
		exit();
	}

	renameDir(argv[1],argv[2]);
  exit();
}
