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

static const struct e4c_exception_type CAUSE = {NULL, "Cause"};
static const struct e4c_exception_type SUPPRESSED = {NULL, "Suppressed"};
static const struct e4c_exception_type OOPS = {NULL, "Oops"};

static volatile bool is_finalized = false;

/**
 * Tests that exceptions may be suppressed.
 */
int main(void) {
    volatile bool caught1 = false, caught2 = false; /* NOSONAR */

    e4c_get_context()->finalize_exception = &custom_finalizer;

    TRY {
        TRY {
            TRY {
                THROW(CAUSE, NULL);
            } FINALLY {
                THROW(SUPPRESSED, NULL);
            }
        } CATCH (SUPPRESSED) {
            caught1 = true;
            TEST_ASSERT(e4c_get_exception()->cause != NULL && e4c_get_exception()->cause->type == &CAUSE);
        } FINALLY {
            THROW(OOPS, NULL);
        }
    } CATCH (OOPS) {
        caught2 = true;
        TEST_ASSERT(e4c_get_exception()->cause == NULL);
    }

    TEST_ASSERT(caught1);
    TEST_ASSERT(caught2);
    TEST_ASSERT(is_finalized);
    TEST_PASS;
}

static void custom_finalizer(const struct e4c_exception *exception) {
    if (!is_finalized) {
        is_finalized = exception->type == &SUPPRESSED;
    }
}
