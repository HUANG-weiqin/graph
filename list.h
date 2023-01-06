#pragma once
typedef struct {
    char title[10] ; // Nom de la tache
    double life ; // Duree de la tache
    int input_degree ; // Son degre de dependance
    int output_degree ; // Les taches qui en dependent
    int rank ; // Rang de latache
    int dyn_input_degree ; // Facilite de prog
    struct list_t * precedence ; // Les taches precedentes
    struct list_t * posteriority ; // Les taches ulterieures
    double au_plus_tot ; // Date au plustot
    double au_plus_tard ; // Date au plustard
    double marge_totale ; // Marge totale
    double marge_libre ; // Marge libre
    int critique ; // Une tache critique
} job_t;



typedef struct element{
    struct element* prev;
    struct element* next;
    job_t* jb;
}elmlist_t;

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

list_t* creat_list() {
    list_t* res = malloc(sizeof(list_t));
    res->head = 0;
    res->tail = 0;
    res->numelm = 0;
    return res;
}

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

void quick_sort(list_t *G,int (* comp)(job_t *a,job_t *b)){
    if (G->numelm < 2)
        return;

    elmlist_t* pivot = G->head;
    take_out_from_list(G, pivot);

    list_t tmp_left;
    list_t tmp_right;
    tmp_left.tail = 0;
    tmp_left.head = 0;
    tmp_left.numelm = 0;
    tmp_right.tail = 0;
    tmp_right.head = 0;
    tmp_right.numelm = 0;

    for (elmlist_t* e = G->head; e;) 
    {
        elmlist_t* cur = e;
        e = e->next;
        take_out_from_list(G, cur);
        if (comp(cur->jb, pivot->jb) < 0)
        {
            queue(&tmp_left, cur);
        }
        else
        {
            queue(&tmp_right, cur);
        }
    }

    quick_sort(&tmp_left, comp);
    quick_sort(&tmp_right, comp);

    cons(G, pivot);

    while (tmp_left.numelm)
    {
        elmlist_t* t = tmp_left.tail;
        take_out_from_list(&tmp_left, t);
        cons(G, t);
    }

    while (tmp_right.numelm)
    {
        elmlist_t* t = tmp_right.head;
        take_out_from_list(&tmp_right, t);
        queue(G, t);
    }
}

