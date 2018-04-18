#ifndef BK_DLIST_H
#define BK_DLIST_H

#include "macro.h"
#include "assert.h"

#define bk_dlist_foreach(itr, dlist) \
	for(bk_dlist_link_t* itr = (dlist)->next; itr != (dlist); itr = itr->next)

typedef struct bk_dlist_link_s bk_dlist_link_t;

struct bk_dlist_link_s
{
	bk_dlist_link_t* next;
	bk_dlist_link_t* prev;
};

BK_INLINE void
bk_dlist_init(bk_dlist_link_t* link)
{
	link->next = link->prev = link;
}

BK_INLINE int
bk_dlist_unlinked(bk_dlist_link_t* link)
{
	return (link->next == link && link->prev == link)
		|| (link->next == NULL && link->prev == NULL);
}

BK_INLINE void
bk_dlist_append(bk_dlist_link_t* list, bk_dlist_link_t* item)
{
	BK_ASSERT(list != item, "Can't link to itself");
	BK_ASSERT(bk_dlist_unlinked(item), "Item already linked");

	bk_dlist_link_t* next = list->next;
	item->prev = list;
	item->next = next;
	next->prev = item;
	list->next = item;
}

BK_INLINE void
bk_dlist_prepend(bk_dlist_link_t* list, bk_dlist_link_t* item)
{
	BK_ASSERT(list != item, "Can't link to itself");
	BK_ASSERT(bk_dlist_unlinked(item), "Item already linked");

	bk_dlist_link_t* prev = list->prev;
	item->prev = prev;
	item->next = list;
	prev->next = item;
	list->prev = item;
}

BK_INLINE void
bk_dlist_unlink(bk_dlist_link_t* item)
{
	BK_ASSERT(!bk_dlist_unlinked(item), "Item not linked");

	bk_dlist_link_t* next = item->next;
	bk_dlist_link_t* prev = item->prev;
	next->prev = prev;
	prev->next = next;

	bk_dlist_init(item);
}

BK_INLINE int
bk_dlist_is_empty(bk_dlist_link_t* list)
{
	return list->next == list;
}

#endif
