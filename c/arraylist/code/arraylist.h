/**
 * Header file for Arraylist.
*/

#ifndef ARRAYLIST_H_
#define ARRAYLIST_H_

#include <stdbool.h>

typedef struct Arraylist *Arraylist;
typedef void *Value;
struct Arraylist {
    int length;  /* Length of elements */
    int capacity;  /* Length of internal array */
    Value *array;
};

/* Initialize/Free */
Arraylist arraylist_init(const int initial_len);
void arraylist_free(const Arraylist a);

/* Get/Remove internal array elements */
bool arraylist_empty(const Arraylist a);
Arraylist arraylist_clear(const Arraylist a);
Value arraylist_get(const Arraylist a, const int index);
Value arraylist_pop(const Arraylist a, const int index);
Value arraylist_set(const Arraylist a, const int index, const Value value);
Value arraylist_push(const Arraylist a, const int index, const Value value);

/* Get size */
int arraylist_length(const Arraylist a);
int arraylist_capacity(const Arraylist a);

/* Set size */
Arraylist arraylist_resize(const Arraylist a, const int length);
Arraylist arraylist_reserve(const Arraylist a, const int capacity);

/* Iterate */
void arraylist_foreach(const Arraylist a, const void (*f)(Value));
void arraylist_map(const Arraylist a, const Value (*f)(Value));

#endif