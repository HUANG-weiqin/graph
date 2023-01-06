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

list_t* creat_list() {
    list_t* res = malloc(sizeof(list_t));
    res->numelm = 0;
    res->head = 0;
    res->tail = 0;
    return res;
}

char* get_title(job_t * j) {
    return j->title;
}

void quick_sort(list_t *G,int (* comp)(job_t *a,job_t *b)){

}