
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include "../code/arraylist.h"

typedef struct UnitTest {
    void (*fn)();
    char *name;
} UnitTest;
typedef struct TestBool {
    bool result;
    bool expected;
} TestBool;
typedef struct TestInt {
    int result;
    int expected;
} TestInt;
typedef struct TestValue {
    Value result;
    Value expected;
} TestValue;
typedef struct TestArray {
    Value *result;
    Value *expected;
} TestArray;
typedef struct TestArraylist {
    Arraylist result;
    Arraylist expected;
} TestArraylist;

void assert_bool(const bool result, const bool expected) {
    assert(result == expected);
}

void assert_int(const int result, const int expected) {
    assert(result == expected);
}

void assert_value(const Value result, const Value expected) {
    assert(result == expected);
}

void assert_array(const Value *result, const Value *expected, const int size) {
    /* NULL */
    if (result == NULL || expected == NULL) {
        assert(result == expected);
        return;
    }

    /* Non-NULL */
    for (int i = 0; i < size; i++) {
        assert_value(result[i], expected[i]);
    }
}

void assert_arraylist(const Arraylist result, const Arraylist expected) {
    /* NULL */
    if (result == NULL || expected == NULL) {
        assert(result == expected);
        return;
    }

    /* Non-NULL */
    assert_int(result->length, expected->length);
    assert_int(result->capacity, expected->capacity);
    assert_array(result->array, expected->array, result->capacity);
}

/**
 * Case initial length is negative.
 * Case initial length is below minimum capacity.
 * Case default.
*/
void test_arraylist_init() {
    const TestArraylist tests[] = {
        { arraylist_init(-1), NULL },
        {
            arraylist_init(0),
            memcpy(
                malloc(sizeof(struct Arraylist)),
                &(struct Arraylist){ 0, 10, calloc(10, sizeof(Value)) },
                sizeof(struct Arraylist)
            )
        },
        {
            arraylist_init(100),
            memcpy(
                malloc(sizeof(struct Arraylist)),
                &(struct Arraylist){ 100, 200, calloc(200, sizeof(Value)) },
                sizeof(struct Arraylist)
            )
        },
    };

    const int num_tests = sizeof(tests) / sizeof(*tests);

    /* Test */
    for (int i = 0; i < num_tests; i++) {
        assert_arraylist(tests[i].result, tests[i].expected);
    }

    /* Free */
    for (int i = 0; i < num_tests; i++) {
        arraylist_free(tests[i].result);
        arraylist_free(tests[i].expected);
    }
}

/**
 * Case is empty.
 * Case is not empty.
*/
void test_arraylist_empty() {
    const Arraylist inputs[] = {
        arraylist_init(0),
        arraylist_init(1),
    };
    const TestBool tests[] = {
        { arraylist_empty(inputs[0]), true },
        { arraylist_empty(inputs[1]), false },
    };

    const int num_tests = sizeof(tests) / sizeof(*tests);

    /* Test */
    for (int i = 0; i < num_tests; i++) {
        assert_bool(tests[i].result, tests[i].expected);
    }

    /* Free */
    for (int i = 0; i < num_tests; i++) {
        arraylist_free(inputs[i]);
    }
}

/**
 * Case is empty.
 * Case is not empty.
*/
void test_arraylist_clear() {
    const TestArraylist tests[] = {
        {
            arraylist_clear(arraylist_init(0)),
            memcpy(
                malloc(sizeof(struct Arraylist)),
                &(struct Arraylist){ 0, 10, calloc(10, sizeof(Value)) },
                sizeof(struct Arraylist)
            )
        },
        {
            arraylist_clear(arraylist_init(100)),
            memcpy(
                malloc(sizeof(struct Arraylist)),
                &(struct Arraylist){ 0, 10, calloc(10, sizeof(Value)) },
                sizeof(struct Arraylist)
            )
        },
    };

    const int num_tests = sizeof(tests) / sizeof(*tests);

    /* Test */
    for (int i = 0; i < num_tests; i++) {
        assert_arraylist(tests[i].result, tests[i].expected);
    }

    /* Free */
    for (int i = 0; i < num_tests; i++) {
        arraylist_free(tests[i].result);
        arraylist_free(tests[i].expected);
    }
}

/**
 * Case invalid index.
 * Case default.
*/
void test_arraylist_get() {
    int value = 7;
    const Arraylist inputs[] = {
        arraylist_init(0),
        memcpy(
            malloc(sizeof(struct Arraylist)),
            &(struct Arraylist){
                3,
                5,
                memcpy(
                    malloc(5 * sizeof(Value)),
                    &(Value[]){ NULL, NULL, &value, NULL, NULL },
                    5 * sizeof(Value)
                )
            },
            sizeof(struct Arraylist)
        ),
    };
    const TestValue tests[] = {
        { arraylist_get(inputs[0], 1), NULL },
        { arraylist_get(inputs[1], 2), &value },
    };

    const int num_tests = sizeof(tests) / sizeof(*tests);

    /* Test */
    for (int i = 0; i < num_tests; i++) {
        assert_value(tests[i].result, tests[i].expected);
    }

    /* Free */
    for (int i = 0; i < num_tests; i++) {
        arraylist_free(inputs[i]);
    }
}

/**
 * Case invalid index.
 * Case default.
*/
void test_arraylist_pop() {
    int value = 7;
    const Arraylist inputs[] = {
        arraylist_init(0),
        memcpy(
            malloc(sizeof(struct Arraylist)),
            &(struct Arraylist){
                5,
                10,
                memcpy(
                    malloc(10 * sizeof(Value)),
                    &(Value[]){ NULL, &value, NULL, &value, NULL, NULL, NULL, NULL, NULL, NULL },
                    10 * sizeof(Value)
                )
            },
            sizeof(struct Arraylist)
        ),
    };
    const Arraylist outputs[] = {
        arraylist_init(0),
        memcpy(
            malloc(sizeof(struct Arraylist)),
            &(struct Arraylist){
                4,
                10,
                memcpy(
                    malloc(10 * sizeof(Value)),
                    &(Value[]){ NULL, &value, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
                    10 * sizeof(Value)
                )
            },
            sizeof(struct Arraylist)
        ),
    };
    const TestValue tests[] = {
        { arraylist_pop(inputs[0], 1), NULL },
        { arraylist_pop(inputs[1], 3), &value },
    };

    const int num_tests = sizeof(tests) / sizeof(*tests);

    /* Test */
    for (int i = 0; i < num_tests; i++) {
        assert_arraylist(inputs[i], outputs[i]);
        assert_value(tests[i].result, tests[i].expected);
    }

    /* Free */
    for (int i = 0; i < num_tests; i++) {
        arraylist_free(inputs[i]);
        arraylist_free(outputs[i]);
    }
}

/**
 * Case index below 0.
 * Case index past length.
 * Case index past capacity.
 * Case default (index within length).
*/
void test_arraylist_set() {
    int value = 7;
    const Arraylist inputs[] = {
        arraylist_init(0),
        arraylist_init(0),
        arraylist_init(0),
        arraylist_init(5),
    };
    const Arraylist outputs[] = {
        arraylist_init(0),
        memcpy(
            malloc(sizeof(struct Arraylist)),
            &(struct Arraylist){
                4,
                10,
                memcpy(
                    malloc(10 * sizeof(Value)),
                    &(Value[]){ NULL, NULL, NULL, &value, NULL, NULL, NULL, NULL, NULL, NULL },
                    10 * sizeof(Value)
                )
            },
            sizeof(struct Arraylist)
        ),
        memcpy(
            malloc(sizeof(struct Arraylist)),
            &(struct Arraylist){
                11,
                22,
                memcpy(
                    malloc(22 * sizeof(Value)),
                    &(Value[]){ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &value, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
                    22 * sizeof(Value)
                )
            },
            sizeof(struct Arraylist)
        ),
        memcpy(
            malloc(sizeof(struct Arraylist)),
            &(struct Arraylist){
                5,
                10,
                memcpy(
                    malloc(10 * sizeof(Value)),
                    &(Value[]){ NULL, NULL, NULL, &value, NULL, NULL, NULL, NULL, NULL, NULL },
                    10 * sizeof(Value)
                )
            },
            sizeof(struct Arraylist)
        ),
    };
    const TestValue tests[] = {
        { arraylist_set(inputs[0], -1, &value), NULL },
        { arraylist_set(inputs[1], 3, &value), &value },
        { arraylist_set(inputs[2], 10, &value), &value },
        { arraylist_set(inputs[3], 3, &value), &value },
    };

    const int num_tests = sizeof(tests) / sizeof(*tests);

    /* Test */
    for (int i = 0; i < num_tests; i++) {
        assert_arraylist(inputs[i], outputs[i]);
        assert_value(tests[i].result, tests[i].expected);
    }

    /* Free */
    for (int i = 0; i < num_tests; i++) {
        arraylist_free(inputs[i]);
        arraylist_free(outputs[i]);
    }
}

/**
 * Case index below 0.
 * Case index past length.
 * Case index past capacity.
 * Case default (index within length).
*/
void test_arraylist_push() {
    int value = 7;
    const Arraylist inputs[] = {
        arraylist_init(0),
        arraylist_init(0),
        arraylist_init(0),
        arraylist_init(5),
    };
    const Arraylist outputs[] = {
        memcpy(
            malloc(sizeof(struct Arraylist)),
            &(struct Arraylist){
                0,
                10,
                calloc(10, sizeof(Value))
            },
            sizeof(struct Arraylist)
        ),
        memcpy(
            malloc(sizeof(struct Arraylist)),
            &(struct Arraylist){
                4,
                10,
                memcpy(
                    malloc(10 * sizeof(Value)),
                    &(Value[]){ NULL, NULL, NULL, &value, NULL, NULL, NULL, NULL, NULL, NULL },
                    10 * sizeof(Value)
                )
            },
            sizeof(struct Arraylist)
        ),
        memcpy(
            malloc(sizeof(struct Arraylist)),
            &(struct Arraylist){
                11,
                22,
                memcpy(
                    malloc(22 * sizeof(Value)),
                    &(Value[]){ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &value, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
                    22 * sizeof(Value)
                )
            },
            sizeof(struct Arraylist)
        ),
        memcpy(
            malloc(sizeof(struct Arraylist)),
            &(struct Arraylist){
                6,
                10,
                memcpy(
                    malloc(10 * sizeof(Value)),
                    &(Value[]){ NULL, NULL, NULL, &value, NULL, NULL, NULL, NULL, NULL, NULL },
                    10 * sizeof(Value)
                )
            },
            sizeof(struct Arraylist)
        ),
    };
    const TestValue tests[] = {
        { arraylist_push(inputs[0], -1, &value), NULL },
        { arraylist_push(inputs[1], 3, &value), &value },
        { arraylist_push(inputs[2], 10, &value), &value },
        { arraylist_push(inputs[3], 3, &value), &value },
    };

    const int num_tests = sizeof(tests) / sizeof(*tests);

    /* Test */
    for (int i = 0; i < num_tests; i++) {
        assert_arraylist(inputs[i], outputs[i]);
        assert_value(tests[i].result, tests[i].expected);
    }

    /* Free */
    for (int i = 0; i < num_tests; i++) {
        arraylist_free(inputs[i]);
        arraylist_free(outputs[i]);
    }
}

/**
 * Case default.
*/
void test_arraylist_length() {
    const Arraylist inputs[] = {
        arraylist_init(5),
    };
    const TestInt tests[] = {
        { arraylist_length(inputs[0]), 5 },
    };

    const int num_tests = sizeof(tests) / sizeof(*tests);

    /* Test */
    for (int i = 0; i < num_tests; i++) {
        assert_int(tests[i].result, tests[i].expected);
    }

    /* Free */
    for (int i = 0; i < num_tests; i++) {
        arraylist_free(inputs[i]);
    }
}

/**
 * Case default
*/
void test_arraylist_capacity() {
    const Arraylist inputs[] = {
        arraylist_init(5),
    };
    const TestInt tests[] = {
        { arraylist_capacity(inputs[0]), 10 }
    };

    const int num_tests = sizeof(tests) / sizeof(*tests);

    /* Test */
    for (int i = 0; i < num_tests; i++) {
        assert_int(tests[i].result, tests[i].expected);
    }

    /* Free */
    for (int i = 0; i < num_tests; i++) {
        arraylist_free(inputs[i]);
    }
}

/**
 * Case capacity is negative.
 * Case new capacity is below minimum threshold.
 * Case new capacity is lower than old capacity.
 * Case new capacity is higher than old capacity.
*/
void test_arraylist_reserve() {
    const Arraylist inputs[] = {
        arraylist_init(10),
        arraylist_init(10),
        arraylist_init(6),
        arraylist_init(0),
    };
    const TestArraylist tests[] = {
        {
            arraylist_reserve(inputs[0], -1),
            NULL
        },
        {
            arraylist_reserve(inputs[1], 2),
            memcpy(
                malloc(sizeof(struct Arraylist)),
                &(struct Arraylist){
                    2,
                    2,
                    memcpy(
                        malloc(2 * sizeof(Value)),
                        &(Value[]){ NULL, NULL },
                        2 * sizeof(Value)
                    )
                },
                sizeof(struct Arraylist)
            )
        },
        {
            arraylist_reserve(inputs[2], 11),
            memcpy(
                malloc(sizeof(struct Arraylist)),
                &(struct Arraylist){
                    6,
                    11,
                    memcpy(
                        malloc(11 * sizeof(Value)),
                        &(Value[]){ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
                        11 * sizeof(Value)
                    )
                },
                sizeof(struct Arraylist)
            )
        },
        {
            arraylist_reserve(inputs[3], 11),
            memcpy(
                malloc(sizeof(struct Arraylist)),
                &(struct Arraylist){
                    0,
                    11,
                    memcpy(
                        malloc(11 * sizeof(Value)),
                        &(Value[]){ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
                        11 * sizeof(Value)
                    )
                },
                sizeof(struct Arraylist)
            )
        },
    };

    const int num_tests = sizeof(tests) / sizeof(*tests);

    /* Test */
    for (int i = 0; i < num_tests; i++) {
        assert_arraylist(tests[i].result, tests[i].expected);
    }

    /* Free */
    for (int i = 0; i < num_tests; i++) {
        arraylist_free(inputs[i]);
        arraylist_free(tests[i].expected);
    }
}

/**
 * Case length is negative.
 * Case capacity changes.
 * Case default (capacity does not change).
*/
void test_arraylist_resize() {
    const Arraylist inputs[] = {
        arraylist_init(0),
        arraylist_init(10),
        arraylist_init(0),
    };

    const TestArraylist tests[] = {
        { arraylist_resize(inputs[0], -1), NULL },
        { arraylist_resize(inputs[1], 0), arraylist_init(0) },
        { arraylist_resize(inputs[2], 2), arraylist_init(2) },
    };

    const int num_tests = sizeof(tests) / sizeof(*tests);

    /* Test */
    for (int i = 0; i < num_tests; i++) {
        assert_arraylist(tests[i].result, tests[i].expected);
    }

    /* Free */
    for (int i = 0; i < num_tests; i++) {
        arraylist_free(inputs[i]);
        arraylist_free(tests[i].expected);
    }
}

/**
 * Case default.
*/
int foreach_counter = 0;
void foreach_fn(Value input) {
    foreach_counter++;
}
void test_arraylist_foreach() {
    foreach_counter = 0;

    const Arraylist input = arraylist_init(7);

    /* Test */
    arraylist_foreach(input, foreach_fn);
    assert_int(foreach_counter, 7);

    /* Free */
    arraylist_free(input);
}

/**
 * Case default.
*/
int map_value = 7;
Value map_fn(Value value) {
    return &map_value;
}
void test_arraylist_map() {
    const TestArraylist tests[] = {
        {
            arraylist_init(5),
            memcpy(
                malloc(sizeof(struct Arraylist)),
                &(struct Arraylist){
                    5,
                    10,
                    memcpy(
                        malloc(10 * sizeof(Value)),
                        &(Value[]){ &map_value, &map_value, &map_value, &map_value, &map_value, NULL, NULL, NULL, NULL, NULL },
                        10 * sizeof(Value)
                    )
                },
                sizeof(struct Arraylist)
            )
        },
    };

    const int num_tests = sizeof(tests) / sizeof(*tests);

    /* Test */
    for (int i = 0; i < num_tests; i++) {
        arraylist_map(tests[i].result, map_fn),
        assert_arraylist(tests[i].result, tests[i].expected);
    }

    /* Free */
    for (int i = 0; i < num_tests; i++) {
        arraylist_free(tests[i].result);
        arraylist_free(tests[i].expected);
    }
}

const UnitTest TESTS[] = {
    { test_arraylist_init, "test_arraylist_init" },
    { test_arraylist_empty, "test_arraylist_empty" },
    { test_arraylist_clear, "test_arraylist_clear" },
    { test_arraylist_get, "test_arraylist_get" },
    { test_arraylist_pop, "test_arraylist_pop" },
    { test_arraylist_set, "test_arraylist_set" },
    { test_arraylist_push, "test_arraylist_push" },
    { test_arraylist_length, "test_arraylist_length" },
    { test_arraylist_capacity, "test_arraylist_capacity" },
    { test_arraylist_reserve, "test_arraylist_reserve" },
    { test_arraylist_resize, "test_arraylist_resize" },
    { test_arraylist_foreach, "test_arraylist_foreach" },
    { test_arraylist_map, "test_arraylist_map" },
};

const int NUM_TESTS = sizeof(TESTS) / sizeof(TESTS[0]);

int main(int argc, char *argv[]) {
    for (int i = 0; i < NUM_TESTS; i++) {
        TESTS[i].fn();
        printf("+ PASSED %s\n", TESTS[i].name);
    }
}