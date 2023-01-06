struct job_t {
    char * title ; // Nom de la tache
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
    bool critique ; // Une tache critique
};


struct elmlist_t {
    struct elmlist_t * prev ;
    struct elmlist_t * next ;
    job_t * job ;
}

struct list_t {
    struct elmlist_t * head ;
    struct elmlist_t * tail ;
    int numelm ;
};

void view_list(list_t *G,void (* view_job)(job_t *a)){

}

void quick_sort(list_t *G,bool (* comp)(job_t *a,job_t *b)){

}