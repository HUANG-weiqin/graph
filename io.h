#include "list.h"
#include <stdio.h>
#include <string.h>
list_t* read_graph(char* str) {
	FILE* fp = fopen(str, "r+");
	char buff[255];
	job_t* jobs[255];
	char jobs_str[255][255];


	while (!feof(fp)) //ֻҪ��û���ļ�ĩβ����ִ��ѭ��
	{
		fgets(buff, 255, (FILE*)fp); //��ȡһ�зŵ�buff��
		int result = split(jobs_str, buff, ' ');
	}
};



// ��str�ַ���spl�ָ�,����dst�У����������ַ�������
int split(char dst[][255], char* str, const char* spl)
{
	int str_nb = 0;
	int char_nb = 0;
	int prev_is_blank = 1;

	for (int i = 0; i < 255; ++i) {
		if (str[i] == '\0') {
			dst[str_nb - 1][char_nb] = '\0';
			break;
		}

		if (str[i] == ' ') {
			if (prev_is_blank == 0) {
				dst[str_nb - 1][char_nb] = '\0';
			}
			prev_is_blank = 1;
		}

		if (str[i] != ' ') {
			if (prev_is_blank == 1) {
				str_nb += 1;
				char_nb = 0;
			}
			
			dst[str_nb - 1][char_nb++] = str[i];
			prev_is_blank = 0;
		}
	}

	return str_nb;
}

void view_job(job_t *a) {};