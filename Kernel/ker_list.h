#ifndef  __KER_LIST_H
#define  __KER_LIST_H


#ifdef __cplusplus
extern "C" {
#endif
  
struct sl_list_node
{
    struct sl_list_node *next;                          /**< point to next node. */
    struct sl_list_node *prev;                          /**< point to prev node. */
};
typedef struct sl_list_node sl_list_t;                  /**< Type for lists. */

  
#define Sl_inline                   static __inline

/**
 * @addtogroup KernelService
 */

/*@{*/

/**
 * @brief initialize a list object
 */
#define SL_LIST_OBJECT_INIT(object) { &(object), &(object) }

/**
 * @brief initialize a list
 *
 * @param l list to be initialized
 */
Sl_inline void sl_list_init(sl_list_t *l)
{
    l->next = l->prev = l;
}

/**
 * @brief insert a node after a list
 *
 * @param l list to insert it
 * @param n new node to be inserted
 */
Sl_inline void sl_list_insert_after(sl_list_t *l, sl_list_t *n)
{
    l->next->prev = n;
    n->next = l->next;

    l->next = n;
    n->prev = l;
}

/**
 * @brief insert a node before a list
 *
 * @param n new node to be inserted
 * @param l list to insert it
 */
Sl_inline void sl_list_insert_before(sl_list_t *l, sl_list_t *n)
{
    l->prev->next = n;
    n->prev = l->prev;

    l->prev = n;
    n->next = l;
}

/**
 * @brief remove node from list.
 * @param n the node to remove from the list.
 */
Sl_inline void sl_list_remove(sl_list_t *n)
{
    n->next->prev = n->prev;
    n->prev->next = n->next;

    n->next = n->prev = n;
}

/**
 * @brief tests whether a list is empty
 * @param l the list to test.
 */
Sl_inline int sl_list_isempty(const sl_list_t *l)
{
    return l->next == l;
}


#if 0
/**
 * @brief get the struct for this entry
 * @param node the entry point
 * @param type the type of structure
 * @param member the name of list in structure
 */
#define sl_list_entry(node, type, member) \
    ((type *)((char *)(node) - (unsigned long)(&((type *)0)->member)))

/**
 * list_for_each_entry  -   iterate over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, head, member)              \
    for (pos = sl_list_entry((head)->next, typeof(*pos), member);  \
         &pos->member != (head);    \
         pos = sl_list_entry(pos->member.next, typeof(*pos), member))

/**
 * list_first_entry - get the first element from a list
 * @ptr:    the list head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define list_first_entry(ptr, type, member) \
    sl_list_entry((ptr)->next, type, member)
/*@}*/


#endif

#ifdef __cplusplus
}
#endif



#endif /* __KER_LIST_H */
