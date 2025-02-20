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

static const struct e4c_exception_type OOPS = {NULL, "Oops"};

/**
 * Tests macro CATCH with a specific exception type.
 */
int main(void) {
    volatile bool caught = false; /* NOSONAR */

    TRY {
        THROW(OOPS, NULL);
    } CATCH (OOPS) {
        caught = true;
        const struct e4c_exception * exception = e4c_get_exception();
        TEST_ASSERT_NOT_NULL(exception);
#ifndef NDEBUG
        TEST_ASSERT_STR_CONTAINS(exception->debug.file, "catch-specific.c");
        TEST_ASSERT_INT_EQUALS(exception->debug.line, 29);
        TEST_ASSERT_STR_EQUALS(exception->debug.function, "main");
#else
        TEST_ASSERT_NULL(exception->debug.file);
        TEST_ASSERT_INT_EQUALS(exception->debug.line, 0);
        TEST_ASSERT_NULL(exception->debug.function);
#endif
    }

    TEST_ASSERT(caught);
    TEST_PASS;
}
