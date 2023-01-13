#pragma once
#include "list.h"
#include <stdio.h>
#include <string.h>
#define UNDEF -1
#define BIGNUMB 99999

static job_t* jobs[255]; /*用来存放所有已经创建好的job的指针*/
static job_nb = 0; /*用来存放所有已经创建好的job的数量*/

/*根据节点名称拿到一个节点，如果该节点没创建，就先创建再返回，如果已经创建
则直接返回该节点，该方法会避免同名节点被重复创建
*/
job_t* get_job_by_name(char *name) {
	/*首先遍历一遍jobs，看看要拿的节点是不是已经创建了，如果已经创建了就直接return
	*/
	for (int i = 0; i < job_nb; ++i) {
		if (strcmp(name, jobs[i]->title) == 0) {
			return jobs[i];
		}
	}
	/*	现在说明要拿的节点还没创建，那么先创建再返回
	*/
	job_t* res = malloc(sizeof(job_t)); //动态分配内存保存一个job
	res->rank = UNDEF;// rank设置为-1代表还没开始计算这个节点的rank
	res->life = UNDEF;
	res->au_plus_tot = UNDEF;
	res->au_plus_tard = UNDEF;
	res->marge_totale = UNDEF;
	res->marge_libre = UNDEF;
	res->critique = 0;
	res->output_degree = 0;
	res->input_degree = 0;
	res->dyn_input_degree = 0;
	jobs[job_nb] = res; //将创建好的节点放到jobs中，方便下次要拿的时候可以直接找到而避免重复创建
	job_nb += 1;
	res->precedence = creat_list();
	res->posteriority = creat_list();
	strcpy(res->title, name);
	return res;
}


/*str代表要读取的文件的名称，这个方法读取文件内容，根据内容创建一个 list 存放所有的job并返回该list
*/
list_t* read_graph(char* str) {
	char buff[255]; // 用来临时存放文件里一行的内容
	char jobs_str[255][255]; // 将这一行以空格分割，得到一连串的字符串放入其中
	/* 比如 文件中有一行    
		DC  4   DS NIL
		这一行是文件中的第二行
		会被分割成
		jobs_str[0][..] =   "DC"
		jobs_str[1][..] =   "4"
		jobs_str[2][..] =   "DS"
		jobs_str[3][..] =   "NIL"
		这四个字符串，分割方法是split，返回分割后得到了几个字符串，这里是4
	*/
	FILE* fp = fopen(str, "r+");
	list_t* res = creat_list();
	while (!feof(fp)) //打开文件每次读取一行。只要还没到文件末尾，就执行循环
	{
		fgets(buff, 255, (FILE*)fp); //读取一行放到buff中
		int result = split(jobs_str, buff, ' '); /*这里进行分割，分割内容放到jobs str中，返回得到的字符串数量*/
		if (result > 1) {
			job_t* j = get_job_by_name(*jobs_str);
			j->life = atoi(jobs_str[1]); /*atoi 将字符串转换为数字，比如上面的例子 字符串4被转换为int 4放入 life中*/
			for (int i = 2; i < result-1; ++i) {
				job_t* jp = get_job_by_name(jobs_str[i]); /*这里就是上面例子中的 DS，DS代表DC的 前节点*/
				insert_to_list(j->precedence, jp); /*DS 节点放到 DC节点的前节点列表中，因为DC可以有多个前节点所以用list存放所有的*/
				insert_to_list(jp->posteriority, j); /*DS是DC的前节点代表DC是DS的后节点，那么将DC放入DS的后节点列表中*/
				j->input_degree += 1; /*因为DC有1个前节点，所以它的input degree + 1*/
				jp->output_degree += 1; /*因为DS有1个后节点，所以它的output degree + 1*/
			}
			insert_to_list(res, j);
		}
	}
	fclose(fp);

	return res;
};



// 将str字符以spl分割,存于dst中，并返回子字符串数量，不用太深究这个函数的原理，只是用来读取数据的
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