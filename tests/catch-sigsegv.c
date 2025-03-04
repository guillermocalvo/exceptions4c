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

#include <signal.h>
#include <exceptions4c.h>
#include "testing.h"

static void *null(int);

static void throw_on_signal(int);

static const struct e4c_exception_type SEGFAULT = {NULL, "Segmentation fault"};

static int integer = 123;

/**
 * Tests that signal SIGSEGV can be converted into a exception.
 */
int main(void) {
    volatile bool caught = false; /* NOSONAR */

    signal(SIGSEGV, throw_on_signal);

    TRY {
        const int *pointer = null(integer);
        integer = *pointer;
        TEST_FAIL("Reached %s:%d\n", __FILE__, __LINE__);
    } CATCH (SEGFAULT) {
        caught = true;
    }

    TEST_ASSERT(caught);
    TEST_PASS;
}

static void *null(const int dummy) {
    return dummy ? NULL : &integer;
}

static void throw_on_signal(int _) {
    (void) _;
    THROW(SEGFAULT, NULL);
}
