#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "io.h"
#include "rank.h"
int main(int argc, char ** argv){
    if(argc < 2) exit(-1);
    struct list_t * G = read_graph(argv[1]);
    printf("Liste des taches lue\n");
    view_list(G, &view_job);
    quick_sort(G, &iDegreeJobCmp);
    printf("Liste des taches triee par degree d'entree croissant\n");
    view_list(G,&view_job);
    ranking(G);
    printf("Liste des taches triee par rang croissant\n");
    view_list(G,&view_job);
    printf("Prune edges\n");
    prune(G);
    view_list(G,&view_job);
    printf("\nMarges totales des taches\n");
    marges_totales(G);
    view_list(G,&view_job);
    return 0;
}