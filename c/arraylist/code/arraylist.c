/**
 * Implementation file for Arraylist.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>
#include "arraylist.h"

const unsigned int MIN_CAPACITY = 10;
const float MIN_FILLED_RATIO = .3;
const float IDEAL_FILLED_RATIO = .5;
const float MAX_FILLED_RATIO = .7;

bool invalid_index(const Arraylist a, const int index);
Arraylist fix_capacity(const Arraylist a);

/**
 * Initialized a new Arraylist.
 * 
 * Inputs:
 *     const int initial_length: Initial length of the internal array.
 * Returns:
 *     Arraylist: NULL if the process fails,
 *                Arraylist that is newly created otherwise.
*/
Arraylist arraylist_init(const int initial_length) {
    if (initial_length < 0) {
        return NULL;
    }

    /* Initial size */
    float ideal_capacity = (float)initial_length / (float)IDEAL_FILLED_RATIO;
    int initial_capacity =
        MIN_CAPACITY > ideal_capacity
        ? MIN_CAPACITY
        : ideal_capacity;

    /* Malloc */
    Arraylist a = malloc(sizeof(*a));
    Value *array = calloc(initial_capacity, sizeof(*array));

    if (a == NULL || array == NULL) {
        free(a);
        free(array);
        return NULL;
    }

    /* Initialize */
    a->length = initial_length;
    a->capacity = initial_capacity;
    a->array = array;

    return a;
}

/**
 * Free an Arraylist.
 * 
 * Inputs:
 *     const Arraylist a: Arraylist to use.
 * Returns:
 *     Nothing.
*/
void arraylist_free(Arraylist a) {
    if (a) {
        free(a->array);
        free(a);
    }
}

/**
 * Query whether the Arraylist has a length of 0.
 * 
 * Inputs:
 *     const Arraylist a: Arraylist to use.
 * Returns:
 *     bool: Whether the Arraylist has a length of 0.
*/
bool arraylist_empty(const Arraylist a) {
    return a->length == 0;
}

/**
 * Remove all elements from the Arraylist.
 * 
 * Inputs:
 *     const Arraylist a: Arraylist to use.
 * Returns:
 *     Arraylist: NULL if the process fails,
 *                Arraylist otherwise.
*/
Arraylist arraylist_clear(const Arraylist a) {
    return arraylist_resize(a, 0);
}

/**
 * Get an index's Value from an Arraylist.
 * 
 * Inputs:
 *     const Arraylist a: Arraylist to use.
 *     const int index: The index to access.
 * Returns:
 *     Value: NULL if the process fails,
 *            Value at the Arraylist's index otherwise.
*/
Value arraylist_get(const Arraylist a, const int index) {
    if (invalid_index(a, index)) {
        return NULL;
    }

    /* Get value */
    Value value = a->array[index];
    return value;
}

/**
 * Get an index's Value, remove that item, and shift elements over.
 * 
 * Inputs:
 *     const Arraylist a: Arraylist to use.
 *     const int index: The index to access.
 * Returns:
 *     Value: NULL if the process fails,
 *            Value at the Arraylist's index otherwise.
*/
Value arraylist_pop(const Arraylist a, const int index) {
    if (invalid_index(a, index)) {
        return NULL;
    }

    /* Get value */
    Value value = a->array[index];

    /* Shift values */
    const int new_length = a->length - 1;
    for (int i = index; i < new_length; i++) {
        a->array[i] = a->array[i + 1];
    }

    /* Shrink array */
    if (!arraylist_resize(a, new_length)) {
        return NULL;
    }
    return value;
}

/**
 * Set an index's Value from an Arraylist.
 * 
 * Inputs:
 *     const Arraylist a: Arraylist to use.
 *     const int index: The index to access.
 *     const Value value: The Value to set at the Arraylist's index.
 * Returns:
 *     Value: NULL if the process fails,
 *            Value that was inserted otherwise.
*/
Value arraylist_set(const Arraylist a, const int index, const Value value) {
    if (index < 0) {
        return NULL;
    }

    /* Keep array the same or expand array to index */
    const int new_length = (index < a->length) ? (a->length) : (index + 1);
    if (!arraylist_resize(a, new_length)) {
        return NULL;
    }

    /* Set value */
    a->array[index] = value;
    return value;
}

/**
 * Set an index's Value, shifting elements further back in an Arraylist.
 * 
 * Inputs:
 *     const Arraylist a: The Arraylist to use.
 *     const int index: The index to access.
 *     const Value value: The Value to set at the Arraylist's index.
 * Returns:
 *     Value: NULL if the process fails,
 *            Value that was inserted otherwise.
*/
Value arraylist_push(const Arraylist a, const int index, const Value value) {
    if (index < 0) {
        return NULL;
    }

    /* Expand array to +1 or expand array to index +1 */
    const int new_length = (index < a->length) ? (a->length + 1) : (index + 1);
    if (!arraylist_resize(a, new_length)) {
        return NULL;
    }

    /* Shift values */
    for (int i = index; i < new_length - 1; i++) {
        a->array[i + 1] = a->array[i];
    }

    /* Set value */
    a->array[index] = value;
    return value;
}

/**
 * Get the length of the available elements of an Arraylist.
 * 
 * Inputs:
 *     const Arraylist a: Arraylist to use.
 * Returns:
 *     int: The length of the available elements in the Arraylist.
*/
int arraylist_length(const Arraylist a) {
    return a->length;
}

/**
 * Get the capacity of the internal array of an Arraylist.
 * 
 * Inputs:
 *     const Arraylist a: Arraylist to use.
 * Returns:
 *     int: The capacity of the internal array of the Arraylist.
*/
int arraylist_capacity(const Arraylist a) {
    return a->capacity;
}

/**
 * Reallocates the internal array of an Arraylist to a new size.
 * 
 * Inputs:
 *     const Arraylist a: Arraylist to use.
 *     const int capacity: The new capacity to use.
 * Returns:
 *     Arraylist: NULL if the process fails,
 *                Arraylist otherwise.
*/
Arraylist arraylist_reserve(const Arraylist a, const int capacity) {
    if (capacity < 0) {
        return NULL;
    }
    
    Value *array = realloc(a->array, capacity * sizeof(*array));
    if (!array) {
        return NULL;
    }

    /* Zero-out remaining elements */
    for (int i = a->capacity; i < capacity; i++) {
        array[i] = NULL;
    }

    a->length = (a->length < capacity) ? a->length : capacity;
    a->capacity = capacity;
    a->array = array;
    return a;
}

/**
 * Sets the length of the Arraylist.
 * 
 * Inputs:
 *     const Arraylist a: Arraylist to use.
 *     const int length: The new length to use.
 * Returns:
 *     Arraylist: NULL if the process fails,
 *                Arraylist otherwise.
*/
Arraylist arraylist_resize(const Arraylist a, const int length) {
    if (length < 0) {
        return NULL;
    }
    a->length = length;
    return fix_capacity(a);
}

/**
 * Calls a function once for each element in the Arraylist,
 * from indices 0 to length - 1, using each element as input.
 * 
 * Inputs:
 *     const Arraylist a: Arraylist to use.
 *     const void (*f)(Value): Function to call for each element.
 * Returns:
 *     Nothing.
*/
void arraylist_foreach(const Arraylist a, const void (*f)(Value)) {
    for (int i = 0; i < a->length; i++) {
        f(a->array[i]);
    }
}

/**
 * Calls a function once for each element in the Arraylist,
 * from indices 0 to length - 1, setting each element with the return.
 * 
 * Inputs:
 *     const Arraylist a: Arraylist to use.
 *     const Value (*f)(Value): Function to call for each element.
 * Returns:
 *     Nothing.
*/
void arraylist_map(const Arraylist a, Value (*f)(Value)) {
    for (int i = 0; i < a->length; i++) {
        arraylist_set(a, i, f(a->array[i]));
    }
}

/**
 * Whether an input index is not accessible in the Arraylist.
 * 
 * Inputs:
 *     const Arraylist a: Arraylist to use.
 *     const int index: The index to access.
 * Returns: 
 *     bool: Whether the index is outside of range.
*/
bool invalid_index(const Arraylist a, const int index) {
    return index < 0 || index > a->length - 1;
}

/**
 * Update the size of the internal array and the Arraylist size attribute.
 * 
 * Inputs:
 *     const Arraylist a: Arraylist to change the size of.
 * Returns:
 *     Arraylist: NULL if the process fails,
 *                Arraylist otherwise.
*/
Arraylist fix_capacity(const Arraylist a) {
    /* In good range */
    float curr_ratio = (float)a->length / (float)a->capacity;
    if (curr_ratio > MIN_FILLED_RATIO && curr_ratio < MAX_FILLED_RATIO) {
        return a;
    }

    int new_capacity = (int)(a->length / IDEAL_FILLED_RATIO);

    /* Reserve min capacity */
    if (new_capacity < MIN_CAPACITY) {
        return arraylist_reserve(a, MIN_CAPACITY);
    }

    /* Reserve max capacity */
    if (new_capacity > INT_MAX && a->capacity < INT_MAX) {
        return arraylist_reserve(a, INT_MAX);
    }
    return arraylist_reserve(a, new_capacity);
}