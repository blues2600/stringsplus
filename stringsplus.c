
/**************************************************************************************
					
								** 简述 **
								* 这是一个清洗字符串的工具

***************************************************************************************/

#include		<stdio.h>
#include		<string.h>
#include		<ctype.h>
#include		<stdlib.h>

#define			MIN_LEN				5						//小于MIN_LEN的字符串会被抛弃
#define			LINE_MAX_SIZE		1024					//待清洗文件中一行的最大长度(从字符串开始到第一个换行符)


int	isNoNumNoWord(const char* strings, size_t len);
int	allRepeat(const char* strings, size_t len);
int isURL(const char* strings, size_t len);
int isDirectory(const char* strings, size_t len);
int	isIP(const char* strings, size_t len);
int	isExe(const char* strings, size_t len);

int main(int argc, char* argv[])
{
	char		fileName[120] = { '0' };
	char		stringFile[] = "c:\\re\\string.txt";
	char		URLFile[] = "c:\\re\\url.txt";
	char		dirFile[] = "c:\\re\\dir.txt";
	char		ipFile[] = "c:\\re\\ip.txt";
	char		exeFile[] = "c:\\re\\exe.txt";
	FILE		*pfile = NULL;
	FILE		*pstringFile = NULL;
	FILE		*pURLFile = NULL;
	FILE		*pDirFile = NULL;
	FILE		*pIPFile = NULL;
	FILE		*pExeFile = NULL;
	char		Line[LINE_MAX_SIZE] = { '0' };				//存放一行数据
	size_t		str_len;


	//			//
	/* 命令行处理 */
	//			//

	if (argc != 2) {
		printf("\n不正确的命令格式。\n");
		printf("\n命令格式参考:\n");
		printf("\tstringsplus  FileName\n");
		return 0;
	}
	else {
		if (strcpy_s(fileName, 119, argv[1])) {
			printf("\n未知错误，程序结束。\n");
			exit(1);
		}
	}


	//			//
	/* 文件初始化 */
	//			//

															//打开待清洗的文件
	if (fopen_s(&pfile, fileName, "r")) {
		fprintf_s(stderr, "\n %s open error.\n", fileName);
		exit(1);
	}

															//新建清洗后的数据文件
	if (fopen_s(&pstringFile, stringFile, "w+")) {
		fprintf_s(stderr, "\n %s open error.\n", stringFile);
		exit(1);
	}

															//新建包含URL的数据文件
	if (fopen_s(&pURLFile, URLFile, "w+")) {
		fprintf_s(stderr, "\n %s open error.\n", fileName);
		exit(1);
	}

															//新建包含dir的数据文件
	if (fopen_s(&pDirFile, dirFile, "w+")) {
		fprintf_s(stderr, "\n %s open error.\n", fileName);
		exit(1);
	}

															//新建包含ip的数据文件
	if (fopen_s(&pIPFile, ipFile, "w+")) {
		fprintf_s(stderr, "\n %s open error.\n", fileName);
		exit(1);
	}

	if (fopen_s(&pExeFile, exeFile, "w+")) {				//新建包含可执行文件的数据文件
		fprintf_s(stderr, "\n %s open error.\n", fileName);
		exit(1);
	}

	setvbuf(pfile, NULL, _IOFBF, BUFSIZ);
	setvbuf(pstringFile, NULL, _IOFBF, BUFSIZ);
	setvbuf(pURLFile, NULL, _IOFBF, BUFSIZ);
	setvbuf(pDirFile, NULL, _IOFBF, BUFSIZ);
	setvbuf(pIPFile, NULL, _IOFBF, BUFSIZ);
	setvbuf(pExeFile, NULL, _IOFBF, BUFSIZ);
	unsigned int k = 0;										//信息行数计数器


	//			  //
	/* 开始处理数据 */
	//			 //
															
															//读取待清洗文件，开始处理数据
	while (!feof(pfile)) {

		fgets(Line, LINE_MAX_SIZE - 1, pfile);

		str_len = strnlen_s(Line, LINE_MAX_SIZE);
		if (str_len < MIN_LEN + 1)							//字符串太小被抛弃
			continue;

		if (isURL(Line, str_len)){							//筛选URL信息
			k++;
			if (fputs(Line, pURLFile) < 0) {					
				fprintf_s(stderr, "\nwirte string error.");
				exit(1);
			}
			continue;
		}

		if (isDirectory(Line,str_len)){						//筛选Windows目录信息
			k++;
			if (fputs(Line, pDirFile) < 0) {				
				fprintf_s(stderr, "\nwirte string error.");
				exit(1);
			}
			continue;
		}

		if (isIP(Line, str_len)) {							//筛选IP信息
			k++;
			if (fputs(Line, pIPFile) < 0) {					
				fprintf_s(stderr, "\nwirte string error.");
				exit(1);
			}
			continue;
		}

		if (isExe(Line, str_len)) {							//筛选可执行文件信息
			k++;
			if (fputs(Line, pExeFile) < 0) {
				fprintf_s(stderr, "\nwirte string error.");
				exit(1);
			}
			continue;
		}

		if(isNoNumNoWord(Line, str_len)){					//包含任意一个非字母非数字的字符串都会被抛弃					
			k++;
			if (allRepeat(Line,str_len))					//筛选所有字符相同的字符串
				continue;
			if (fputs(Line, pstringFile) < 0) {					
				fprintf_s(stderr, "\nwirte string error.");
				exit(1);
			}
		}
	}
			
	printf("\nstringsplus共计输出%d行信息。\n",k);
	printf("生成文件 %s\n", stringFile);
	printf("生成文件 %s\n", URLFile);
	printf("生成文件 %s\n", dirFile);
	printf("生成文件 %s\n", ipFile);
	printf("生成文件 %s\n", exeFile);

	fclose(pfile);
	fclose(pstringFile);
	fclose(pURLFile);
	fclose(pDirFile);
	fclose(pIPFile);
	fclose(pExeFile);
	pfile = NULL;
	pstringFile = NULL;
	pURLFile = NULL;
	pDirFile = NULL;
	pIPFile = NULL;
	pExeFile = NULL;

	return 0;
}

//			  //
/*  函数定义区 */
//			 //

//如果字符串中包含一个非数字或非字母的字符，那么返回0
int	isNoNumNoWord(const char* strings, size_t len)
{
	for (size_t i = 0; i < len - 1; ++i) {
		if (strings[i] < 0x30 && strings[i] > 0x0A) {
			return 0;
		}

		else if (strings[i] > 0x7A) {
			return 0;
		}

		else if (strings[i] > 0x39 && strings[i] < 0x41) {
			return 0;
		}

		else if (strings[i] > 0x5A && strings[i] < 0x61) {
			return 0;
		}
		else
			continue; 
	}

	return 1;
}

//如果字符串中的所有字符相同，那么返回1
int	allRepeat(const char* strings, size_t len)
{
	for (size_t i = 1; i < len - 1; ++i)
	{
		if (strings[0] == strings[i])
			continue;
		else
			return 0;
	}
	return 1;
}

//如果字符串中的任何一个位置包含类似URL的数据，那么返回1
int isURL(const char* strings, size_t len)
{
	if (len < 6)
		return 0;

	for (size_t i = 0; i < len - 4; ++i)
	{
		if (strncmp(strings + i, "http",4) == 0)
			return 1;
		if (strncmp(strings + i, "Http", 4) == 0)
			return 1;
		if (strncmp(strings + i, "HTTP", 4) == 0)
			return 1;
		if (strncmp(strings + i, "www.", 4) == 0)
			return 1;
		if (strncmp(strings + i, "WWW.", 4) == 0)
			return 1;
	}

	for (size_t i = 0; i < len - 2; ++i)
	{
		if (strncmp(strings + i, "//", 2) == 0)
			return 1;
	}

	return 0;
}

//如果字符串中的任何一个位置包含类似Windows路径的信息，那么返回1
int isDirectory(const char* strings, size_t len)
{
	if (len < 9)
		return 0;

	for (size_t i = 0; i < len - 2; ++i)
	{
		if (strncmp(strings + i, "\\\\", 2) == 0)
			return 1;
		if (strncmp(strings + i, ":\\", 2) == 0)
			return 1;
	}
	return 0;
}

//如果字符串中的任何一个位置包含类似ip地址的信息，那么返回1
int	isIP(const char* strings, size_t len)
{
	int count = 0;
	for (size_t i = 1; i < len; ++i) {
		if (strings[i] == '.') {
			if (isdigit(strings[i - 1]) != 0 && isdigit(strings[i + 1]) != 0) {
				i = i + 1;
				count++;
			}
		}
	}

	if (count >=3)
		return 1;
	else
		return 0;
}

//如果字符串中任何一个位置包含类似.exe .dll这样的可执行文件信息，那么返回1
int	isExe(const char* strings, size_t len)
{
	for (size_t i = 0; i < len - 4; ++i)
	{
		if (strncmp(strings + i, ".exe", 4) == 0)
			return 1;
		if (strncmp(strings + i, ".Exe", 4) == 0)
			return 1;
		if (strncmp(strings + i, ".EXE", 4) == 0)
			return 1;
		if (strncmp(strings + i, ".dll", 4) == 0)
			return 1;
		if (strncmp(strings + i, ".DLL", 4) == 0)
			return 1;
		if (strncmp(strings + i, ".Dll", 4) == 0)
			return 1;
		if (strncmp(strings + i, ".sys", 4) == 0)
			return 1;
		if (strncmp(strings + i, ".Sys", 4) == 0)
			return 1;
		if (strncmp(strings + i, ".SYS", 4) == 0)
			return 1;
	}

	return 0;
}