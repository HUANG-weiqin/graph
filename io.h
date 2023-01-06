#pragma once
#include "list.h"
#include <stdio.h>
#include <string.h>
#define UNDEF -1

static job_t* jobs[255];
static job_nb = 0;


job_t* get_job_by_name(char *name) {
	for (int i = 0; i < job_nb; ++i) {
		if (strcmp(name, jobs[i]->title) == 0) {
			return jobs[i];
		}
	}
	job_t* res = malloc(sizeof(job_t)); //开辟内存保存一个job
	res->rank = UNDEF;// rank设置为-1代表还没开始计算这个节点的rank
	res->life = UNDEF;
	res->au_plus_tot = UNDEF;
	res->au_plus_tard = UNDEF;
	res->marge_totale = UNDEF;
	res->marge_libre = UNDEF;
	res->critique = UNDEF;
	jobs[job_nb] = res;
	job_nb += 1;
	res->precedence = creat_list();
	res->posteriority = creat_list();
	strcpy(res->title, name);
	return res;
}

int insert_to_list(list_t* list,job_t* job) {
	elmlist_t* e = malloc(sizeof(elmlist_t));
	e->next = 0;
	e->prev = 0;
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
	list_t* res = creat_list();
	while (!feof(fp)) //只要还没到文件末尾，就执行循环
	{
		fgets(buff, 255, (FILE*)fp); //读取一行放到buff中
		int result = split(jobs_str, buff, ' ');
		if (result > 1) {
			job_t* j = get_job_by_name(*jobs_str);
			printf("%s\n", j->title);
			j->life = atoi(jobs_str[1]);
			for (int i = 2; i < result-1; ++i) {
				job_t* jp = get_job_by_name(jobs_str[i]);
				insert_to_list(j->precedence, jp);
				insert_to_list(jp->posteriority, j);
			}
			insert_to_list(res, j);
		}
	}
	fclose(fp);

	return res;
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

void view_job(job_t* J) {
	printf("JOB %s\n\tpreceeded by [", J->title);
	for (elmlist_t* E = get_head(J->precedence); E; E = E->next) {
		printf(" %s", E->jb->title);
	}
	printf(" ]\n");
	// if ( !get_numelm ( J->posteriority ) ) printf ( "\t" );
	printf("\tfollowed by [");
	for (elmlist_t* E = get_head(J->posteriority); E; E = E->next) {
		printf(" %s",get_title(E->jb));
	}
	printf(" ]\n");
	printf("\tiDeg=%d\toDeg=%d\tlife=%2.2lf", J->input_degree, J->output_degree, J->life);
	printf("\trank=");
	if (J->rank == UNDEF) printf("U"); else printf("%d", J->rank);
	printf("\tearly=");
	if (J->au_plus_tot == UNDEF) printf("U"); else printf("%2.2lf", J->au_plus_tot);
	printf("\tlate=");
	if (J->au_plus_tard == UNDEF) printf("U"); else printf("%2.2lf", J->au_plus_tard);
	printf("\ttotale= ");
	if (J->marge_totale == UNDEF) printf("U"); else printf("%2.2lf", J->marge_totale);
	printf("\tcritical= ");
	if (J->critique) printf("Y\n"); else printf("N\n");
}


void view_list(list_t* L, void (*ptrf)(job_t* a)) {
	printf("\t\t====================\n");
	if (L->numelm == 0) {
		printf("[   ] //empty list_t\n");
	}
	else {
		elmlist_t* iterator = L->head;
		while (iterator) {
			ptrf(iterator->jb);
			iterator = iterator->next;
		}
	}
	printf("\t\t====================\n\n");
}