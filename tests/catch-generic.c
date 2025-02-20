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

static const struct e4c_exception_type GENERIC = {NULL, "Generic exception"};
static const struct e4c_exception_type SPECIFIC = {&GENERIC, "Specific exception"};
static const struct e4c_exception_type DIFFERENT = {&GENERIC, "Different exception"};
static const struct e4c_exception_type MORE_SPECIFIC = {&SPECIFIC, "More specific exception"};

/**
 * Tests macro CATCH with generic exception types.
 */
int main(void) {
    volatile bool caught1 = false, caught2 = false, caught3 = false; /* NOSONAR */

    TRY {
        THROW(SPECIFIC, NULL);
    } CATCH (GENERIC) {
        caught1 = true;
    }

    TEST_ASSERT(caught1);

    TRY {
        TRY {
            THROW(SPECIFIC, NULL);
        } CATCH (DIFFERENT) {
            TEST_FAIL("Should not have caught the exception here\n");
        } FINALLY {
            THROW(MORE_SPECIFIC, NULL);
        }
    } CATCH (GENERIC) {
        caught2 = true;
    }

    TEST_ASSERT(caught2);

    TRY {
        TRY {
            THROW(SPECIFIC, NULL);
        } CATCH (MORE_SPECIFIC) {
            TEST_FAIL("Should not have caught the exception here\n");
        } FINALLY {
            THROW(MORE_SPECIFIC, NULL);
        }
    } CATCH (SPECIFIC) {
        caught3 = true;
    }

    TEST_ASSERT(caught3);
    TEST_PASS;
}
