// set.h

// Standart preamble for header file
#ifndef _SET_H_
#define _SET_H_

#include <stddef.h> // for size_t and NULL
#include <stdbool.h> // for set_contains() and set_insert()

// Error-code for invalid situations
static const size_t INVALID = ~((size_t)0);


/*
 * Create new set.
 * Memory demanded by each item is *item_size*;
 * Using function *hash* for handling items;
 * Using *equals* function for testing whether items are equal.
 * Return pointer to the new set. Return NULL if parameters are not valid or memory wasn't allocated.
 */
void* set_create(size_t item_size, size_t(*hash)(const void*), bool (*equals)(const void*, const void*));

/*
 * Completely destroy existing *set*.
 * If function *destroy* is defined then call it for each deleting item.
 * If *set* is NULL do nothing.
 */
void set_destroy(void* set, void (*destroy)(void*));

/*
 * Initialize the *set*.
 * If set contains any items then first of all delete all items, after what initialize the set with new parameters.
 * Memory demanded by each item is *item_size*;
 * Using new function *hash* for handling items;
 * Using new function *equals* for testing whether items are equal.
 * If function *destroy* is defined then call it for each deleting item.
 * Return the same pointer *set* if set was initializad successfully.
 * If parameters are not valid then *set* retain the same and NULL will be returned.
 */ 
void* set_init(void* set, size_t item_size, size_t hash(const void*), bool (*equals)(const void*, const void*), void (*destroy)(void*));

/*
 * Delete all items in the *set*.
 * If function *destroy* is defined then call it for each deleting item.
 * If *set* is NULL do nothing.
 */
void set_clear(void* set, void (*destroy)(void*));

/*
 * Return amount of items in the *set*.
 * If *set* is NULL - return INVALID.
 */
size_t set_count(const void* set);

/*
 * Check whether the *set* contains the element *item*.
 * If *set* or *item* is NULL also return false.
 */
bool set_contains(const void* set, const void* item);

/*
 * Add new element *item* into the *set*.
 * If *item* is new - allocate memory, insert the *item* and return true.
 * Otherwise (*item* already exists / *item* or *set* is NULL) - return false.
 * If memory wasn't allocated - return false.
 */
bool set_insert(void* set, const void* item);

/*
 * Find element *item* in the *set* and remove it.
 * If function *destroy* is defined then call it for the *item*.
 * If *set* or *item* is NULL then do nothing.
 */
void set_remove(void* set, const void* item, void (*destroy)(void*));

/*
 * Id for the first item in the *set*.
 * Return set_stop(*set*) if *set* is NULL or *set* is empty.
 */
size_t set_first(const void* set);

/*
 * Id for the last item in the *set*.
 * Return set_stop(*set*) if the *set* is NULL or *set* is empty.
 */
size_t set_last(const void* set);

/*
 * Get next identifier after current *item_id*.
 * Return set_stop(*set*) if the *set* is NULL / the item with the *item_id* is the last in the set / *item_id* == set_stop(*set*) or INVALID.
 * There is no check that item with such id exists so programm will crash if *item_id* does not exist in the set.
 */
size_t set_next(const void* set, size_t item_id);

/*
 * Get previous identifier after current *item_id*.
 * Return set_stop(*set*) if the *set* is NULL / the item with the *item_id* is the first in the set / *item_id* == set_stop(*set*) or INVALID.
 * There is no check that item with such id exists so programm will crash if *item_id* does not exist in the set.
 */
size_t set_prev(const void* set, size_t item_id);

/*
 * Id gotten in case of attempting to get beyond set.
 */
size_t set_stop(const void* set);

/*
 * Get pointer to item in *set* by its identifier *item_id*.
 * Return NULL if the *set* is NULL or *item_id* == set_stop(*set*) or INVALID.
 * There is no check that item with such id exists so programm will crash if *item_id* does not exist in the set. 
 */
const void* set_current(const void* set, size_t item_id);

/*
 * Deleting of item of the *set* by its *item_id*.
 * If function *destroy* is defined then call it for the item.
 * If *set* is NULL or *item_id* == set_stop(*set*) or INVALID then do nothing.
 * There is no check that item with such id exists so programm will crash if *item_id* does not exist in the set.
 */
void set_erase(void* set, size_t item_id, void (*destroy)(void*));


#endif
