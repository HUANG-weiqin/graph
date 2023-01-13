#include "list.h"


/*����input degree��С���������������������comp�����ǻὫ���������ָ�봫�� ���������㷨*/
int iDegreeJobCmp(job_t* a, job_t* b) {
	return a->input_degree - b->input_degree;
}

/*����rank ��С���������������������comp�����ǻὫ���������ָ�봫�� ���������㷨*/
int iRankJobCmp(job_t* a, job_t* b) {
	return a->rank - b->rank;
}

void ranking(list_t* G) {

	/*����G �ҵ�����û��input degreeΪ0�Ľڵ㣬��û�� ǰ���� �����񣬽������rank����Ϊ0
	  ��Ϊû��ǰ������ ������Щ�����ȻҪ��һ����ʼ��
	*/
	
	for (elmlist_t* e = G->head; e; e = e->next) {

		if (e->jb->input_degree == 0)
		{
			e->jb->rank = 0;
		}
	}

	int ok; //������־

	/*rankѭ������*/
	do
	{
		ok = 1;

		/*����G*/
		for (elmlist_t* e = G->head; e;e = e->next) {
			if (e->jb->rank == UNDEF) {
				/*ֻҪG����һ���ڵ�e��rank��Ϊ���壬��ok�����ܽ���rankѭ������
				  �����еĽڵ��rank���ж���󣬲Ž���
				*/
				ok = 0;
			}

			/*���ڵ�ǰ�ڵ�e������e������ ǰ���� prev*/
			for (elmlist_t* prev = get_head(e->jb->precedence); prev; prev = prev->next) {
				if (prev->jb->rank == UNDEF) {
					/*���ǰ����prev��rank��û���壬�������prev*/
					continue;
				}
				/*���ǰ����prev�� rank + 1 ��  ��ǰ�ڵ�e ��rankҪ����ǰ�ڵ�e��rank�͵���ǰ�ýڵ�rank + 1 */
				int new_rank = prev->jb->rank + 1;
				if (new_rank > e->jb->rank) {
					e->jb->rank = new_rank;
				}
			}
		}

	} while (ok != 1);

	/*���е������rank���Ѿ���������ˣ��Ϳ���������rank��������*/
	quick_sort(G, &iRankJobCmp);
};


/*prune��ԭ���ǣ��������������� job һ����j1 һ�� j2����j2��j1��ǰ����
  j1��rank��5
  j2��rank��3

  ��ô�϶� j1 ������һ�� ǰ���� j3 ��j3��rank�� 5-1 = 4
  ����� j2 �ǲ��� j1 ��ǰ��������ν����Ϊ j3����� ���ܿ�ʼ j1
  ��j2��ΪrankС��j3�����Կ϶�������j3��ɵģ����� j1 �� j2 ʲôʱ�����û�й�ϵ
  �����Ļ���j2 �ǲ��� j1��ǰ��������ν�����ǾͿ��Խ�j2��j1��ǰ�����б���ɾ����Ҳ��
  j1��j2�ĺ������б���ɾ��

  ���仰˵����������һ��job ���������֮���rank �� ���ڵ��� 2����ô����֮����Ⱥ��ϵ�Ϳ��Ա�ɾ��
  prune�������������ģ��ҵ����п���ɾ����ǰ���ϵ��ɾ��֮
*/
void prune(list_t* G) {
	for (elmlist_t* cur = G->head; cur; cur = cur->next) {
		for (elmlist_t* prev = get_head(cur->jb->precedence); prev; ) {
			/*����G ��ǰ�Ľڵ�cur��cur������prev���������֮���rank�����1����ô��ɾ�����ǵ��Ⱥ��ϵ*/

			if (cur->jb->rank - prev->jb->rank > 1) {
				elmlist_t*  to_remove = prev;
				prev = prev->next;
				take_out_from_list(cur->jb->precedence, to_remove); 

				cur->jb->input_degree -= 1; /*ɾ���Ⱥ��ϵ�ǵ�Ҫ����degree*/
				to_remove->jb->output_degree -= 1;

				free(to_remove);
			}
			else
			{
				prev = prev->next;
			}
		}
	}
};


/*����ÿ���ڵ�����翪ʼʱ��*/
void calcule_plustot(list_t* G) {

	/*�����ڵ㣬���û��ǰ����˵���������������翪ʼ����plus tot��Ϊ0*/
	for (elmlist_t* e = G->head; e; e = e->next) {
		if (e->jb->input_degree == 0)
		{
			e->jb->au_plus_tot = 0;
		}
	}

	/*�����ڵ㣬���ڵ�ǰ�ڵ�cur���������翪ʼ����ȡ������������ǰ�ýڵ�prev�� �����  prev �����翪ʼ���� + prev��life*/
	for (elmlist_t* cur = G->head; cur; cur = cur->next) {
		for (elmlist_t* prev = get_head(cur->jb->precedence); prev; prev = prev->next) {
			int newtot = prev->jb->au_plus_tot + prev->jb->life;
			if (newtot > cur->jb->au_plus_tot) {
				cur->jb->au_plus_tot = newtot;
			}
		}
	}
}


/*����ÿ���ڵ������ʼʱ�� �����翪ʼ���ڼ��㷽ʽ������ͬ*/
void calcule_plustard(list_t* G) {
	for (elmlist_t* e = G->head; e; e = e->next) {
		if (e->jb->output_degree == 0)
		{
			/*����output degreeΪ0�Ľڵ㣬������job���������翪ʼ����������ʼ���ڣ�û����������*/
			e->jb->au_plus_tard = e->jb->au_plus_tot;
		}
		else
		{
			e->jb->au_plus_tard = BIGNUMB;
		}
	}

	for (elmlist_t* cur = G->tail; cur; cur = cur->prev) {
		for (elmlist_t* pose = get_head(cur->jb->posteriority); pose; pose = pose->next) {
			int newtard = pose->jb->au_plus_tard - cur->jb->life;
			if (newtard < cur->jb->au_plus_tard) {
				cur->jb->au_plus_tard = newtard;
			}
		}
	}
}

/*����Ǹ�����ͼG����������������ڵ㣬������ �� ŷ����
	������ ���� ��������Ŀ�ʼ�������������
	ŷ���� ���� ��������Ľ����������������
	����������life����0 ��Ϊ�����������ֻ��������ǿ�ʼ�ͽ���
	���򣬿����ж����ʼ����Ͷ�����������������������Ϊ�����ƿ�ʼ�ͽ����������������Ϊ1
	�Է������Ǽ������������
*/
void add_alpha_omega(list_t* G) {
	job_t* ja = get_job_by_name("alpha");
	job_t* jo = get_job_by_name("omega");
	for (elmlist_t* e = G->head; e; e = e->next) {
		if (e->jb->input_degree == 0)
		{
			/*�����ǰ�ڵ�e�ǿ�ʼjob����ô������ǰ�ýڵ���Ϊ������*/
			insert_to_list(ja->posteriority, e->jb);
			insert_to_list(e->jb->precedence, ja);
			ja->input_degree = 0;
			e->jb->input_degree += 1;
			ja->rank = -1;
			ja->life = 0;

		}
		if (e->jb->output_degree == 0)
		{
			/*�����ǰ�ڵ�e�ǽ���job����ô�����ĺ��ýڵ���Ϊŷ����*/
			insert_to_list(jo->precedence, e->jb);
			insert_to_list(e->jb->posteriority, jo);
			jo->output_degree = 0;
			jo->life = 0;
			e->jb->output_degree += 1;
			int newrank = e->jb->rank + 1;
			if (newrank > jo->rank) {
				jo->rank = newrank;
			}
			
		}
	}
	insert_to_list(G, jo);
	insert_to_list(G, ja);
	quick_sort(G, &iRankJobCmp); /*�����ǵ�Ҫ������Ϊ�������µ�*/
}

/*marges_totales ����һ���ڵ�� ���� - ���� �������job�ж�������ʱ��*/
void marges_totales(list_t *G) {
	add_alpha_omega(G); /*Ҫ����Ӱ��׼ӺͰ����������� ��ʼ�ͽ���������Ϊ1���ܽ��м�����������*/
	calcule_plustot(G);
	calcule_plustard(G);
	for (elmlist_t* cur = G->head; cur; cur = cur->next) {
		job_t* jb = cur->jb;
		jb->marge_totale =jb->au_plus_tard - jb->au_plus_tot;

		if (jb->marge_totale == 0) {
			jb->critique = 1;
		}
	}
};

