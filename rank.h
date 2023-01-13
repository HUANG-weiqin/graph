#include "list.h"


/*按照input degree由小到大来排序，这个函数就是comp，我们会将这个函数的指针传给 快速排序算法*/
int iDegreeJobCmp(job_t* a, job_t* b) {
	return a->input_degree - b->input_degree;
}

/*按照rank 由小到大来排序，这个函数就是comp，我们会将这个函数的指针传给 快速排序算法*/
int iRankJobCmp(job_t* a, job_t* b) {
	return a->rank - b->rank;
}

void ranking(list_t* G) {

	/*遍历G 找到所有没有input degree为0的节点，即没有 前任务 的任务，将任务的rank设置为0
	  因为没有前置任务 所以这些任务必然要第一个开始做
	*/
	
	for (elmlist_t* e = G->head; e; e = e->next) {

		if (e->jb->input_degree == 0)
		{
			e->jb->rank = 0;
		}
	}

	int ok; //结束标志

	/*rank循环计算*/
	do
	{
		ok = 1;

		/*遍历G*/
		for (elmlist_t* e = G->head; e;e = e->next) {
			if (e->jb->rank == UNDEF) {
				/*只要G中有一个节点e的rank还为定义，则不ok，不能结束rank循环计算
				  当所有的节点的rank都有定义后，才结束
				*/
				ok = 0;
			}

			/*对于当前节点e，遍历e的所有 前任务 prev*/
			for (elmlist_t* prev = get_head(e->jb->precedence); prev; prev = prev->next) {
				if (prev->jb->rank == UNDEF) {
					/*如果前任务prev的rank还没定义，跳过这个prev*/
					continue;
				}
				/*如果前任务prev的 rank + 1 比  当前节点e 的rank要大，则当前节点e的rank就等于前置节点rank + 1 */
				int new_rank = prev->jb->rank + 1;
				if (new_rank > e->jb->rank) {
					e->jb->rank = new_rank;
				}
			}
		}

	} while (ok != 1);

	/*所有的任务的rank都已经被计算好了，就快速排序，以rank进行排序*/
	quick_sort(G, &iRankJobCmp);
};


/*prune的原理是，假如我们有两个 job 一个是j1 一个 j2，且j2是j1的前任务
  j1的rank是5
  j2的rank是3

  那么肯定 j1 还有另一个 前任务 j3 且j3的rank是 5-1 = 4
  这代表， j2 是不是 j1 的前任务都无所谓，因为 j3完成了 才能开始 j1
  而j2因为rank小于j3，所以肯定是先于j3完成的，所以 j1 与 j2 什么时候结束没有关系
  这样的话，j2 是不是 j1的前任务都无所谓，我们就可以将j2从j1的前任务列表中删除，也将
  j1从j2的后任务列表中删除

  换句话说，对于任意一对job ，如果他们之间的rank 差 大于等于 2，那么他们之间的先后关系就可以被删除
  prune就是做这个事情的，找到所有可以删除的前后关系，删除之
*/
void prune(list_t* G) {
	for (elmlist_t* cur = G->head; cur; cur = cur->next) {
		for (elmlist_t* prev = get_head(cur->jb->precedence); prev; ) {
			/*遍历G 当前的节点cur和cur的所有prev，如果他们之间的rank差大于1，那么就删除他们的先后关系*/

			if (cur->jb->rank - prev->jb->rank > 1) {
				elmlist_t*  to_remove = prev;
				prev = prev->next;
				take_out_from_list(cur->jb->precedence, to_remove); 

				cur->jb->input_degree -= 1; /*删除先后关系记得要更改degree*/
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


/*计算每个节点的最早开始时间*/
void calcule_plustot(list_t* G) {

	/*遍历节点，如果没有前任务，说明这个任务可以最早开始，则plus tot设为0*/
	for (elmlist_t* e = G->head; e; e = e->next) {
		if (e->jb->input_degree == 0)
		{
			e->jb->au_plus_tot = 0;
		}
	}

	/*遍历节点，对于当前节点cur，他的最早开始日期取决于他的所有前置节点prev中 最晚的  prev 的最早开始日期 + prev的life*/
	for (elmlist_t* cur = G->head; cur; cur = cur->next) {
		for (elmlist_t* prev = get_head(cur->jb->precedence); prev; prev = prev->next) {
			int newtot = prev->jb->au_plus_tot + prev->jb->life;
			if (newtot > cur->jb->au_plus_tot) {
				cur->jb->au_plus_tot = newtot;
			}
		}
	}
}


/*计算每个节点的最晚开始时间 与最早开始日期计算方式有所不同*/
void calcule_plustard(list_t* G) {
	for (elmlist_t* e = G->head; e; e = e->next) {
		if (e->jb->output_degree == 0)
		{
			/*对于output degree为0的节点，即最后的job，它的最早开始就是最早晚开始日期，没有摸鱼的余地*/
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

/*这个是给整个图G添加两个特殊的任务节点，阿尔法 和 欧米茄
	阿尔法 代表 所有任务的开始，即最早的任务
	欧米茄 代表 所有任务的结束，即最晚的任务
	他们两个的life都是0 因为是虚拟的任务，只是用来标记开始和结束
	否则，可能有多个开始任务和多个结束任务，所以添加这俩是为了限制开始和结束的任务的数量都为1
	以方便我们计算最早和最晚
*/
void add_alpha_omega(list_t* G) {
	job_t* ja = get_job_by_name("alpha");
	job_t* jo = get_job_by_name("omega");
	for (elmlist_t* e = G->head; e; e = e->next) {
		if (e->jb->input_degree == 0)
		{
			/*如果当前节点e是开始job，那么将它的前置节点设为阿尔法*/
			insert_to_list(ja->posteriority, e->jb);
			insert_to_list(e->jb->precedence, ja);
			ja->input_degree = 0;
			e->jb->input_degree += 1;
			ja->rank = -1;
			ja->life = 0;

		}
		if (e->jb->output_degree == 0)
		{
			/*如果当前节点e是结束job，那么将它的后置节点设为欧米茄*/
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
	quick_sort(G, &iRankJobCmp); /*塞完后记得要排序，因为这俩是新的*/
}

/*marges_totales 就是一个节点的 最晚 - 最早 ，即这个job有多少摸鱼时间*/
void marges_totales(list_t *G) {
	add_alpha_omega(G); /*要先添加奥米加和阿尔法来限制 开始和结束的数量为1才能进行计算最早最晚*/
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

