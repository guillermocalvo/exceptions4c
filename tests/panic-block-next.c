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

static void failure(int);

static struct e4c_context *my_supplier(void);

static struct e4c_context my_context = {0};

/**
 * Force library panic due to null block on next stage.
 */
int main(void) {
    signal(SIGABRT, failure);

    e4c_set_context_supplier(&my_supplier);

    (void) e4c_next(EXCEPTIONS4C_DEBUG);

    TEST_PRINT_ERR("Reached %s:%d\n", __FILE__, __LINE__);
    TEST_PASS;
}

static struct e4c_context *my_supplier(void) {
    free(my_context._innermost_block);
    my_context._innermost_block = NULL;
    return &my_context;
}

static void failure(int _) {
    (void) _;
    TEST_FAIL("Handled SIGABORT %s:%d\n", __FILE__, __LINE__);
}
