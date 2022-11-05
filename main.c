#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* List declaration */
typedef struct node {
    int timestamp;
    double value;
    struct node *next;
    struct node *prev;
    int to_delete;
} node;

typedef struct TDoubleLinkedList {
    struct node *head;
    struct node *tail;
    int PairNumbers;
} Tlist;

/* Function to initialize list */
void init(Tlist **list) {
    (*list) = (Tlist *) malloc (sizeof(Tlist));
    (*list)->head = (*list)->tail = NULL;
    (*list)->PairNumbers = 0;
}

/* Function for adding elements to the list */
void add(Tlist *list, int n, int time, double val) {
    node *new;
    new = (node*)malloc(sizeof(node));
    new->timestamp = time;
    new->value = val;
    new->to_delete = 0;
    int i;

    /* when the list is empty */
    if(list->head == NULL) {
        list->head = new;
        list->tail = new;
        new->next = NULL;
        new->prev = NULL;
        list->PairNumbers = 1;
    }
    /* when we add to the beginning of the list */
    else if (n == 0) {
        new->next = list->head;
        new->prev = NULL;
        list->head->prev = new;
        list->head = new;
        (list->PairNumbers)++;
    }
    /* when we add at the end of the list */
    else if (n == list->PairNumbers) {
        new->prev = list->tail;
        new->next = NULL;
        list->tail->next = new;
        list->tail = new;
        (list->PairNumbers)++;
    }
    /* when we add in the interior of the list */
    else {
        node *current = list->head;
        for(i = 1; i <= n-1 ; i++) {
            current = current->next;
        }
        new->prev = current;
        new->next = current->next;
        current->next->prev = new;
        current->next = new;
        (list->PairNumbers)++;
    }
}

void print(Tlist *list) {
    node *ajutor = list->head;
    if(list->head == NULL) {
        return;
    }
    while(ajutor != NULL) {
        printf("%d %.2lf\n", ajutor->timestamp, ajutor->value);
        ajutor = ajutor->next;
    }
}

void free_list(Tlist **list) {
    node *ajutor;
    while((*list)->head != NULL) {
        ajutor = (*list)->head;
        (*list)->head = (*list)->head->next;
        free(ajutor);
    }
    (*list)->tail = NULL;
    free(*list);
}

void delete_node(Tlist *list, int n) {
    /* Deleting first node */
    int i;
    if(n == 0) {
        node *ajutor;
        ajutor = list->head;
        ajutor->next->prev = NULL;
        list->head = ajutor->next;
        free(ajutor);
        (list->PairNumbers)--;
    }
    /* Deleting last node */
    else if(n == list->PairNumbers - 1) {
        node *ajutor;
        ajutor = list->tail;
        ajutor->prev->next = NULL;
        list->tail = ajutor->prev;
        free(ajutor);
        (list->PairNumbers)--;
    }
    /* Delete node from the interior of the list */
    else {
        node *ajutor;
        ajutor = list->head;
        for(i = 0; i < n; i++) {
            ajutor = ajutor->next;
        }
        ajutor->prev->next = ajutor->next;
        ajutor->next->prev = ajutor->prev;
        free(ajutor);
        (list->PairNumbers)--;
    }
}

/* Function for eliminating exceptions using statistics */
void e1(Tlist *list) {
    int k, z, i;
    double s1, s2, media, dev;
    node *ajutor;
    for(i = 1; i <= list->PairNumbers - 5; i++) {
        ajutor = list->head;
        for (z = 0; z < i; z++) {
            ajutor = ajutor->next;
        }

        s1 = 0;
        for (k = 0; k < 5; k++) {

            s1 = ajutor->value + s1;
            ajutor = ajutor->next;
        }
        media = s1 / 5;

        ajutor = list->head;
        for (z = 0; z < i; z++) {
            ajutor = ajutor->next;
        }

        s2 = 0;
        for (k = 0; k < 5; k++) {
            s2 = pow(ajutor->value - media, 2) + s2;
            ajutor = ajutor->next;
        }
        dev = sqrt(s2 / 5);

        ajutor = list->head;
        for (z = 0; z < i + 2; z++) {
            ajutor = ajutor->next;
        }

        if(ajutor->value < (media - dev) || ajutor->value > (media + dev)) {
            ajutor->to_delete = 1;
        }
    }
    int poz = 0;
    ajutor = list->head;
    while(ajutor != NULL) {
        if(ajutor->to_delete == 1) {
            ajutor = ajutor->next;
            delete_node(list, poz);
            continue;
        }
        ajutor = ajutor->next;
        poz++;
    }
}

/* Function to sort the elements of the list */
void sort_list(Tlist *list) {
    struct node *current = list->head, *index = NULL;
    double temp_value;

    for(current = list->head; current->next != NULL; current = current->next)
        for(index = current->next; index != NULL; index = index->next)
            if (current->value > index->value) {
                temp_value = current->value;
                current->value = index->value;
                index->value = temp_value;
            }
}

/* Function for eliminating noise using median filter */
void e2(Tlist *list, Tlist *list2) {
    int poz = 0, i;
    node *ajutor = list->head, *current;
    Tlist *helper;
    for(i = 1; i <= list->PairNumbers - 4; i++) {
        init(&helper);
        add(helper, 0, ajutor->timestamp, ajutor->value);
        add(helper, 1, ajutor->next->timestamp, ajutor->next->value);
        add(helper, 2, ajutor->next->next->timestamp, ajutor->next->next->value);
        add(helper, 3, ajutor->next->next->next->timestamp, ajutor->next->next->next->value);
        add(helper, 4, ajutor->next->next->next->next->timestamp, ajutor->next->next->next->next->value);
        sort_list(helper);
        current = helper->head;
        add(list2, poz, current->next->next->timestamp, current->next->next->value);
        free_list(&helper);
        poz++;
        ajutor = ajutor->next;
        }
}

/* Function for calculating the average of sublists of 5 elements */
double list_average(Tlist *list) {
    node *current = list->head;
    double s = 0;
    int k;
    for(k = 0; k < 5; k++) {
        s = s + current->value;
        current = current->next;
    }

    return s / 5;
}

/* Function for eliminating noise using average based filter */
void e3(Tlist *list, Tlist *list3) {
    int poz = 0, i;
    node *ajutor = list->head, *current;
    double average;
    Tlist *helper;
    for(i = 1; i <= list->PairNumbers - 4; i++) {
        init(&helper);
        add(helper, 0, ajutor->timestamp, ajutor->value);
        add(helper, 1, ajutor->next->timestamp, ajutor->next->value);
        add(helper, 2, ajutor->next->next->timestamp, ajutor->next->next->value);
        add(helper, 3, ajutor->next->next->next->timestamp, ajutor->next->next->next->value);
        add(helper, 4, ajutor->next->next->next->next->timestamp, ajutor->next->next->next->next->value);
        average = list_average(helper);
        current = helper->head;
        add(list3, poz, current->next->next->timestamp, average);
        free_list(&helper);
        poz++;
        ajutor = ajutor->next;
    }
}

/* Function for uniformizing the frequency */
void u(Tlist *list) {
    node *current = list->head->next;
    int i;
    for(i = 2; i <= list->PairNumbers; i++) {
        if((current->timestamp - current->prev->timestamp) >= 100 && (current->timestamp - current->prev->timestamp) <= 1000) {
            current->timestamp = (current->prev->timestamp + current->timestamp) / 2;
            current->value = (current->prev->value + current->value) / 2;
        }
        current = current->next;
    }
}

/* Function for calculating statistics */
void statistici(Tlist *list, int omg) {
    sort_list(list);
    node *current;
    int margine_stanga, margine_dreapta, contor, rest;
    double minim, maxim;
    minim = list->head->value;
    maxim = list->tail->value;
    rest = (int)(minim) % omg;
    margine_stanga = (int)(minim) - rest;
    if(minim < 0) {
        margine_stanga -= omg;
    }
    margine_dreapta =  margine_stanga + omg;
    while(margine_dreapta <= (int)(maxim) + omg) {
        contor = 0;
        for(current = list->head; current != NULL; current = current->next) {
            if(current->value >= margine_stanga && current->value <= margine_dreapta) {
                contor++;
            }
        }
        if(contor != 0) {
           printf("[%d, %d] %d\n", margine_stanga, margine_dreapta, contor);
        }
        margine_stanga += omg;
        margine_dreapta += omg;
    }
}

int main(int argc, char** argv) {
    Tlist *list, *list2, *list3;
    int numar_perechi, timp, omega, i;
    double valoare;
    init(&list);
    init(&list2);
    init(&list3);
    scanf("%d", &numar_perechi);

    for (i = 0; i < numar_perechi; i++) {
        scanf("%d%lf", &timp, &valoare);
        add(list, i, timp, valoare);
    }

    if(strcmp(argv[1], "--e1") == 0 && argc == 2) {
        e1(list);
    }
    else if(strcmp(argv[1], "--e2") == 0 && argc == 2) {
        e2(list, list2);
    }
    else if(strcmp(argv[1], "--e3") == 0 && argc == 2) {
        e3(list, list3);
    }
    else if(strcmp(argv[1], "--u") == 0 && argc == 2) {
        u(list);
    }
    else if (strcmp(argv[1], "--e1") == 0 && strcmp(argv[2], "--e2") == 0 && argc == 3) {
        e1(list);
        e2(list, list2);
    }
    else if (strcmp(argv[1], "--e1") == 0 && strcmp(argv[2], "--e3") == 0 && argc == 3 ) {
        e1(list);
        e3(list, list3);
    }
    else if(strcmp(argv[1], "--e1") == 0 && strcmp(argv[2], "--e2") == 0 && strcmp(argv[3], "--e3") == 0 && argc == 4) {
        e1(list);
        e2(list, list2);
        e3(list2, list3);
    }
    else if(strcmp(argv[1], "--e2") == 0 && strcmp(argv[2], "--u") == 0 && strcmp(argv[3], "--u") == 0 && argc == 4 ) {
        e2(list, list2);
        u(list2);
        u(list2);
    }
    else if(strcmp(argv[1], "--e3") == 0 && strcmp(argv[2], "--u") == 0 && strcmp(argv[3], "--u") == 0 && argc == 4 ) {
        e3(list, list3);
        u(list3);
        u(list3);
    }
    else if(strcmp(argv[1], "--e2") == 0 && strcmp(argv[2], "--e3") == 0 && strcmp(argv[3], "--e1") == 0 && strcmp(argv[4], "--u") == 0 && argc == 5) {
        e2(list, list2);
        e3(list2, list3);
        e1(list3);
        u(list3);
    }
    else if(strstr(argv[1], "st") != 0 && argc == 2) {
        omega = atoi(argv[1] + 4);
        statistici(list, omega);
    }

    if(strcmp(argv[1], "--e1") == 0 && argc == 2) {
        printf("%d\n", list->PairNumbers);
        print(list);
    }
    else if(strcmp(argv[1], "--e2") == 0 && argc == 2) {
        printf("%d\n", list2->PairNumbers);
        print(list2);
    }
    else if(strcmp(argv[1], "--e3") == 0 && argc == 2) {
        printf("%d\n", list3->PairNumbers);
        print(list3);
    }
    else if(strcmp(argv[1], "--u") == 0 && argc == 2) {
        printf("%d\n", list->PairNumbers);
        print(list);
    }
    else if (strcmp(argv[1], "--e1") == 0 && strcmp(argv[2], "--e2") == 0 && argc == 3) {
        printf("%d\n", list2->PairNumbers);
        print(list2);
    }
    else if (strcmp(argv[1], "--e1") == 0 && strcmp(argv[2], "--e3") == 0 && argc == 3 ) {
        printf("%d\n", list3->PairNumbers);
        print(list3);
    }
    else if(strcmp(argv[1], "--e1") == 0 && strcmp(argv[2], "--e2") == 0 && strcmp(argv[3], "--e3") == 0 && argc == 4) {
        printf("%d\n", list3->PairNumbers);
        print(list3);
    }
    else if(strcmp(argv[1], "--e2") == 0 && strcmp(argv[2], "--u") == 0 && strcmp(argv[3], "--u") == 0 && argc == 4 ) {
        printf("%d\n", list2->PairNumbers);
        print(list2);
    }
    else if(strcmp(argv[1], "--e3") == 0 && strcmp(argv[2], "--u") == 0 && strcmp(argv[3], "--u") == 0 && argc == 4 ) {
        printf("%d\n", list3->PairNumbers);
        print(list3);
    }
    else if(strcmp(argv[1], "--e2") == 0 && strcmp(argv[2], "--e3") == 0 && strcmp(argv[3], "--e1") == 0 && strcmp(argv[4], "--u") == 0 && argc == 5) {
        printf("%d\n", list3->PairNumbers);
        print(list3);
    }

    free_list(&list);
    free_list(&list2);
    free_list(&list3);
    return 0;
}