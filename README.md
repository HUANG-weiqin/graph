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


