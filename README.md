# graph

Explication du tri rapide:

    fonction :  void quick_sort(list_t *G,int (* comp)(job_t *a,job_t *b))

    Le déroulement du tri rapide est le suivant:

    1) tout d'abord, nous prenons un élément aléatoire de la liste G et comme "pivot".
        (Par commodité, j'ai choisi l'élément de tête de la list G comme pivot.)

    2) Nous retirons le pivot de G et comparons le reste des éléments de G avec le pivot utilisant le *comp
       - Pour tous les éléments plus petits que le pivot, nous formons une nouvelle liste appelée liste gauche.
       - Pour tous les éléments plus grandes que le pivot, nous formons une nouvelle liste appelée liste droite.

    3) Nous appelons récursivement la méthode "quick_sort" pour trier "liste gauche" et "liste droite"

    4) Maintenant nous savons que les deux listes ont été bien triées.
       Le list gauche est déjà trié et tous les éléments du list gauche sont plus petits que le pivot
       et le list droite est également trié et tous les éléments du list droite sont supérieurs au pivot
       Donc nous formons un nouvelle list contient ces trois choses comme ceci     

            list_left --> pivot --> list_right

       Il est facile de voir que les trois éléments ci-dessus vont créer une nouvelle list complètement triée.
   


Explication du void ranking(list_t* G):

    Le rank est calculé comme suit

    1- Un job sans prédécesseur a un rank de 0

    2- Le rank d'un job avec un prédécesseur est le plus grand rank de tous les job prédécesseurs + 1.



Explication du void prune(list_t* G):

  Si nous avons deux job, un pour j1 et un pour j2, et j2 est job précédent de j1
  j1 a un rank de 5
  j2 a un rank de 3

  alors sûrement j1 a un autre job précédent j3, et le rank de j3 est 5-1 = 4
  Cela signifie que le fait que j2 soit une tâche précédente de j1 n'a pas d'importance, car j1 ne peut pas commencer tant que j3 n'est pas terminée.
  j2 doit avoir terminé avant j3 parce que son rank est inférieur à celui de j3, donc j1 n'a rien à voir avec le moment où j2 termine.
  Dans ce cas, peu importe que j2 soit une pré-tâche de j1, nous pouvons retirer j2 de la liste des pré-tâches de j1 et également retirer
  j1 de la liste des post-tâches de j2

  En d'autres termes, pour toute paire de jobs, si la différence de rank entre eux est supérieure ou égale à 2, alors leur relation de préséance peut être supprimé.
  C'est ce que fait le prune, il supprime toutes les relations précédentes et suivantes qui peuvent être supprimées avec la différence de rank entre eux supérieure ou   égale à 2.
  
 

Explication du void marges_totales(list_t* G):

  1-Tout d'abord, nous devons ajouter deux tâches virtuelles, alpha et omega.

    alpha comme tâche de départ virtuelle, pour toutes les tâches avec un rang de 0 nous faisons de alpha leur tâche prédécesseur

    omega comme tâche finale virtuelle, pour toutes les tâches qui n'ont pas de post-tâches, nous faisons de omega leur post-tâche.
    
  2-Nous avons ensuite calculé la date de début au plus tôt et la date de début au plus tard selon la méthode utilisée sur le PPT.
  
  3-Enfin, pour tout job, sa marge totale = date de début au plus tard - date de début au plus tôt.

