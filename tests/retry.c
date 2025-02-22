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
static const struct e4c_exception_type GIVE_UP = {NULL, "Giving up"};

/**
 * Tests macro RETRY.
 */
int main(void) {
    volatile int total_tries = 0; /* NOSONAR */
    TRY {
        TRY {
            total_tries++;
            if (total_tries == 1) {
                TEST_PRINT_OUT("First try\n");
            } else {
                TEST_PRINT_OUT("retries: %d\n", total_tries - 1);
            }
            THROW(OOPS, NULL);
        } CATCH (OOPS) {
            RETRY(2, GIVE_UP, NULL);
        }
    } CATCH (GIVE_UP) {
        TEST_PRINT_OUT("total_tries: %d\n", total_tries);
    }
    TEST_ASSERT_INT_EQUALS(total_tries, 3);
    TEST_PASS;
}
