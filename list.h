#pragma once
typedef struct {
    char title[10] ; // Nom de la tache
    int life ; // Duree de la tache
    int input_degree ; // Son degre de dependance
    int output_degree ; // Les taches qui en dependent
    int rank ; // Rang de latache
    int dyn_input_degree ; // Facilite de prog
    struct list_t * precedence ; // Les taches precedentes
    struct list_t * posteriority ; // Les taches ulterieures
    int au_plus_tot ; // Date au plustot
    int au_plus_tard ; // Date au plustard
    int marge_totale ; // Marge totale
    int marge_libre ; // Marge libre
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

void view_list(list_t *G,void (* view_job)(job_t *a)){

}

void quick_sort(list_t *G,int (* comp)(job_t *a,job_t *b)){

}