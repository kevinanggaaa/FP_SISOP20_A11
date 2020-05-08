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
