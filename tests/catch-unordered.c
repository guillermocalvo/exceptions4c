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

static const struct e4c_exception_type GENERIC = {NULL, "Generic error"};
static const struct e4c_exception_type SPECIFIC = {&GENERIC, "Specific error"};

/**
 * Specific CATCH blocks should go before generic ones.
 */
int main(void) {
    volatile bool caught1 = false, caught2 = false; /* NOSONAR */
    TRY {
        THROW(SPECIFIC, NULL);
    } CATCH (GENERIC) {
        caught1 = true;
    } CATCH (SPECIFIC) {
        caught2 = true;
    }
    TEST_ASSERT(caught1);
    TEST_ASSERT(!caught2);
    TEST_PASS;
}
