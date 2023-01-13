#pragma once
typedef struct {
    char title[10] ; // Nom de la tache
    double life ; // Duree de la tache    这个job要多少时间
    int input_degree ; // Son degre de dependance  有多少个前job
    int output_degree ; // Les taches qui en dependent 有多少个后job
    int rank ; // Rang de latache  排在第几个开始
    int dyn_input_degree ; // Facilite de prog 没有用到
    struct list_t * precedence ; // Les taches precedentes 所有的前节点
    struct list_t * posteriority ; // Les taches ulterieures 所有的后节点
    double au_plus_tot ; // Date au plustot 最早什么时候开始
    double au_plus_tard ; // Date au plustard 最晚什么时候开始
    double marge_totale ; // Marge totale 等于 最晚 - 最早，其实就是这个任务有几天摸鱼时间
    double marge_libre ; // Marge libre 和上面区别不大，输出的不要求算这个就没算
    int critique ; // Une tache critique 其实就是 这个任务有没有摸鱼时间 ， 有 为1 没有为 0
} job_t;


/*双向链表list的节点，保存着一个job和前面一个链表节点和后面的链表节点*/
typedef struct element{
    struct element* prev;
    struct element* next;
    job_t* jb;
}elmlist_t;

/*链表本体，保存着一个头部链表节点和尾部链表节点和一个int代表这个链表有多少个节点*/
typedef struct  {
    struct element * head ;
    struct element * tail ;
    int numelm ;
}list_t;


elmlist_t * get_head(list_t *ls){
    return ls->head;
}

elmlist_t* get_tail(list_t* ls) {
    return ls->tail;
}

/*动态分配一个链表*/
list_t* creat_list() {
    list_t* res = malloc(sizeof(list_t));
    res->head = 0;
    res->tail = 0;
    res->numelm = 0;
    return res;
}

/*将一个job塞入一个新的动态分配好的链表节点，然后直接塞入链表的尾部*/
int insert_to_list(list_t* list, job_t* job) {
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

/*将一个已有的链表节点e塞入链表list的头部*/
int cons(list_t* list, elmlist_t* e) {

    if (list->numelm == 0) {
        list->numelm = 1;
        list->head = e;
        list->tail = e;
    }
    else {
        list->numelm += 1;
        list->head->prev = e;
        e->next = list->head;
        list->head = e;
    }
    return list->numelm;
}

/*将一个已有的链表节点e塞入链表list的尾部*/
int queue(list_t* list, elmlist_t* e) {
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


/*将elem这个节点从链表list中拿出来，list减少一个节点，现在这个elem变成了一个独立的节点不依附于任何list*/
void take_out_from_list(list_t* list, elmlist_t* elem) {
    if (list->numelm == 1 && list->head == elem) {
        list->head = 0;
        list->tail = 0;
    }
    else if (list->head == elem) {
        list->head = elem->next;
        elem->next->prev = 0;
    }
    else if (list->tail == elem) {
        list->tail = elem->prev;
        elem->prev->next = 0;
    }
    else
    {
        elem->prev->next = elem->next;
        elem->next->prev = elem->prev;
    }
    elem->next = 0;
    elem->prev = 0;
    list->numelm -= 1;
}


char* get_title(job_t * j) {
    return j->title;
}

/*快速排序算法，需要先去看快速排序的基本解释再来看这里的注释*/
void quick_sort(list_t *G,int (* comp)(job_t *a,job_t *b)){
    if (G->numelm < 2) /*递归结束条件， 即当链表只有1个或者0个节点的时候不需要排序，直接返回*/
        return;

    elmlist_t* pivot = G->head; 
    take_out_from_list(G, pivot);  /*我们将链表G的头部节点拿出来 叫做 pivot */

    /*创造两个个临时对象，这里和动态分配不同，这个没有用malloc
     所以他们是临时对象，不用担心内存溢出，但是要记得初始化 tail 和 head 和 numelm

     左链表 tmp_left 和一个右链表 tmp_right 
     左链表保存所有  小于 pivot的节点
     右链表保存所有  大于 pivot的节点
     怎么判断 节点之间的大小呢 ，通过 comp 这个传进来的函数指针进行判断
     我们可以看到这个函数指针 接受两个 job_t指针 分别是 a和b，然后在comp内部对 a和b的内容进行判断返回大小

    */
    list_t tmp_left;
    list_t tmp_right;

    tmp_left.tail = 0;
    tmp_left.head = 0;
    tmp_left.numelm = 0;
    tmp_right.tail = 0;
    tmp_right.head = 0;
    tmp_right.numelm = 0;

    /*这里我们遍历链表G的每一个节点*/
    for (elmlist_t* e = G->head; e;) 
    {
        elmlist_t* cur = e; /*当前的链表节点cur*/
        e = e->next; /*下一个链表节点e*/
        take_out_from_list(G, cur); /*将当前的节点cur从G中拿出来*/
        if (comp(cur->jb, pivot->jb) < 0) /*判断cur和pivot哪个大，如果comp返回值小于0代表pivot大 否则 cur大*/
        {
            queue(&tmp_left, cur); /*cur小于pivot所以塞入左list*/
        }
        else
        {
            queue(&tmp_right, cur); /*cur大于pivot所以塞入右list*/
        }
    }

    quick_sort(&tmp_left, comp); /*快速排序左链表，即所有小于pivot的元素会被快速排序*/
    quick_sort(&tmp_right, comp); /*快速排序左链表，即所有大于pivot的元素会被快速排序*/

    /*做完上面两步后，我们可以知道，
       
        左链表已经被排序好了，而且左链表中所有的元素都小于 pivot
        而且右链表也已经被排序好了，而且右链表中所有的元素都大于 pivot
        这样我们将  这三个东西这样排序     
        
        tmp_left ------> pivot -------> tmp_right

        很容易发现上面的这仨就可以构成一个完整排序的新链表了
    */

    cons(G, pivot); /*将pivot塞回原链表G中*/


    /*将左链表的元素一个个从尾部拿出来，然后塞回 G的头部中构成  G =    tmp_left ------> pivot */
    while (tmp_left.numelm)
    {
        elmlist_t* t = tmp_left.tail;
        take_out_from_list(&tmp_left, t);
        cons(G, t);
    }


    /*将右链表的元素一个个从头部拿出来，然后塞回 G的尾部部中构成  G =    tmp_left ------> pivot  -------> tmp_right */
    while (tmp_right.numelm)
    {
        elmlist_t* t = tmp_right.head;
        take_out_from_list(&tmp_right, t);
        queue(G, t);
    }

    /*大功告成此时G已经完全排序好了*/
}

