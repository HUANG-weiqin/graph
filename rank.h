#include "list.h"

int iDegreeJobCmp(job_t* a, job_t* b) {
	return a->input_degree - b->input_degree;
}

int iRankJobCmp(job_t* a, job_t* b) {
	return a->rank - b->rank;
}

void ranking(list_t* G) {

	//先找到所有开始节点，设为0 rank
	
	for (elmlist_t* e = G->head; e; e = e->next) {

		if (e->jb->input_degree == 0)
		{
			e->jb->rank = 0;
		}
	}

	int ok; //结束标志
	do
	{
		ok = 1;
		for (elmlist_t* e = G->head; e;e = e->next) {
			if (e->jb->rank == UNDEF) {
				ok = 0;
			}

			for (elmlist_t* prev = get_head(e->jb->precedence); prev; prev = prev->next) {
				if (prev->jb->rank == UNDEF) {
					continue;
				}
				int new_rank = prev->jb->rank + 1;
				if (new_rank > e->jb->rank) {
					e->jb->rank = new_rank;
				}
			}
		}

	} while (ok != 1);

	quick_sort(G, &iRankJobCmp);
};


void prune(list_t* G) {
	for (elmlist_t* cur = G->head; cur; cur = cur->next) {
		for (elmlist_t* prev = get_head(cur->jb->precedence); prev; ) {
			if (cur->jb->rank - prev->jb->rank > 1) {
				elmlist_t*  to_remove = prev;
				prev = prev->next;
				take_out_from_list(cur->jb->precedence, to_remove);

				cur->jb->input_degree -= 1;
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

void calcule_plustot(list_t* G) {

	for (elmlist_t* e = G->head; e; e = e->next) {
		if (e->jb->input_degree == 0)
		{
			e->jb->au_plus_tot = 0;
		}
	}

	for (elmlist_t* cur = G->head; cur; cur = cur->next) {
		for (elmlist_t* prev = get_head(cur->jb->precedence); prev; prev = prev->next) {
			int newtot = prev->jb->au_plus_tot + prev->jb->life;
			if (newtot > cur->jb->au_plus_tot) {
				cur->jb->au_plus_tot = newtot;
			}
		}
	}
}

void calcule_plustard(list_t* G) {
	for (elmlist_t* e = G->head; e; e = e->next) {
		if (e->jb->output_degree == 0)
		{
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

void add_alpha_omega(list_t* G) {
	job_t* ja = get_job_by_name("alpha");
	job_t* jo = get_job_by_name("omega");
	for (elmlist_t* e = G->head; e; e = e->next) {
		if (e->jb->input_degree == 0)
		{
			insert_to_list(ja->posteriority, e->jb);
			insert_to_list(e->jb->precedence, ja);
			ja->input_degree = 0;
			e->jb->input_degree += 1;
			ja->rank = -1;
			ja->life = 0;

		}
		if (e->jb->output_degree == 0)
		{
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
	quick_sort(G, &iRankJobCmp);
}

void marges_totales(list_t *G) {
	add_alpha_omega(G);
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

