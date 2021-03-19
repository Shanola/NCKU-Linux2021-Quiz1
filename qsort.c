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
} node_t;

static inline void list_add_node_t(node_t **list, node_t *node_t)
{
    node_t->next = *list;
    *list = node_t;
}

static inline void list_concat(node_t **left, node_t *right)
{
    while (*left)
        left = &((*left)->next);
    *left = right;
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
    if (!*list) {
        return;
	}
	int arr[LIST_COUNTS];
	node_t *tmp = *list;
	for (int i=0; i<LIST_COUNTS; i++) {
	    arr[i] = tmp->value;
		tmp = tmp->next;
	}
	int i=0;
	int pivot, begin[MAX_LEVELS], end[MAX_LEVELS], L, R;
	begin[0] = 0;
	end[0] = LIST_COUNTS;
	while (i >= 0) {
        L = begin[i];
		R = end[i] - 1;
		if (L < R) {
		    pivot = arr[L];
			if (i == MAX_LEVELS) assert(i != MAX_LEVELS);
			while (L < R) {
			    while (arr[R] >= pivot && L < R) {
			        R--;
			    }
			    if (L < R) {
			        arr[L] = arr[R];
			    	L++;
			    }
		    	while (arr[L] <= pivot && L < R) {
			        L++;
		    	}
		    	if (L < R) {
		    	    arr[R] = arr[L];
		    		R--;
		    	}
			}
			arr[L] = pivot;
			begin[i+1] = L+1;
			end[i+1] = end[i];
			end[i] = L;
			i++;
		} else {
		    i--;
		}
	}
	tmp = *list;
	for (int i=0; i<LIST_COUNTS; i++) {
        // printf("%d ", arr[i]);
		tmp->value = arr[i];
		tmp = tmp->next;
	}
	// printf("\n");
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
    if (!list) {
        return n;
    } else {
        node_t *p = list;
        while (list->next) {
            list = (list->next);
        }
        list->next = n;
        return p;
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
    qsort_non_recursive(&list);
	// quicksort(&list);
    qsort_non_recursive(&list);
	list_display(list);

    if (!list_is_ordered(list))
        return EXIT_FAILURE;

    list_free(&list);
    return EXIT_SUCCESS;
}
