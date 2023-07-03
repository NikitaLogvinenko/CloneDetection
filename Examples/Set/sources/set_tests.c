// set_tests.c

#include "set_tests.h" // header with declarations
#include "set.h" // testing library
#include <assert.h> // for assert()
#include <string.h> // for strcmp and strlen
#include <stdio.h> // for printf
#include <stdbool.h> // for set_insert
#include <stdlib.h>


#define MSG_ASSERT(condition, success, fail) if (condition) {printf(success);} else {printf(fail);} if (true)


// Structure for items in set
typedef struct {
    char name[10];
} KeyValue;

// Hash function for items
static size_t hash(const void* ptr) {
    const int p = 31;
    const int m = (int)1e9 + 9;
    size_t hash_value = 0;
    size_t p_pow = 1;

    const KeyValue* keyValue = (const KeyValue*)ptr;

    for (size_t i = 0; i < 10; ++i) {
        const char c = keyValue->name[i];
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }

    return hash_value;
}

// Comparing function for items
static bool equals(const void* lhsp, const void* rhsp) {
    const KeyValue* lhs = (const KeyValue*)lhsp;
    const KeyValue* rhs = (const KeyValue*)rhsp;

    return 0 == strcmp(lhs->name, rhs->name);
}

// Destroy items
static void destroy(void* item)
{
    assert(item != NULL);
    KeyValue* item_explicit = item;
    assert(strlen(item_explicit->name) < 10);
}

// Structure for reinitialize set
typedef struct {
    char name[20];
} BigKeyValue;

// Dummy function for reinitialize set
static size_t dummy_hash(const void* ptr)
{
    return 1;
}

// Dummy function for reinitialize set
static bool dummy_equals(const void* lhsp, const void* rhsp)
{
    return true;
}

// Tests with set=NULL or other invalid parameters
static void invalid_params_set_tests(void)
{
    printf("\n---------------------------INVALID PARAMETERS TESTS---------------------------\n");
    // Auxiliary objects
    void* set_for_invalid_tests = set_create(sizeof(KeyValue), hash, equals);
    const KeyValue item_for_invalid_tests = { "Key-value" };

    // Create tests
    MSG_ASSERT(set_create(0, hash, equals) == NULL, "[set_create_zero_item_size] OK\n", "[set_create_zero_item_size] FAILED\n");
    MSG_ASSERT(set_create(sizeof(KeyValue), NULL, equals) == NULL, "[set_create_NULL_hash] OK\n", "[set_create_NULL_hash] FAILED\n");
    MSG_ASSERT(set_create(sizeof(KeyValue), hash, NULL) == NULL, "[set_create_NULL_equals] OK\n", "[set_create_NULL_equals] FAILED\n");

    // Destroy test
    set_destroy(NULL, destroy);
    printf("[set_destroy_NULL_set] OK\n");

    // Init tests
    MSG_ASSERT(set_init(NULL, sizeof(KeyValue), hash, equals, destroy) == NULL, "[set_init_NULL_set] OK\n", "[set_init_NULL_set] FAILED\n");
    MSG_ASSERT(set_init(set_for_invalid_tests, 0, hash, equals, destroy) == NULL, "[set_init_zero_item_size] OK\n", "[set_init_zero_item_size] FAILED\n");
    MSG_ASSERT(set_init(set_for_invalid_tests, sizeof(KeyValue), NULL, equals, destroy) == NULL, "[set_init_NULL_hash] OK\n", "[set_init_NULL_hash] FAILED\n");
    MSG_ASSERT(set_init(set_for_invalid_tests, sizeof(KeyValue), hash, NULL, destroy) == NULL, "[set_init_NULL_equals] OK\n", "[set_init_NULL_equals] FAILED\n");
    
    // Clear test
    set_clear(NULL, destroy);
    printf("[set_clear_NULL_set] OK\n");
    
    // Count test
    MSG_ASSERT(set_count(NULL) == INVALID, "[set_count_NULL_set] OK\n", "[set_count_NULL_set] FAILED\n");
    
    // Contains tests
    MSG_ASSERT(set_contains(NULL, &item_for_invalid_tests) == false, "[set_contains_NULL_set] OK\n", "[set_contains_NULL_set] FAILED\n");
    MSG_ASSERT(set_contains(set_for_invalid_tests, NULL) == false, "[set_contains_NULL_item] OK\n", "[set_contains_NULL_item] FAILED\n");
    
    // Insert tests
    MSG_ASSERT(set_insert(NULL, &item_for_invalid_tests) == false, "[set_insert_NULL_set] OK\n", "[set_insert_NULL_set] FAILED\n");
    MSG_ASSERT(set_insert(set_for_invalid_tests, NULL) == false, "[set_insert_NULL_item] OK\n", "[set_insert_NULL_item] FAILED\n");
    
    // Remove test
    set_remove(NULL, &item_for_invalid_tests, destroy);
    printf("[set_remove_NULL_set] OK\n");
    set_remove(set_for_invalid_tests, NULL, destroy);
    printf("[set_remove_NULL_item] OK\n");
    
    // First test
    MSG_ASSERT(set_first(NULL) == set_stop(set_for_invalid_tests), "[set_first_NULL_set] OK\n", "[set_first_NULL_set] FAILED\n");
    
    // Last test
    MSG_ASSERT(set_last(NULL) == set_stop(set_for_invalid_tests), "[set_last_NULL_set] OK\n", "[set_last_NULL_set] FAILED\n");
   
    // Next tests
    MSG_ASSERT(set_next(NULL, 1) == set_stop(set_for_invalid_tests), "[set_next_NULL_set] OK\n", "[set_next_NULL_set] FAILED\n");
    MSG_ASSERT(set_next(set_for_invalid_tests, set_stop(set_for_invalid_tests)) == set_stop(set_for_invalid_tests), "[set_next_stop_id] OK\n", "[set_next_stop_id] FAILED\n");
    MSG_ASSERT(set_next(set_for_invalid_tests, INVALID) == set_stop(set_for_invalid_tests), "[set_next_INVALID_id] OK\n", "[set_next_INVALID_id] FAILED\n");
   
    // Prev tests
    MSG_ASSERT(set_prev(NULL, 1) == set_stop(set_for_invalid_tests), "[set_prev_NULL_set] OK\n", "[set_prev_NULL_set] FAILED\n");
    MSG_ASSERT(set_prev(set_for_invalid_tests, set_stop(set_for_invalid_tests)) == set_stop(set_for_invalid_tests), "[set_prev_stop_id] OK\n", "[set_prev_stop_id] FAILED\n");
    
    // Stop test
    MSG_ASSERT(set_stop(NULL) == set_stop(set_for_invalid_tests), "[set_stop_NULL_set] OK\n", "[set_stop_NULL_set] FAILED\n");
   
    // Current tests
    MSG_ASSERT(set_current(NULL, 1) == NULL, "[set_current_NULL_set] OK\n", "[set_current_NULL_set] FAILED\n");
    MSG_ASSERT(set_current(set_for_invalid_tests, set_stop(set_for_invalid_tests)) == NULL, "[set_current_stop_id] OK\n", "[set_current_stop_id] FAILED\n");
    MSG_ASSERT(set_current(set_for_invalid_tests, INVALID) == NULL, "[set_current_INVALID_id] OK\n", "[set_current_INVALID_id] FAILED\n");
    
    // Erase tests
    set_erase(NULL, 1, destroy);
    printf("[set_erase_NULL_set] OK\n");
    set_erase(set_for_invalid_tests, set_stop(set_for_invalid_tests), destroy);
    printf("[set_erase_stop_id] OK\n");
   
    // End
    set_destroy(set_for_invalid_tests, NULL);
    printf("-------------------------------------------------------------------------------\n");
}

// Tests with empty set
static void empty_set_tests()
{
    printf("\n---------------------------EMPTY SET TESTS---------------------------\n");
    // Auxiliary objects
    const KeyValue kv_to_set = { "10_KV" };
    const KeyValue new_kv_to_set = { "10_NEW_KV" };
    const BigKeyValue big_kv_to_set = { "20_KV" };
    
    // Create test
    void* empty_set = set_create(sizeof(KeyValue), hash, equals);
    MSG_ASSERT(empty_set != NULL, "[set_create_empty_set] OK\n", "[set_create_empty_set] FAILED\n");

    // Destroy test
    set_destroy(empty_set, destroy);
    printf("[set_destroy_empty_set] OK\n");
    empty_set = set_create(sizeof(KeyValue), hash, equals);
    
    // Init tests
    void* old_address = empty_set;
    empty_set = set_init(empty_set, sizeof(KeyValue), hash, equals, destroy);
    MSG_ASSERT(empty_set = old_address, "[set_init_empty_set_keep_address] OK\n", "[set_init_empty_set_keep_address] FAILED\n");
    bool inserted = set_insert(empty_set, &kv_to_set);
    const KeyValue* kv_ptr = set_current(empty_set, set_first(empty_set));
    MSG_ASSERT(equals(kv_ptr, &kv_to_set) && inserted, "[set_init_empty_set_same_parameters] OK\n", "[set_init_empty_set_same_parameters] FAILED\n");
    set_clear(empty_set, NULL);

    set_init(empty_set, sizeof(BigKeyValue), hash, equals, destroy);
    inserted = set_insert(empty_set, &big_kv_to_set);
    const BigKeyValue* big_kv_ptr = set_current(empty_set, set_first(empty_set));
    MSG_ASSERT(strcmp(big_kv_ptr->name, big_kv_to_set.name) == 0 && inserted, "[set_init_empty_set_change_item_size] OK\n", "[set_init_empty_set_change_item_size] FAILED\n");
    set_clear(empty_set, NULL);

    set_init(empty_set, sizeof(KeyValue), dummy_hash, equals, destroy);
    inserted = set_insert(empty_set, &kv_to_set);
    inserted *= set_insert(empty_set, &new_kv_to_set);
    kv_ptr = set_current(empty_set, set_first(empty_set));
    bool two_compares = equals(kv_ptr, &kv_to_set) && inserted;
    kv_ptr = set_current(empty_set, set_last(empty_set));
    two_compares *= equals(kv_ptr, &new_kv_to_set);
    MSG_ASSERT(two_compares, "[set_init_empty_set_change_hash] OK\n", "[set_init_empty_set_change_hash] FAILED\n");
    set_clear(empty_set, destroy);

    set_init(empty_set, sizeof(KeyValue), dummy_hash, dummy_equals, destroy);
    inserted = set_insert(empty_set, &kv_to_set);
    inserted *= !(set_insert(empty_set, &new_kv_to_set));
    MSG_ASSERT(inserted, "[set_init_empty_set_change_equals] OK\n", "[set_init_empty_set_change_equals] FAILED\n");
    
    set_init(empty_set, sizeof(KeyValue), hash, equals, destroy);
    // Clear test
    set_clear(empty_set, destroy);
    printf("[set_clear_empty_set] OK\n");
    
    // Count test
    MSG_ASSERT(set_count(empty_set) == 0, "[set_count_empty_set] OK\n", "[set_count_empty_set] FAILED\n");
    
    // Contains test
    MSG_ASSERT(set_contains(empty_set, &kv_to_set) == false, "[set_contains_empty_set] OK\n", "[set_contains_empty_set] FAILED\n");
    
    // Insert test    
    inserted = set_insert(empty_set, &kv_to_set);
    kv_ptr = set_current(empty_set, set_first(empty_set));
    two_compares = inserted && equals(kv_ptr, &kv_to_set) && set_first(empty_set) == set_last(empty_set);
    const KeyValue kv_not_to_set = { "NTS_KV" };
    two_compares *= !set_contains(empty_set, &kv_not_to_set);
	MSG_ASSERT(two_compares, "[set_insert_empty_set] OK\n", "[set_insert_empty_set] FAILED\n");
    
    set_clear(empty_set, destroy);

    // Remove test
    set_remove(empty_set, &kv_to_set, destroy);
    printf("[set_remove_empty_set] OK\n");

    // Insert-remove-insert
    inserted = set_insert(empty_set, &kv_to_set);
    kv_ptr = set_current(empty_set, set_first(empty_set));
    two_compares = inserted && equals(kv_ptr, &kv_to_set) && set_first(empty_set) == set_last(empty_set);
	two_compares *= set_count(empty_set) == 1 && set_contains(empty_set, &kv_to_set);
    set_remove(empty_set, &kv_to_set, destroy);
    two_compares *= set_first(empty_set) == set_stop(empty_set) && set_last(empty_set) == set_stop(empty_set);
    two_compares *= set_count(empty_set) == 0 && !set_contains(empty_set, &kv_to_set);
    inserted = set_insert(empty_set, &kv_to_set);
    kv_ptr = set_current(empty_set, set_last(empty_set));
    two_compares *= inserted && equals(kv_ptr, &kv_to_set) && set_first(empty_set) == set_last(empty_set);
    two_compares *= set_count(empty_set) == 1 && set_contains(empty_set, &kv_to_set);
    MSG_ASSERT(two_compares, "[set_insert_remove_insert_empty_set] OK\n", "[set_insert_remove_insert_empty_set] FAILED\n");

    set_clear(empty_set, destroy);

    // Ten thousands inserts
    KeyValue big_array[10000];
    for (size_t i = 0; i < 10000; ++i)
    {
        _itoa_s(i, big_array[i].name, 10, 10);
        inserted = set_insert(empty_set, big_array + i);
        two_compares *= inserted && set_count(empty_set) == i + 1;
        inserted = set_insert(empty_set, big_array + i);
        two_compares *= inserted == false && set_count(empty_set) == i + 1;

    }
    for (int i = 9999; i >= 0; --i)
    {
        set_remove(empty_set, big_array + i, destroy);
        two_compares *= set_count(empty_set) == (size_t)i;
    }
    for (size_t i = 0; i < 10000; ++i)
    {
        inserted = set_insert(empty_set, big_array + i);
        two_compares *= inserted && set_count(empty_set) == i + 1;
    }
    MSG_ASSERT(two_compares, "[set_ten_thousands_inserts_empty_set] OK\n", "[set_ten_thousands_inserts_empty_set] FAILED\n");
    
    set_clear(empty_set, destroy);
    // First test
    MSG_ASSERT(set_first(empty_set) == set_stop(empty_set), "[set_first_empty_set] OK\n", "[set_first_empty_set] FAILED\n");
    
    // Last test
    MSG_ASSERT(set_last(empty_set) == set_stop(empty_set), "[set_last_empty_set] OK\n", "[set_last_empty_set] FAILED\n");
    
    // Next test
    MSG_ASSERT(set_next(empty_set, 1) == set_stop(empty_set), "[set_next_empty_set] OK\n", "[set_next_empty_set] FAILED\n");
    
    // Prev test
    MSG_ASSERT(set_prev(empty_set, 1) == set_stop(empty_set), "[set_prev_empty_set] OK\n", "[set_prev_empty_set] FAILED\n");
    
    // Current test
    MSG_ASSERT(set_current(empty_set, 1) == NULL, "[set_current_empty_set] OK\n", "[set_current_empty_set] FAILED\n");
    
    // Erase test
    set_erase(empty_set, 1, destroy);
    printf("[set_erase_empty_set] OK\n");
    
    // End
    set_destroy(empty_set, NULL);
    printf("---------------------------------------------------------------------\n");
}

// Tests with set with single item
static void one_item_set_tests()
{
    printf("\n---------------------------ONE ITEM SET TESTS---------------------------\n");
    // Auxiliary objects
    const KeyValue kv_to_set = { "10_KEY" };
    const KeyValue new_kv_to_set = { "10_NEWKEY" };
    const BigKeyValue big_kv_to_set = { "20_KEY" };
    const KeyValue* kv_ptr = NULL;
    const BigKeyValue* big_kv_ptr = NULL;
    bool inserted, compare;
    
    // Create_destroy test
    void* one_item_set = set_create(sizeof(KeyValue), hash, equals);
    inserted = set_insert(one_item_set, &kv_to_set);
    if (set_count(one_item_set) == 1)
    {
        set_destroy(one_item_set, destroy);
        printf("[set_create_destroy_one_item_set] OK\n");
    }
    else
    {
        printf("[set_create_destroy_one_item_set] FAILED\n");
    }
    
    // Create_clear test
    one_item_set = set_create(sizeof(KeyValue), hash, equals);
    inserted = set_insert(one_item_set, &kv_to_set);
    if (set_count(one_item_set) == 1 && set_first(one_item_set) == set_last(one_item_set))
    {
        set_clear(one_item_set, destroy);
        if (set_count(one_item_set) == 0 && set_first(one_item_set) == set_stop(one_item_set) && set_last(one_item_set) == set_stop(one_item_set))
        {
            printf("[set_create_clear_one_item_set] OK\n");
        }
        else
        {
            printf("[set_create_clear_one_item_set] FAILED\n");
        }
    }
    else
    {
        printf("[set_create_clear_one_item_set] FAILED\n");
    }

    inserted = set_insert(one_item_set, &kv_to_set);
    // Init tests
    set_init(one_item_set, 0, hash, equals, destroy);
    set_init(one_item_set, sizeof(KeyValue), NULL, equals, destroy);
    set_init(one_item_set, sizeof(KeyValue), hash, NULL, destroy);
    MSG_ASSERT(set_count(one_item_set) == 1, "[set_init_one_item_set_wrong_new_params] OK\n", "[set_init_one_item_set_wrong_new_params] FAILED\n");

    set_init(one_item_set, sizeof(BigKeyValue), dummy_hash, dummy_equals, destroy);
    compare = (set_count(one_item_set) == 0);
    inserted = set_insert(one_item_set, &big_kv_to_set);
    big_kv_ptr = set_current(one_item_set, set_first(one_item_set));
    compare *= (strcmp(big_kv_ptr->name, big_kv_to_set.name) == 0 && inserted);
    MSG_ASSERT(compare, "[set_init_one_item_set] OK\n", "[set_init_one_item_set] FAILED\n");
    
    set_init(one_item_set, sizeof(KeyValue), hash, equals, destroy);
    inserted = set_insert(one_item_set, &kv_to_set);
    // Count test
    MSG_ASSERT(set_count(one_item_set) == 1, "[set_count_one_item_set] OK\n", "[set_count_one_item_set] FAILED\n");
    
    // Contains tests
    inserted = set_contains(one_item_set, &kv_to_set);
    MSG_ASSERT(inserted == true, "[set_item_one_item_set_existed_key] OK\n", "[set_item_one_item_set_existed_key] FAILED\n");
    inserted = set_contains(one_item_set, &new_kv_to_set);
    MSG_ASSERT(inserted == false, "[set_item_one_item_set_unexisted_key] OK\n", "[set_item_one_item_set_unexisted_key] FAILED\n");
    
    // Insert tests
    inserted = set_insert(one_item_set, &kv_to_set);
    MSG_ASSERT(inserted == false, "[set_insert_one_item_set_existed_key] OK\n", "[set_insert_one_item_set_existed_key] FAILED\n");
    inserted = set_insert(one_item_set, &new_kv_to_set);
    compare = (set_count(one_item_set) == 2 && inserted);
    MSG_ASSERT(compare, "[set_insert_one_item_set_new_key] OK\n", "[set_insert_one_item_set_new_key] FAILED\n");
    
    set_clear(one_item_set, destroy);
    inserted = set_insert(one_item_set, &kv_to_set);
    // Remove test
    set_remove(one_item_set, &new_kv_to_set, destroy);
    MSG_ASSERT(compare, "[set_remove_one_item_set_unexisted_key] OK\n", "[set_remove_one_item_set_unexisted_key] FAILED\n");
    set_remove(one_item_set, &kv_to_set, destroy);
    MSG_ASSERT(compare, "[set_remove_one_item_set_existed_key] OK\n", "[set_remove_one_item_set_existed_key] FAILED\n");

    inserted = set_insert(one_item_set, &kv_to_set);
    // ID tests
    MSG_ASSERT(set_first(one_item_set) == set_last(one_item_set), "[set_one_item_set_first_equal_last] OK\n", "[set_one_item_set_first_equal_last] FAILED\n");

    MSG_ASSERT(set_next(one_item_set, set_first(one_item_set)) == set_stop(one_item_set), "[set_one_item_set_next_first_stop] OK\n", "[set_one_item_set_next_first_stop] FAILED\n");
    MSG_ASSERT(set_prev(one_item_set, set_first(one_item_set)) == set_stop(one_item_set), "[set_one_item_set_prev_first_stop] OK\n", "[set_one_item_set_prev_first_stop] FAILED\n");
    MSG_ASSERT(set_next(one_item_set, set_last(one_item_set)) == set_stop(one_item_set), "[set_one_item_set_next_last_stop] OK\n", "[set_one_item_set_next_last_stop] FAILED\n");
    MSG_ASSERT(set_prev(one_item_set, set_last(one_item_set)) == set_stop(one_item_set), "[set_one_item_set_prev_last_stop] OK\n", "[set_one_item_set_prev_last_stop] FAILED\n");

    kv_ptr = set_current(one_item_set, set_first(one_item_set));
    MSG_ASSERT(equals(kv_ptr, &kv_to_set), "[set_one_item_set_first_current] OK\n", "[set_one_item_set_first_current] FAILED\n");
    kv_ptr = set_current(one_item_set, set_last(one_item_set));
    MSG_ASSERT(equals(kv_ptr, &kv_to_set), "[set_one_item_set_last_current] OK\n", "[set_one_item_set_last_current] FAILED\n");
    kv_ptr = set_current(one_item_set, set_next(one_item_set, set_first(one_item_set)));
    MSG_ASSERT(kv_ptr == NULL, "[set_one_item_set_first_next_current] OK\n", "[set_one_item_set_first_next_current] FAILED\n");
    kv_ptr = set_current(one_item_set, set_prev(one_item_set, set_first(one_item_set)));
    MSG_ASSERT(kv_ptr == NULL, "[set_one_item_set_first_prev_current] OK\n", "[set_one_item_set_first_prev_current] FAILED\n");
    kv_ptr = set_current(one_item_set, set_next(one_item_set, set_last(one_item_set)));
    MSG_ASSERT(kv_ptr == NULL, "[set_one_item_set_last_next_current] OK\n", "[set_one_item_set_last_next_current] FAILED\n");
    kv_ptr = set_current(one_item_set, set_prev(one_item_set, set_last(one_item_set)));
    MSG_ASSERT(kv_ptr == NULL, "[set_one_item_set_last_prev_current] OK\n", "[set_one_item_set_last_prev_current] FAILED\n");
    
    // Erase tests
    set_erase(one_item_set, set_stop(one_item_set), destroy);
    MSG_ASSERT(set_count(one_item_set) == 1, "[set_erase_one_item_set_stop_id] OK\n", "[set_erase_one_item_set_stop_id] FAILED\n");

    set_erase(one_item_set, set_first(one_item_set), destroy);
    compare = set_count(one_item_set) == 0 && set_first(one_item_set) == set_stop(one_item_set) && set_last(one_item_set) == set_stop(one_item_set);
    MSG_ASSERT(compare, "[set_erase_one_item_set_first] OK\n", "[set_erase_one_item_set_first] FAILED\n");

    inserted = set_insert(one_item_set, &kv_to_set);
    set_erase(one_item_set, set_last(one_item_set), destroy);
    compare = set_count(one_item_set) == 0 && set_first(one_item_set) == set_stop(one_item_set) && set_last(one_item_set) == set_stop(one_item_set);
    MSG_ASSERT(compare, "[set_erase_one_item_set_last] OK\n", "[set_erase_one_item_set_last] FAILED\n");

    // End
    set_destroy(one_item_set, NULL);
    printf("-------------------------------------------------------------------------------\n");
}

// Tests with set with several items
static void several_items_set_tests()
{
    printf("\n---------------------------SEVERAL ITEMS SET TESTS---------------------------\n");
    // Auxiliary objects
    const KeyValue kv_1 = { "KV_1" };
    const KeyValue kv_2 = { "KV_2" };
    const KeyValue kv_3 = { "KV_3" };
    const KeyValue kv_4 = { "KV_4" };
    const KeyValue kv_5 = { "KV_5" };
    const KeyValue kv_6 = { "KV_6" };
    const KeyValue kv_7 = { "KV_7" };
    const KeyValue unexisted_kv = { "UNEXISTED" };
    const KeyValue* kv_ptr = NULL;
    bool inserted, compare;

    // Create_destroy test
    void* set = set_create(sizeof(KeyValue), hash, equals);
    inserted = set_insert(set, &kv_1);
    inserted *= set_insert(set, &kv_2);
    inserted *= set_insert(set, &kv_3);
    inserted *= set_insert(set, &kv_4);
    inserted *= set_insert(set, &kv_5);
    inserted *= set_insert(set, &kv_6);
    inserted *= set_insert(set, &kv_7);
    if (set_count(set) == 7 && inserted)
    {
        set_destroy(set, destroy);
        printf("[set_create_destroy_several_items_set] OK\n");
    }
    else
    {
        printf("[set_create_destroy_several_items_set] FAILED\n");
    }

    // Create_clear test
    set = set_create(sizeof(KeyValue), hash, equals);
    inserted = set_insert(set, &kv_1);
    inserted *= set_insert(set, &kv_2);
    inserted *= set_insert(set, &kv_3);
    inserted *= set_insert(set, &kv_4);
    inserted *= set_insert(set, &kv_5);
    inserted *= set_insert(set, &kv_6);
    inserted *= set_insert(set, &kv_7);
    set_clear(set, destroy);
    if (set_count(set) == 0 && set_first(set) == set_stop(set) && set_last(set) == set_stop(set) && inserted)
    {
        printf("[set_create_clear_several_items_set] OK\n");
    }
    else
    {
        printf("[set_create_clear_several_items_set] FAILED\n");
    }

    
    // Init tests
    inserted = set_insert(set, &kv_1);
    inserted = set_insert(set, &kv_2);
    inserted = set_insert(set, &kv_3);
    inserted = set_insert(set, &kv_4);
    set_init(set, 0, hash, equals, destroy);
    set_init(set, sizeof(KeyValue), NULL, equals, destroy);
    set_init(set, sizeof(KeyValue), hash, NULL, destroy);
    MSG_ASSERT(set_count(set) == 4, "[set_init_several_items_set_wrong_new_params] OK\n", "[set_init_several_items_set_wrong_new_params] FAILED\n");

    set_init(set, sizeof(BigKeyValue), dummy_hash, dummy_equals, destroy);
    MSG_ASSERT(set_count(set) == 0, "[set_init_several_items_set] OK\n", "[set_init_several_items_set] FAILED\n");

    // Count test
    set_init(set, sizeof(KeyValue), hash, equals, destroy);
    inserted = set_insert(set, &kv_1);
    inserted = set_insert(set, &kv_2);
    inserted = set_insert(set, &kv_3);
    inserted = set_insert(set, &kv_4);
    MSG_ASSERT(set_count(set) == 4, "[set_count_several_items_set] OK\n", "[set_count_several_items_set] FAILED\n");

    // Contains tests
    inserted = set_contains(set, &kv_1);
    inserted *= set_contains(set, &kv_2);
    inserted *= set_contains(set, &kv_3);
    inserted *= set_contains(set, &kv_4);
    MSG_ASSERT(inserted, "[set_item_several_items_set_existed_keys] OK\n", "[set_item_several_items_set_existed_keys] FAILED\n");

    inserted *= set_contains(set, &unexisted_kv);
    MSG_ASSERT(inserted == false, "[set_item_several_items_set_unexisted_key] OK\n", "[set_item_several_items_set_unexisted_key] FAILED\n");

    // Insert tests
    inserted = set_insert(set, &kv_2);
    compare = (set_count(set) == 4 && inserted == false);
    MSG_ASSERT(compare, "[set_insert_several_items_set_existed_key] OK\n", "[set_insert_several_items_set_existed_key] FAILED\n");

    inserted = set_insert(set, &kv_5);
    inserted *= set_count(set) == 5;
    inserted *= set_insert(set, &kv_6);
    inserted *= set_count(set) == 6;
    inserted *= set_insert(set, &kv_7);
    inserted *= set_count(set) == 7;
    MSG_ASSERT(inserted, "[set_insert_several_items_set_new_keys] OK\n", "[set_insert_several_items_set_new_keys] FAILED\n");

    // Remove tests
    set_remove(set, &unexisted_kv, destroy);
    compare = set_count(set) == 7;
    MSG_ASSERT(compare, "[set_remove_several_items_set_unexisted_key] OK\n", "[set_remove_several_items_set_unexisted_key] FAILED\n");

    const void* former_second = set_current(set, set_next(set, set_first(set)));
    const void* former_before_last = set_current(set, set_prev(set, set_last(set)));
    set_remove(set, &kv_1, destroy);
    compare = set_count(set) == 6 && set_current(set, set_first(set)) == former_second;
    MSG_ASSERT(compare, "[set_remove_several_items_set_remove_first] OK\n", "[set_remove_several_items_set_remove_first] FAILED\n");
    set_remove(set, &kv_7, destroy);
    compare = set_count(set) == 5 && set_current(set, set_last(set)) == former_before_last;
    MSG_ASSERT(compare, "[set_remove_several_items_set_remove_last] OK\n", "[set_remove_several_items_set_remove_last] FAILED\n");
    set_remove(set, &kv_4, destroy);
    compare = set_count(set) == 4;
    MSG_ASSERT(compare, "[set_remove_several_items_set_remove_central] OK\n", "[set_remove_several_items_set_remove_central] FAILED\n");
    set_remove(set, &kv_2, destroy);
    set_remove(set, &kv_3, destroy);
    set_remove(set, &kv_5, destroy);
    set_remove(set, &kv_6, destroy);
    compare = set_count(set) == 0 && set_first(set) == set_stop(set) && set_last(set) == set_stop(set);
    MSG_ASSERT(compare, "[set_remove_several_items_set_remove_all] OK\n", "[set_remove_several_items_set_remove_all] FAILED\n");

    // ID tests
    inserted = set_insert(set, &kv_1);
    inserted = set_insert(set, &kv_2);
    compare = set_first(set) != set_last(set);
    MSG_ASSERT(compare, "[set_id_two_items_set_first_inequal_last] OK\n", "[set_id_two_items_set_first_inequal_last] FAILED\n");

    inserted = set_insert(set, &kv_3);
    inserted = set_insert(set, &kv_4);
    inserted = set_insert(set, &kv_5);
    inserted = set_insert(set, &kv_6);
    inserted = set_insert(set, &kv_7);
      
    // Forward loop
    size_t current_id = set_first(set);
    kv_ptr = set_current(set, current_id);
    int symbols_counter = 0;
    while (current_id != set_stop(set))
    {
        symbols_counter += strlen(kv_ptr->name);
        current_id = set_next(set, current_id);
        kv_ptr = set_current(set, current_id);
    }
    MSG_ASSERT(symbols_counter == 28, "[set_id_several_items_set_forward_loop] OK\n", "[set_id_several_items_set_forward_loop] FAILED\n");

    // Backward loop
    current_id = set_last(set);
    kv_ptr = set_current(set, current_id);
    symbols_counter = 0;
    while (current_id != set_stop(set))
    {
        symbols_counter += strlen(kv_ptr->name);
        current_id = set_prev(set, current_id);
        kv_ptr = set_current(set, current_id);
    }
    MSG_ASSERT(symbols_counter == 28, "[set_id_several_items_set_backward_loop] OK\n", "[set_id_several_items_set_backward_loop] FAILED\n");

    // Erase tests
    current_id = set_first(set);
    while (current_id != set_stop(set))
    {
        set_erase(set, current_id, destroy);
        current_id = set_first(set);
    }
    compare = set_count(set) == 0 && set_first(set) == set_stop(set) && set_last(set) == set_stop(set);
    MSG_ASSERT(compare, "[set_id_several_items_set_erase] OK\n", "[set_id_several_items_set_erase] FAILED\n");
    
    // End
    set_destroy(set, NULL);
    printf("-----------------------------------------------------------------------------\n");
}

void run_all_set_tests()
{
    invalid_params_set_tests();
    empty_set_tests();
    one_item_set_tests();
    several_items_set_tests();
}