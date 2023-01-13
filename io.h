#pragma once
#include "list.h"
#include <stdio.h>
#include <string.h>
#define UNDEF -1
#define BIGNUMB 99999

static job_t* jobs[255]; /*������������Ѿ������õ�job��ָ��*/
static job_nb = 0; /*������������Ѿ������õ�job������*/

/*���ݽڵ������õ�һ���ڵ㣬����ýڵ�û���������ȴ����ٷ��أ�����Ѿ�����
��ֱ�ӷ��ظýڵ㣬�÷��������ͬ���ڵ㱻�ظ�����
*/
job_t* get_job_by_name(char *name) {
	/*���ȱ���һ��jobs������Ҫ�õĽڵ��ǲ����Ѿ������ˣ�����Ѿ������˾�ֱ��return
	*/
	for (int i = 0; i < job_nb; ++i) {
		if (strcmp(name, jobs[i]->title) == 0) {
			return jobs[i];
		}
	}
	/*	����˵��Ҫ�õĽڵ㻹û��������ô�ȴ����ٷ���
	*/
	job_t* res = malloc(sizeof(job_t)); //��̬�����ڴ汣��һ��job
	res->rank = UNDEF;// rank����Ϊ-1����û��ʼ��������ڵ��rank
	res->life = UNDEF;
	res->au_plus_tot = UNDEF;
	res->au_plus_tard = UNDEF;
	res->marge_totale = UNDEF;
	res->marge_libre = UNDEF;
	res->critique = 0;
	res->output_degree = 0;
	res->input_degree = 0;
	res->dyn_input_degree = 0;
	jobs[job_nb] = res; //�������õĽڵ�ŵ�jobs�У������´�Ҫ�õ�ʱ�����ֱ���ҵ��������ظ�����
	job_nb += 1;
	res->precedence = creat_list();
	res->posteriority = creat_list();
	strcpy(res->title, name);
	return res;
}


/*str����Ҫ��ȡ���ļ������ƣ����������ȡ�ļ����ݣ��������ݴ���һ�� list ������е�job�����ظ�list
*/
list_t* read_graph(char* str) {
	char buff[255]; // ������ʱ����ļ���һ�е�����
	char jobs_str[255][255]; // ����һ���Կո�ָ�õ�һ�������ַ�����������
	/* ���� �ļ�����һ��    
		DC  4   DS NIL
		��һ�����ļ��еĵڶ���
		�ᱻ�ָ��
		jobs_str[0][..] =   "DC"
		jobs_str[1][..] =   "4"
		jobs_str[2][..] =   "DS"
		jobs_str[3][..] =   "NIL"
		���ĸ��ַ������ָ����split�����طָ��õ��˼����ַ�����������4
	*/
	FILE* fp = fopen(str, "r+");
	list_t* res = creat_list();
	while (!feof(fp)) //���ļ�ÿ�ζ�ȡһ�С�ֻҪ��û���ļ�ĩβ����ִ��ѭ��
	{
		fgets(buff, 255, (FILE*)fp); //��ȡһ�зŵ�buff��
		int result = split(jobs_str, buff, ' '); /*������зָ�ָ����ݷŵ�jobs str�У����صõ����ַ�������*/
		if (result > 1) {
			job_t* j = get_job_by_name(*jobs_str);
			j->life = atoi(jobs_str[1]); /*atoi ���ַ���ת��Ϊ���֣�������������� �ַ���4��ת��Ϊint 4���� life��*/
			for (int i = 2; i < result-1; ++i) {
				job_t* jp = get_job_by_name(jobs_str[i]); /*����������������е� DS��DS����DC�� ǰ�ڵ�*/
				insert_to_list(j->precedence, jp); /*DS �ڵ�ŵ� DC�ڵ��ǰ�ڵ��б��У���ΪDC�����ж��ǰ�ڵ�������list������е�*/
				insert_to_list(jp->posteriority, j); /*DS��DC��ǰ�ڵ����DC��DS�ĺ�ڵ㣬��ô��DC����DS�ĺ�ڵ��б���*/
				j->input_degree += 1; /*��ΪDC��1��ǰ�ڵ㣬��������input degree + 1*/
				jp->output_degree += 1; /*��ΪDS��1����ڵ㣬��������output degree + 1*/
			}
			insert_to_list(res, j);
		}
	}
	fclose(fp);

	return res;
};



// ��str�ַ���spl�ָ�,����dst�У����������ַ�������������̫����������ԭ��ֻ��������ȡ���ݵ�
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