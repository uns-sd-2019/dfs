const MAXLEN = 1024;
const FILELEN = 1000000000;
typedef string my_file<FILELEN>;

typedef char filename[MAXLEN];

struct file_to_send{
	filename name;
	my_file data;
	int size;
};

program FILE_PROG{
	version FILE_VER{
		int sendFile(file_to_send) = 1;
		file_to_send getFile(string) = 2;
	} = 1;
} = 0x20000011;