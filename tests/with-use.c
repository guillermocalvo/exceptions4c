/*
 * Copyright 2025 Guillermo Calvo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <exceptions4c.h>
#include "testing.h"

enum resource_status { UNINITIALIZED, INITIALIZED, USED, FINALIZED };

static const struct e4c_exception_type OOPS = {NULL, "Oops"};

#define FINALIZE(fail, status, expected_status, caught, thrown)                \
    0);                                                                        \
    TEST_ASSERT_INT_EQUALS(status, expected_status);                           \
    TEST_ASSERT_FALSE(caught);                                                 \
    if (thrown) {                                                              \
        TEST_ASSERT_NOT_NULL(e4c_get_exception());                             \
    } else {                                                                   \
        TEST_ASSERT_NULL(e4c_get_exception());                                 \
    }                                                                          \
    if (fail) {                                                                \
        THROW(OOPS, NULL);                                                     \
    }                                                                          \
    (void) (status = FINALIZED


/**
 * Tests macros WITH... USE.
 */
int main(void) {
    volatile enum resource_status status; /* NOSONAR */
    volatile bool caught; /* NOSONAR */

    /* Happy path */
    status = UNINITIALIZED;
    caught = false;
    WITH (FINALIZE(false, status, USED, caught, false)) {
        status = INITIALIZED;
    } USE (true) {
        TEST_ASSERT_INT_EQUALS(status, INITIALIZED);
        status = USED;
    } FINALLY {
        TEST_ASSERT_INT_EQUALS(status, FINALIZED);
    }
    TEST_ASSERT_INT_EQUALS(status, FINALIZED);

    /* Not used */
    status = UNINITIALIZED;
    caught = false;
    WITH (FINALIZE(false, status, INITIALIZED, caught, false)) {
        status = INITIALIZED;
    } USE (false) {
        TEST_FAIL("Reached %s:%d", __FILE__, __LINE__);
    } FINALLY {
        TEST_ASSERT_INT_EQUALS(status, FINALIZED);
    }
    TEST_ASSERT_INT_EQUALS(status, FINALIZED);

    /* Exception while acquiring the resource */
    status = UNINITIALIZED;
    caught = false;
    WITH (FINALIZE(false, status, UNINITIALIZED, caught, true)) {
        THROW(OOPS, NULL);
    } USE (true) {
        TEST_FAIL("Reached %s:%d", __FILE__, __LINE__);
    } CATCH_ALL {
        TEST_ASSERT_INT_EQUALS(status, UNINITIALIZED);
        caught = true;
    } FINALLY {
        TEST_ASSERT_INT_EQUALS(status, UNINITIALIZED);
        TEST_ASSERT_TRUE(caught);
    }
    TEST_ASSERT_INT_EQUALS(status, UNINITIALIZED);

    /* Exception while using the resource */
    status = UNINITIALIZED;
    caught = false;
    WITH (FINALIZE(false, status, USED, caught, true)) {
        status = INITIALIZED;
        TEST_ASSERT_FALSE(caught);
    } USE (true) {
        TEST_ASSERT_INT_EQUALS(status, INITIALIZED);
        status = USED;
        THROW(OOPS, NULL);
    } CATCH_ALL {
        TEST_ASSERT_INT_EQUALS(status, FINALIZED);
        caught = true;
    } FINALLY {
        TEST_ASSERT_INT_EQUALS(status, FINALIZED);
        TEST_ASSERT_TRUE(caught);
    }
    TEST_ASSERT_INT_EQUALS(status, FINALIZED);

    /* Exception while disposing of the resource */
    status = UNINITIALIZED;
    caught = false;
    WITH (FINALIZE(true, status, USED, caught, false)) {
        TEST_ASSERT_FALSE(caught);
        status = INITIALIZED;
    } USE (true) {
        TEST_ASSERT_INT_EQUALS(status, INITIALIZED);
        status = USED;
    } CATCH_ALL {
        TEST_ASSERT_INT_EQUALS(status, USED);
        caught = true;
    } FINALLY {
        TEST_ASSERT_INT_EQUALS(status, USED);
        TEST_ASSERT_TRUE(caught);
    }
    TEST_ASSERT_INT_EQUALS(status, USED);
    TEST_ASSERT_TRUE(caught);

    TEST_PASS;
}
