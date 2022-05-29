#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head) {
    /* TODO: Implement ll_has_cycle */
    if(head == NULL || head->next == NULL){
        return 0;
    }
    node *slow_ptr = head;
    node *fast_ptr = head->next;
    while (fast_ptr->next != NULL)
    {
        if(fast_ptr == slow_ptr){
            return 1;
        }
        if(fast_ptr->next->next == NULL)
            break;
        fast_ptr = fast_ptr->next->next;
        slow_ptr = slow_ptr->next;
    }
    return 0;
}