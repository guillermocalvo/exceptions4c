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

static void custom_finalizer(const struct e4c_exception *);

static volatile bool custom_handler_was_finalized = false;
static const struct e4c_exception_type OOPS = {NULL, "Oops"};

/**
 * Tests that exception finalizers are called.
 */
int main(void) {
    e4c_get_context()->finalize_exception = &custom_finalizer;

    TRY {
        THROW(OOPS, NULL);
    } CATCH (OOPS) {
        TEST_PRINT_OUT("Exception caught");
    }

    TEST_ASSERT(custom_handler_was_finalized);
    TEST_PASS;
}

static void custom_finalizer(const struct e4c_exception *exception) {
    (void) exception;
    custom_handler_was_finalized = true;
}
