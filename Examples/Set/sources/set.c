// set.c

#include "set.h" // for functions declaration
#include <stdlib.h> // for malloc, calloc and free
#include <string.h> // for memcpy
#include <limits.h> // for ULLONG_MAX


// The longest sequence for items with the same hash, exceeding which will induce table resizing
static const size_t LONGEST_AVAILABLE_COLLISION = 100;

// Initial amount of rows in the set's table
static const size_t INITIAL_TABLE_SIZE = 100;


/*
 * Node that contains item and some information about its "surrounding".
 * *item* is stored item in the node.
 * *collision* pointer is using to create linked lists of collisions for items with the same hash.
 * *prev* is the pointer to the node which was inserted in the set right before current.
 * *next* is the pointer to the node which was inserted in the set right after current.
 * *prev* and *next* are using for fast iteration through the set with time complexity about O(1). Item's id is the address of its SetNode in the memory.
 * If node is first in the set then *prev* = NULL; if node is last in the set then *next* = NULL.
 */
typedef struct SetNode
{
	void* item;
	struct SetNode* collision;
	struct SetNode* prev;
	struct SetNode* next;
} SetNode;

/*
 * The set of elements. It is guaranteed that all items in the set are unique. Set based on hash table.
 * *table_size* is the number of rows (collisions lists of items with the same hash) the set contains.
 * *nodes_counter* is amount of items stored in the set.
 * Each item demands *item_size* memory allocated dynamically.
 * *rows* is dynamic array of pointers to the linked lists of collisions. In each row items with the same hash are located.
 * If there is no items with the hash=i then rows[i]=NULL.
 * *first_node* and *last_node* are using to iteration through the set. The item's id is the address of its node in the memory.
 * *first_node* is the pointer to the first node was inserted in the set.
 * *last_node* is the pointer to the last node was inserted in the set.
 * *first_node* and *last_node* are updating after deleting nodes.
 * *first_node* and *last_node* are NULL if the set is empty.
 * For handling items the *hash* function is using to calculate hash by the item and *equals* function for direct comparing items.
 * *hash* calculates the hash of the item and might return any number from (0, LLONG_MAX). But since we have only *table_size* rows in the set, 
 * the division by module *table_size* is applying for calculated hash and derived number represents the index of the row we must insert node to.
 */
typedef struct Set
{
    size_t table_size;
    size_t nodes_counter;
    size_t item_size;
    SetNode** rows;
    SetNode* first_node;
    SetNode* last_node;
    size_t(*hash)(const void*);
    bool (*equals)(const void*, const void*);
} Set;

/*
 * Apply the *set->hash* function to the *item* and divide it by module *set->table_size*
 */
static size_t bounded_hash(const Set* set, const void* item);

/*
 * Create new node.
 * Allocate memory for *item* according to the *item_size*.
 * Copy *item*.
 * Return pointer to the new node.
 * If memory wasn't allocated - return NULL.
 * Function does not update identifiers in the set.
 */
static SetNode* row_create(const size_t item_size, const void* item);

/*
 * Completely destroy linked list *row* of collisions in the set.
 * If *destroy* function is defined then apply it for each item (in case of item containing pointers to dynamic data).
 * Free memory allocated dynamically for the item.
 * Free memory allocated dynamically for the SetNode.
 * If *row* is NULL - do nothing.
 * Return NULL.
 */
static void* row_destroy(SetNode* row, void(*destroy)(void*));

/*
 * Calculate the length of the collisions list.
 * If *row* is NULL - return 0.
 */
static size_t row_count(const SetNode* row);

/*
 * Find *item* in the collisions list *row*.
 * *equals* function from Set is applied for comparing items.
 * If *row* is NULL - return false.
 * If *item* does not exist - return false.
 * Otherwise return true.
 */
static bool row_contains(const SetNode* row, const void* item, bool (*equals)(const void*, const void*));

/*
 * Insert *item* into existing collisions list with the index *row_index* from set *set*.
 * If *item* already exists return NULL pointer.
 * If *item* is new then create new node for it, allocate demanded memory for *item*, copy *item* and return pointer to the new node.
 * If row length is equal to LONGEST_AVAILABLE_COLLISION or greater - insert the *item* and increase the table two times.
 * Function does not update identifiers in the set.
 */
static SetNode* row_insert(Set* set, const size_t row_index, const void* item);

/*
 * Update identifiers in the *set* after inserting.
 * Update set->first_node (perhaps we insert the first node in the set), set->last_node, previous_last_node->next_node, item->prev_node.
 */
static void update_id_insert(Set* set, SetNode* item_node);

/*
 * Unlink node with the *item* from other nodes, but don't destroy it.
 * Function does not change identifiers.
 * Return pointer to the node. If *item* does not exist in the set - return NULL.
 */
static SetNode* set_unbind(Set* set, const size_t row_index, const void* item);

/*
 * Update identifiers in the *set* after removing (just detached yet) *item_node*.
 * If item_node is NULL do nothing.
 * Otherwise update set->first_node (perhaps we remove the single node in the set), set->last_node (perhaps we remove the last node), prev_node->next_node, next_node->prev_node.
 */
static void update_id_remove(Set* set, SetNode* item_node);

/*
 * Increase table in the *set* two times.
 * Not copying items. Just allocate two times more pointers in the set->rows, recalculate hashes and reassigned links.
 * It is not guaranteed that new rows exceeding max acceptable length won't emerge.
 * If set->table_size > max_size_t / 2 then don't change the size.
 */
static void table_increase(Set* set);

/*
 * Reassign the pointers of collisions lists from *old_rows* to *new_rows* using *hash* function.
 */
static void fill_rows(SetNode** new_rows, SetNode** old_rows, const size_t new_size, const size_t old_size, size_t(*hash)(const void*));

static size_t set_edge_node(const Set* set, const bool first_edge);

static size_t set_one_step(const Set* set, const size_t item_id, const bool step_next);


/*
* Create new set.
* Memory demanded by each item is *item_size*;
* Using function *hash* for handling items;
* Using *equals* function for testing whether items are equal.
* Return pointer to the new set. Return NULL if parameters are not valid or memory wasn't allocated.
*/
void* set_create(size_t item_size, size_t(*hash)(const void*), bool(*equals)(const void*, const void*))
{
    if (item_size == 0 || hash == NULL || equals == NULL)
    {
        return NULL;
    }

    // If all parameters are valid then allocate memory for new set
    Set* new_set = calloc(1, sizeof(Set));
    if (new_set == NULL)
    {
        return NULL;
    }

    // Memory was allocated
    new_set->table_size = INITIAL_TABLE_SIZE; // initial amount of available rows in the set
    new_set->rows = calloc(INITIAL_TABLE_SIZE, sizeof(SetNode*)); // create INITIAL_TABLE_SIZE NULL-pointers which will point to the lists of collisions later
    if (new_set->rows != NULL)
    {
        // Memory for pointers was allocated, set other fields
        new_set = set_init(new_set, item_size, hash, equals, NULL);
    }
    else
    {
        // Memory for pointers wasn't allocated, free the new_set and return NULL
        free(new_set);
        new_set = NULL;
    }

    return new_set;
}

void set_destroy(void* set, void (*destroy)(void*))
{
    if (set != NULL)
    {
        Set* set_explicit = set;
        // Clear all nodes
        set_clear(set_explicit, destroy);
        // Free memory allocated for pointers to collisions lists
        free(set_explicit->rows);
        // Free memory allocated for set
        free(set_explicit);
    }
}

void* set_init(void* set, size_t item_size, size_t hash(const void*), bool (*equals)(const void*, const void*), void (*destroy)(void*))
{
    Set* set_explicit = set;
    if (set_explicit == NULL || hash == NULL || equals == NULL || item_size == 0)
    {
        return NULL;
    }
    
    // All parameters are valid
    set_clear(set_explicit, destroy); // clear former data if it is exists; set all collisions lists pointers to the NULL; set first_node and last_node to the NULL; retain table_size
    set_explicit->item_size = item_size;
    set_explicit->hash = hash;
    set_explicit->equals = equals;
    return set_explicit;
}

void set_clear(void* set, void (*destroy)(void*))
{
    if (set != NULL)
    {
        Set* set_explicit = set;
        // Destroy each collisions list and set the pointers to the NULL
        for (size_t row_index = 0; row_index < set_explicit->table_size; ++row_index)
        {
            set_explicit->rows[row_index] = row_destroy(set_explicit->rows[row_index], destroy);
        }
        // Set first_node and last_node pointers to the NULL
        set_explicit->first_node = NULL;
        set_explicit->last_node = NULL;
        set_explicit->nodes_counter = 0;
    }
}

size_t set_count(const void* set)
{
    return set == NULL ? INVALID : ((const Set*)set)->nodes_counter;
}

bool set_contains(const void* set, const void* item)
{
    if (set == NULL || item == NULL)
    {
        return false;
    }

    const Set* set_explicit = set;
    // Find the row where item could exist
    const SetNode* appropriate_row = set_explicit->rows[bounded_hash(set_explicit, item)];
    // Find the *item* in the list
    bool contains = contains = row_contains(appropriate_row, item, set_explicit->equals);
    return contains;
}

bool set_insert(void* set, const void* item)
{
    if (set == NULL || item == NULL)
    {
        return false;
    }

    // All parameters are valid
    Set* set_explicit = set;
    // Find the row where item could exist
    const size_t row_index = bounded_hash(set_explicit, item);
    SetNode* item_node = row_insert(set_explicit, row_index, item);
    // Check insertion
    const bool inserted = item_node != NULL;
    if (inserted)
    {
        update_id_insert(set_explicit, item_node); // update identifiers in the set
        ++set_explicit->nodes_counter; // if new item was inserted then add one to counter
    }
    return inserted;
}

void set_remove(void* set, const void* item, void (*destroy)(void*))
{
    if (set == NULL || item == NULL)
    {
        return;
    }

    Set* set_explicit = set;
    // Find the row where item could exist
    const size_t row_index = bounded_hash(set_explicit, item);
    if (set_explicit->rows[row_index] != NULL)
    {
        SetNode* item_node = set_unbind(set_explicit, row_index, item); // unlink the item_node from other nodes in collisions list
        update_id_remove(set_explicit, item_node); // update identifiers in the set, if item_node != NULL
        row_destroy(item_node, destroy); // completely destroy the node
        set_explicit->nodes_counter -= item_node == NULL ? 0 : 1;
    }
}

size_t set_first(const void* set)
{   
    const bool first_edge = true;
    return set_edge_node(set, first_edge);
}

size_t set_last(const void* set)
{
    const bool first_edge = false;
    return set_edge_node(set, first_edge);
}

size_t set_next(const void* set, size_t item_id)
{
    const bool next_required = true;
    return set_one_step(set, item_id, next_required);
}

size_t set_prev(const void* set, size_t item_id)
{
    const bool next_required = false;
    return set_one_step(set, item_id, next_required);
}

size_t set_stop(const void* set)
{
	return set == (const void*)INVALID ? INVALID : 0;
}

const void* set_current(const void* set, size_t item_id)
{
    const Set* set_explicit = set;
    if (set_explicit == NULL || set_explicit->first_node == NULL || item_id == set_stop(set_explicit) || item_id == INVALID)
    {
        return NULL;
    }
    
    const SetNode* item_node = (const SetNode*)item_id; // id is address of HTableNode
    const void* item = item_node->item;
    return item;
}

void set_erase(void* set, size_t item_id, void(*destroy)(void*))
{
    const void* erasing_item = set_current(set, item_id);
    if (erasing_item != NULL)
    {
        set_remove(set, erasing_item, destroy);
    }
}


static size_t bounded_hash(const Set* set, const void* item)
{
    return set->hash(item) % set->table_size;
}

static SetNode* row_create(const size_t item_size, const void* item)
{
    // Create new node
    SetNode* new_node = calloc(1, sizeof(SetNode));
    if (new_node == NULL)
    {
        return NULL;
    }

    // Memory for new node allocated. Allocate memory for item
    new_node->item = malloc(item_size);

    if (new_node->item == NULL)
    {
        // Memory wasn't allocated. Free all other memory and return NULL
        free(new_node);
        return NULL;
    }

    // Memory allocated. Copy item
    memcpy(new_node->item, item, item_size);
    return new_node;
}

static void* row_destroy(SetNode* row, void(*destroy)(void*))
{
    if (row == NULL)
    {
        return NULL;
    }

    SetNode* current_node = row;
    while (current_node != NULL)
    {
        if (destroy != NULL)
        {
            destroy(current_node->item); // clear dynamic data in item object
        }
        free(current_node->item); // free item memory
        SetNode* next_node = current_node->collision;
        free(current_node);
        current_node = next_node;
    }

    return NULL;
}

static size_t row_count(const SetNode* row)
{
    size_t counter = 0;
    while (row != NULL)
    {
        ++counter;
        row = row->collision;
    }
    return counter;
}

static bool row_contains(const SetNode* row, const void* item, bool (*equals)(const void*, const void*))
{
    bool contains = false;
    while (row != NULL && contains == false)
    {
        const void* item_in_node = row->item;
        contains = equals(item, item_in_node);
        row = row->collision;
    }
    return contains;
}

static SetNode* row_insert(Set* set, const size_t row_index, const void* item)
{
    bool found = false; // flag
    SetNode* row = set->rows[row_index]; // row to inserting
    const SetNode* current_node = row; // pointer for iteration through the row
    while (current_node != NULL && found == false)
    {
        found = set->equals(item, current_node->item);
        current_node = current_node->collision; // go to the next node
    }
    if (found)
    {
        return NULL;
    }
    
    // Key wasn't found. Insert new node
    SetNode* item_node = row_create(set->item_size, item); // create new node with the item;
    if (item_node == NULL)
    {
        return NULL;
    }
    // Memory was allocated
    item_node->collision = row; // bind with other collisions
    set->rows[row_index] = item_node; // update collisions list in the set
    if (row_count(item_node) > LONGEST_AVAILABLE_COLLISION)
    {
        table_increase(set); // increase table two times if collisions list became too long
    }
    return item_node;
}

static void update_id_insert(Set* set, SetNode* item_node)
{
    if (set->first_node == NULL)
    {
        // First item in the whole set
        set->first_node = item_node;
        set->last_node = item_node;
    }
    else
    {
        // Not first item
        SetNode* prev_last = set->last_node;
        set->last_node = item_node;
        prev_last->next = item_node;
        item_node->prev = prev_last;
    }
}

static SetNode* set_unbind(Set* set, const size_t row_index, const void* item)
{
    SetNode* current_node = set->rows[row_index];
    SetNode* item_node = NULL;
    if (set->equals(item, current_node->item))
    {
        // Remove the head of the row
        item_node = current_node;
        set->rows[row_index] = item_node->collision;
        item_node->collision = NULL;
        return item_node;
    }
    // Find deeper
    while (current_node->collision != NULL && item_node == NULL)
    {
        if (set->equals(item, current_node->collision->item))
        {
            item_node = current_node->collision;
            current_node->collision = item_node->collision;
            item_node->collision = NULL;
        }
        else
        {
            current_node = current_node->collision;
        }
    }
    return item_node;
}

static void update_id_remove(Set* set, SetNode* item_node)
{
    if (item_node == NULL)
    {
        return;
    }

    // Node was founded and we have to update identifiers
    SetNode* next_node = item_node->next;
    SetNode* prev_node = item_node->prev;

    if (prev_node != NULL)
    {
        // Removing item is not first
        prev_node->next = next_node;
    }
    else
    {
        // Remove first item
        set->first_node = next_node;
    }

    if (next_node != NULL)
    {
        // Removing item is not last
        next_node->prev = prev_node;
    }
    else
    {
        // Remove last item
        set->last_node = prev_node;
    }

    item_node->next = NULL;
    item_node->prev = NULL;
}

static void table_increase(Set* set)
{
    if (set->table_size >= ULLONG_MAX / 2)
    {
        return;
    }

    const size_t new_size = set->table_size * 2;
    SetNode** new_rows = calloc(new_size, sizeof(SetNode*)); // create new pointers to the rows
    if (new_rows != NULL)
    {
        // Memory for pointers was allocated
        fill_rows(new_rows, set->rows, new_size, set->table_size, set->hash); // reassign pointers
        free(set->rows); // free memory allocated for old pointers
        set->rows = new_rows; // memorize new rows
        set->table_size = new_size;
    }
}

static void fill_rows(SetNode** new_rows, SetNode** old_rows, const size_t new_size, const size_t old_size, size_t(*hash)(const void*))
{
    for (size_t row_index = 0; row_index < old_size; ++row_index)
    {
        // Handle each row
        SetNode* current_node = old_rows[row_index];
        while (current_node != NULL)
        {
            // Handle each node in the row
            const void* current_item = current_node->item;
            const size_t new_index = hash(current_item) % new_size; // row index for key in the increased table
            SetNode* next_node = current_node->collision; // save the tail
            current_node->collision = new_rows[new_index]; // replace the head
            new_rows[new_index] = current_node; // update collisions list in the new_rows
            current_node = next_node; // go to the next element in the old_rows
        }
    }
}

static size_t set_edge_node(const Set* set, const bool first_edge)
{
    if (set == NULL || set->first_node == NULL)
    {
        return set_stop(set);
    }

    const size_t id = first_edge ? (size_t)set->first_node : (size_t)set->last_node; // id is address of SetNode
    return id;
}

static size_t set_one_step(const Set* set, const size_t item_id, const bool step_next)
{
    if (set == NULL || set->first_node == NULL || item_id == set_stop(set) || item_id == INVALID)
    {
        return set_stop(set);
    }

    const SetNode* item_node = (const SetNode*)item_id;
    const SetNode* required_node = step_next ? item_node->next : item_node->prev;
    size_t id = required_node == NULL ? set_stop(set) : (size_t)required_node; // id is address of SetNode
    return id;
}