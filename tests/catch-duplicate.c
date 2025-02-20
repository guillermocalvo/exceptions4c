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
 * Duplicate CATCH blocks should have no effect.
 */
int main(void) {
    volatile bool caught1 = false, caught2 = false; /* NOSONAR */

    TRY {
        THROW(OOPS, NULL);
    } CATCH (OOPS) {
        caught1 = true;
    } CATCH (OOPS) {
        caught2 = true;
    }

    TEST_ASSERT(caught1);
    TEST_ASSERT(!caught2);
    TEST_PASS;
}
