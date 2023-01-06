#pragma once
#include "list.h"
#include <stdio.h>
#include <string.h>

static job_t* jobs[255];
static job_nb = 0;

job_t* get_job_by_name(char *name) {
	for (int i = 0; i < job_nb; ++i) {
		if (strcmp(name, jobs[i]->title) == 0) {
			return jobs[i];
		}
	}
	job_t* res = malloc(sizeof(job_t)); //开辟内存保存一个job
	jobs[job_nb] = res;
	job_nb += 1;
	res->precedence = malloc(sizeof(list_t));
	res->posteriority = malloc(sizeof(list_t));
	strcpy(res->title, name);
	return res;
}

int insert_to_list(list_t* list,job_t* job) {
	elmlist_t* e = malloc(sizeof(elmlist_t));
	e->jb = job;
	if (list->numelm == 0) {
		list->numelm = 1;
		list->head = e;
		list->tail = e;
	}
	else {
		list->numelm += 1;
		list->tail->next = e;
		e->prev = list->tail;
		list->tail = e;
	}
	return list->numelm;
}


list_t* read_graph(char* str) {
	
	char buff[255];
	
	char jobs_str[255][255];
	FILE* fp = fopen(str, "r+");
	while (!feof(fp)) //只要还没到文件末尾，就执行循环
	{
		fgets(buff, 255, (FILE*)fp); //读取一行放到buff中
		int result = split(jobs_str, buff, ' ');
		if (result > 1) {
			job_t* j = get_job_by_name("fuck");
			printf("%s\n", j->title);
			//j->life = atoi(jobs_str[1]);
			
			//for (int i = 2; i < result; ++i) {
				//job_t* jp = get_job_by_name(jobs_str[i]);
				//insert_to_list(j->precedence, jp);
				//insert_to_list(jp->posteriority, j);
			//}
		}
	}

	printf("%d", job_nb);



	fclose(fp);
};



// 将str字符以spl分割,存于dst中，并返回子字符串数量
int split(char dst[][255], char* str, const char* spl)
{
	int str_nb = 0;
	int char_nb = 0;
	int prev_is_char = 0;

	for (int i = 0; i < 255; ++i) {
		if (str[i] == '\0') {
			if (prev_is_char == 1) {
				dst[str_nb][char_nb] = '\0';
				str_nb += 1;
				char_nb = 0;
			}
			break;
		}

		if (str[i] == ' ') {
			if (prev_is_char == 1) {
				dst[str_nb][char_nb] = '\0';
				str_nb += 1;
				char_nb = 0;
			}
			prev_is_char = 0;
		}

		else if (str[i] != ' ') {
			dst[str_nb][char_nb] = str[i];
			char_nb += 1;
			prev_is_char = 1;
		}
	}

	return str_nb;
}

void view_job(job_t *a) {};