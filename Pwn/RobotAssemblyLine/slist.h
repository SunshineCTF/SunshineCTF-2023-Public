#ifndef SLIST_H
#define SLIST_H

#include <stddef.h>
#include "common.h"

/*
 * Modified version of list.h for a singly linked list whose
 * list head includes pointers to both the front and back of
 * the list.
 */

struct slist_head {
	struct slist_node *first, *last;
};

struct slist_node {
	struct slist_node *next;
};

#define SLIST_HEAD_INIT(name) { NULL, NULL }

#define SLIST_HEAD(name) \
	struct slist_head name = SLIST_HEAD_INIT(name)

static inline void INIT_SLIST_HEAD(struct slist_head *slist)
{
	slist->first = NULL;
	slist->last = NULL;
}

static inline void __slist_insert(
	struct slist_node *new_node,
	struct slist_node **pcur)
{
	new_node->next = *pcur;
	*pcur = new_node;
}

static inline void slist_add(
	struct slist_node *new_node,
	struct slist_head *head)
{
	if (!head->last) {
		head->last = new_node;
	}
	__slist_insert(new_node, &head->first);
}

static inline void slist_add_tail(
	struct slist_node *new_node,
	struct slist_head *head)
{
	new_node->next = NULL;
	if (!head->first) {
		head->first = head->last = new_node;
	}
	else {
		head->last->next = new_node;
		head->last = new_node;
	}
}

static inline void __slist_del(
	struct slist_node **pcur,
	struct slist_node *next)
{
	*pcur = next;
}

static inline void slist_del(
	struct slist_node **pcur,
	struct slist_head *head)
{
	if (*pcur == head->last) {
		*pcur = NULL;
		head->last = NULL;
	}
	else {
		__slist_del(pcur, (*pcur)->next);
	}
}

static inline void slist_move(
	struct slist_node **pcur,
	struct slist_head *from_head,
	struct slist_head *to_head)
{
	struct slist_node *cur = *pcur;
	slist_del(pcur, from_head);
	slist_add(cur, to_head);
}

static inline void slist_move_tail(
	struct slist_node **pcur,
	struct slist_head *from_head,
	struct slist_head *to_head)
{
	struct slist_node *cur = *pcur;
	slist_del(pcur, from_head);
	slist_add_tail(cur, to_head);
}

static inline int slist_is_last(
	const struct slist_node *node)
{
	return node->next == NULL;
}

static inline int slist_empty(
	const struct slist_head *head)
{
	return head->first == NULL;
}

#define slist_entry(ptr, type, member) ({ \
	struct slist_node *__slptr = (ptr); \
	__slptr ? container_of(__slptr, type, member) : NULL; \
})

#define slist_first_entry(ptr, type, member) \
	slist_entry((ptr)->first, type, member)

#define slist_last_entry(ptr, type, member) \
	slist_entry((ptr)->last, type, member)

#define slist_next_entry(pos, member) \
	slist_entry((pos)->member.next, typeof(*(pos)), member)

#define slist_for_each(pos, head) \
	for (pos = (head)->first; pos != NULL; pos = pos->next)

#define slist_for_each_safe(pcur, pnext, head) \
	for ( \
		pcur = &(head)->first, pnext = *pcur ? &(*pcur)->next : NULL; \
		*pcur != NULL; \
		pcur = pnext, pnext = *pcur ? &(*pcur)->next : NULL)

#define slist_for_each_entry(pos, head, member) \
	for ( \
		pos = slist_first_entry(head, typeof(*pos), member); \
		pos != NULL; \
		pos = slist_next_entry(pos, member))

#endif /* SLIST_H */
