#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

static list_ele_t *merge_sort_list(list_ele_t *head);
/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* TODO: What if malloc returned NULL? */
    if (q) {
        q->head = NULL;
        q->tail = NULL;
        q->size = 0;
    }
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* TODO: How about freeing the list elements and the strings? */
    /* Free queue structure */
    if (q) {
        while (q->head != NULL) {
            list_ele_t *tmp = q->head;
            q->head = q->head->next;
            free(tmp->value);
            free(tmp);
        }
        free(q);
    }
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    /* TODO: What should you do if the q is NULL? */
    if (!q)
        return false;
    else {
        newh = malloc(sizeof(list_ele_t));
        /* Don't forget to allocate space for the string and copy it */
        /* What if either call to malloc returns NULL? */
        if (!newh)
            return false;
        int buff_size = strlen(s) + 1;
        newh->value = (char *) malloc(sizeof(char) * buff_size);
        if (!newh->value) {
            free(newh);
            return false;
        }
        memset(newh->value, '\0', buff_size);
        strncpy(newh->value, s, buff_size - 1);
        newh->next = q->head;
        q->head = newh;
        if (q->size == 0)
            q->tail = newh;
        ++q->size;
        return true;
    }
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    list_ele_t *newh;

    if (!q)
        return false;
    else {
        newh = malloc(sizeof(list_ele_t));
        if (!newh)
            return false;
        int buff_size = strlen(s) + 1;
        newh->value = malloc(sizeof(char) * buff_size);
        if (!newh->value) {
            free(newh);
            return false;
        }
        memset(newh->value, '\0', buff_size);
        strncpy(newh->value, s, buff_size - 1);
        newh->next = NULL;
        if (q->size == 0) {
            q->head = newh;
        } else {
            q->tail->next = newh;
        }
        q->tail = newh;
        ++q->size;
        return true;
    }
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || q->size == 0)
        return false;
    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    bool ret = false;
    if (sp) {
        memset(sp, '\0', bufsize);
        strncpy(sp, tmp->value, bufsize - 1);
        ret = true;
    }

    free(tmp->value);
    free(tmp);
    --q->size;
    return ret;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return q ? q->size : 0;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (q) {
        if (q->size == 2) {
            list_ele_t *tmp = q->head;
            q->tail->next = q->head;
            q->head->next = NULL;
            q->head = q->tail;
            q->tail = tmp;
        } else if (q->size > 2) {
            list_ele_t *tmp1, *tmp2, *tmp3;
            tmp1 = q->head;
            tmp2 = tmp1->next;
            tmp3 = tmp2->next;
            q->head->next = NULL;
            while (tmp3 != q->tail) {
                tmp2->next = tmp1;
                tmp1 = tmp2;
                tmp2 = tmp3;
                tmp3 = tmp3->next;
            }
            tmp2->next = tmp1;
            tmp3->next = tmp2;
            tmp1 = q->head;  // swap q->head and q->tail
            q->head = q->tail;
            q->tail = tmp1;
        }
    }
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */

list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    if (!l2)
        return l1;
    if (!l1)
        return l2;

    list_ele_t *head, *tmp;
    if (strcmp(l1->value, l2->value) <= 0) {
        head = l1;
        tmp = l1;
        l1 = l1->next;
    } else {
        head = l2;
        tmp = l2;
        l2 = l2->next;
    }

    while (l1 || l2) {
        if (l1 && l2) {
            if (strcmp(l1->value, l2->value) <= 0) {
                tmp->next = l1;
                l1 = l1->next;
            } else {
                tmp->next = l2;
                l2 = l2->next;
            }
            tmp = tmp->next;
        } else if (l1 && !l2) {
            tmp->next = l1;
            break;
        } else if (!l1 && l2) {
            tmp->next = l2;
            break;
        }
    }

    return head;
}


static list_ele_t *merge_sort_list(list_ele_t *head)
{
    if (!head || !head->next) {
        return head;
    }
    list_ele_t *fast = head->next;
    list_ele_t *slow = head;
    while (fast && fast->next) {
        fast = fast->next->next;
        slow = slow->next;
    }
    fast = slow->next;
    slow->next = NULL;

    list_ele_t *l1, *l2;
    l1 = merge_sort_list(head);
    l2 = merge_sort_list(fast);
    return merge(l1, l2);
}

void q_sort(queue_t *q)
{
    if (q && q->size > 1) {
        q->head = merge_sort_list(q->head);
        list_ele_t *tmp = q->head;
        while (tmp->next) {
            tmp = tmp->next;
        }
        q->tail = tmp;
    }
}
