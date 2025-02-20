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

/**
 * Force library panic due to TRY without exception block.
 */
int main(void) {
    signal(SIGABRT, failure);

    (void) e4c_try(EXCEPTIONS4C_DEBUG);

    TEST_PRINT_ERR("Reached %s:%d\n", __FILE__, __LINE__);
    TEST_PASS;
}

static void failure(int _) {
    (void) _;
    TEST_FAIL("Handled SIGABORT %s:%d\n", __FILE__, __LINE__);
}
