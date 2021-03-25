#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LEVELS 1000
#define LIST_COUNTS 20

typedef struct __node {
    int value;
    struct __node *next;
	struct __node *prev;
} node_t;

static inline void list_add_node_t(node_t **list, node_t *node_t)
{
    node_t->next = *list;
    *list = node_t;
}

static inline void list_concat(node_t **left, node_t *right)
{
    if (!(*left)) {
	    *left = right;
	} else {
        while ((*left)->next) {
            left = &((*left)->next);
		}
		(*left)->next = right;
	}
}

void quicksort(node_t **list)
{
    if (!*list)
        return;

    node_t *pivot = *list;
    int value = pivot->value;
    node_t *p = pivot->next;
    pivot->next = NULL;

	node_t *left = NULL, *right = NULL;
    while (p) {
        node_t *n = p;
        p = p->next;
        list_add_node_t(n->value > value ? &right : &left, n);
    }

    quicksort(&left);
    quicksort(&right);

    node_t *result = NULL;
	list_concat(&result, left);
    list_concat(&result, pivot);
    list_concat(&result, right);
    *list = result;
}

void qsort_non_recursive(node_t **list)
{ 
    if (!(*list)) {
        return;
	}
	int i=0;
	int pivot;

	node_t **begin = (node_t **)malloc(MAX_LEVELS * sizeof(node_t));
	node_t **end = (node_t **)malloc(MAX_LEVELS * sizeof(node_t));
	for(int i=0; i<MAX_LEVELS; i++) {
	    begin[i] = (node_t *)malloc(1 * sizeof(node_t));
		end[i] = (node_t *)malloc(1 * sizeof(node_t));
	}
	node_t *L;
	node_t *R;
	begin[0] = *list; //head of list
	node_t *tmp = *list;
	while (tmp->next) {
	    tmp = tmp->next;
	}
	end[0] = tmp; // tail of list
	while (i >= 0) {
        L = begin[i]; // left node of each round
		R = end[i]; // right node of each round
		if (L != R && L && R) {
		    pivot = L->value;
			if (i == MAX_LEVELS) assert(i != MAX_LEVELS);
			while (L != R && L && R) {
			    while (R->value >= pivot && L != R) {
			        R = R->prev;
			    }
			    if (L != R) {
				    L->value = R->value;
			    	L = L->next;
			    }
		    	while (L->value <= pivot && L != R) {
			        L = L->next;
		    	}
		    	if (L != R) {
		    	    R->value = L->value;
			        R = R->prev;
		    	}
			}
			L->value = pivot;
			begin[i+1] = L->next;
			end[i+1] = end[i];
			end[i] = L;
			i++;
		} else {
		    i--;
		}
	}
}


static bool list_is_ordered(node_t *list)
{
    bool first = true;
    int value;
    while (list) {
        if (first) {
            value = list->value;
            first = false;
        } else {
            if (list->value < value)
                return false;
            value = list->value;
        }
        list = list->next;
    }
    return true;
}

static void list_display(node_t *list)
{
    printf("%s IN ORDER : ", list_is_ordered(list) ? "   " : "NOT");
    while (list) {
        printf("%d ", list->value);
        list = list->next;
    }
    printf("\n");
}

node_t *list_make_node_t(node_t *list, int value)
{
    node_t *n = malloc(1 * sizeof(node_t));
    if (!n) {
        assert(n && "malloc error");
        return NULL;
    }
    n->value = value;
    n->next = NULL;
	n->prev = NULL;
    if (!list) {
        return n;
    } else {
        node_t *tmp = list;
        while (tmp->next) {
            tmp = (tmp->next);
        }
        tmp->next = n;
		n->prev = tmp;
        return list;
    }
}

void list_free(node_t **list)
{
    if (!(*list)->next) {
        free(*list);
    } else {
        list_free(&((*list)->next));
    }
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    size_t count = LIST_COUNTS;
    node_t *list = NULL;
    while (count--) {
        list = list_make_node_t(list, rand() % 1024);    
    }
    list_display(list);
	//quicksort(&list);
	qsort_non_recursive(&list);
	list_display(list);
    
	if (!list_is_ordered(list))
        return EXIT_FAILURE;

    list_free(&list);
    return EXIT_SUCCESS;
}
