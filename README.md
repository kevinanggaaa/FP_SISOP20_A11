# FP_SISOP20_A11

## Kevin Angga Wijaya - 05111840000024
## Julius - 05111840000082
![image](https://user-images.githubusercontent.com/60419316/81245500-57499180-903f-11ea-86d1-7a9597fafeb0.png)
# Daftar Isi
1. [ Soal ](#soal)
2. [ Hasil ](#hasil)
3. [ Pembuktian ](#bukti)
<a name="soal"></a>
## Soal
Praktikan memilih 5 command yang terdiri dari 1 command tabel 1, 2 command tabel 2 dan 2 command tabel 3.
Pada Final Project ini, kami memilih command sebagai berikut :
1. [ Clear ](#clear)
2. [ Rename ](#rename)
3. [ Head ](#head)
4. [ Base64 ](#base64)
5. [ Tail ](#tail)  

Untuk membuat command di atas, dibutuhkan library baru yaitu [ fcntl.h ](#fcntl.h)
<a name="hasil"></a>
## Hasil
Sebelum mengerjakan command berikut dibuat dahulu satu library baru, yaitu fcntl.h :
<a name="fcntl.h"></a>
- fcntl.h
```
#define O_RDONLY  0x000
#define O_WRONLY  0x001
#define O_RDWR    0x002
#define O_CREATE  0x200
#define O_CREAT   0x040
#define O_TRUNC   0x200
```
<a name="clear"></a>
1. Clear
```
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int
main(int argc, char *argv[])
{
  printf(1, "\033[2J\033[1;1H\n");
  exit();
}
```
<a name="rename"></a>
2. Rename
```
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
```
<a name="head"></a>
3. Head
```
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
```
<a name="base64"></a>
4. Base64
```
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

/* 
echo "The quick brown fox Lorem ipsum" > test.txt
base64 test.txt
 */
char * base64_encode(const char *data, int input_length, int *output_length) {
	char encoding_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int mod_table[] = {0, 2, 1};
	*output_length = 4 * ((input_length + 2) / 3);

	char *encoded_data = malloc(*output_length);
	if (encoded_data == 0) {
		return 0;
	}

	for (int i = 0, j = 0; i < input_length;) {
		int octet_a = i < input_length ? (char)data[i++] : 0;
		int octet_b = i < input_length ? (char)data[i++] : 0;
		int octet_c = i < input_length ? (char)data[i++] : 0;

		int triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

		encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
		encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
		encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
		encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
	}

	for (int i = 0; i < mod_table[input_length % 3]; i++){
		encoded_data[*output_length - 1 - i] = '=';
	}

	return encoded_data;
}

int main(int argc, char* argv[]) {
	if(argc!=2) {
		printf(1, "Usage: base64 <filename>");
		exit();
	}

	int fd = open(argv[1], 0);
	char *buf=malloc(16384 * sizeof(char));
	int nBuff=0;
	while(read(fd, &buf[nBuff++], sizeof(char)) > 0) {
	}
	int nOut; char *outBuf;
	outBuf = base64_encode(buf, strlen(buf), &nOut);
	printf(1, "%s", outBuf);
	close(fd);
	exit();
}
```
<a name="tail"></a>
5. Tail
```
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
```
<a name="bukti"></a>
## Bukti
1. [ text.txt dan text2.txt ](#text)
2. [ Head ](#bkhead)
3. [ Tail ](#bktail)
4. [ Base64 ](#bkbase64)
5. [ Rename ](#bkrename)
6. [ Clear ](#bkclear)
<a name="text"></a>
### Diberikan sebuah text.txt dan text2.txt seperti berikut.
![image](https://user-images.githubusercontent.com/60419316/81377565-fa79d400-912f-11ea-929a-29ce5d83181c.png)
<a name="bkhead"></a>
### Head
![image](https://user-images.githubusercontent.com/60419316/81377585-09f91d00-9130-11ea-8aed-0efddcdcad31.png)
</br>![image](https://user-images.githubusercontent.com/60419316/81377597-13828500-9130-11ea-94f3-ea4e0f94bc5e.png)
</br>![image](https://user-images.githubusercontent.com/60419316/81377618-1c735680-9130-11ea-9110-cc92bcf6f373.png)
</br>![image](https://user-images.githubusercontent.com/60419316/81377646-27c68200-9130-11ea-8136-8d010c36784d.png)
</br>![image](https://user-images.githubusercontent.com/60419316/81377671-33b24400-9130-11ea-92eb-c1559d951e17.png)
<a name="bktail"></a>
### Tail
![image](https://user-images.githubusercontent.com/60419316/81377708-40cf3300-9130-11ea-9620-93cb11167aa6.png)
</br>![image](https://user-images.githubusercontent.com/60419316/81377729-49276e00-9130-11ea-9abd-a0dd33b2c3ce.png)
</br>![image](https://user-images.githubusercontent.com/60419316/81377759-55abc680-9130-11ea-93ff-052a20ce01c9.png)
</br>![image](https://user-images.githubusercontent.com/60419316/81377778-5f352e80-9130-11ea-973b-761ec2ab7965.png)
</br>![image](https://user-images.githubusercontent.com/60419316/81377933-a02d4300-9130-11ea-95cb-f6eba2e247bf.png)
</br>![image](https://user-images.githubusercontent.com/60419316/81377957-ac190500-9130-11ea-9025-22e2a0732597.png)
<a name="bkbase64"></a>
### Base64
![image](https://user-images.githubusercontent.com/60419316/81378019-bfc46b80-9130-11ea-803e-518cde685b8c.png)
</br>![image](https://user-images.githubusercontent.com/60419316/81378044-ca7f0080-9130-11ea-8de0-87bb9ea368d6.png)
</br>![image](https://user-images.githubusercontent.com/60419316/81378069-d7035900-9130-11ea-994e-dbc72811f434.png)
<a name="bkrename"></a>
### Rename
![image](https://user-images.githubusercontent.com/60419316/81378378-64df4400-9131-11ea-802e-efdc11e91f5b.png)
</br>![image](https://user-images.githubusercontent.com/60419316/81378411-71639c80-9131-11ea-905d-3679e04e6020.png)
</br>![image](https://user-images.githubusercontent.com/60419316/81378431-7cb6c800-9131-11ea-9dfa-d54fe265d959.png)
</br>![image](https://user-images.githubusercontent.com/60419316/81378450-86d8c680-9131-11ea-9840-938090af3ff9.png)
</br>![image](https://user-images.githubusercontent.com/60419316/81378480-92c48880-9131-11ea-83cb-4cddc519f22e.png)
</br>![image](https://user-images.githubusercontent.com/60419316/81378517-a112a480-9131-11ea-8af2-d51d82a0115d.png)
</br> uppercase lowercase
</br>![image](https://user-images.githubusercontent.com/60419316/81378605-c2739080-9131-11ea-9b64-fdee38f9d73d.png)
</br>![image](https://user-images.githubusercontent.com/60419316/81378635-cd2e2580-9131-11ea-81f5-13469bac968a.png)
</br>![image](https://user-images.githubusercontent.com/60419316/81378650-d4edca00-9131-11ea-8bd5-6cf599e9b66c.png)
</br>![image](https://user-images.githubusercontent.com/60419316/81378673-dfa85f00-9131-11ea-8c5f-ccbcb432088b.png)
<a name="bkclear"></a>
### Clear
![image](https://user-images.githubusercontent.com/60419316/81378712-f2bb2f00-9131-11ea-8652-aea4d4108119.png)
</br>![image](https://user-images.githubusercontent.com/60419316/81378733-0070b480-9132-11ea-8bcf-9db8cbdda55b.png)
