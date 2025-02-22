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

static const struct e4c_exception_type CAUSE = {NULL, "Root cause"};
static const struct e4c_exception_type ERROR = {NULL, "Generic error"};
static const struct e4c_exception_type PROBLEM = {NULL, "Specific problem"};

/**
 * Tests that exceptions keep track of their root causes.
 */
int main(void) {
    TRY {
        TRY {
            THROW(CAUSE, NULL);
        } CATCH (CAUSE) {
            THROW(ERROR, NULL);
        }
    } CATCH (ERROR) {
        TEST_ASSERT_NOT_NULL(e4c_get_exception()->cause);
        TEST_ASSERT_PTR_EQUALS(e4c_get_exception()->cause->type, &CAUSE);
    }

    TRY {
        TRY {
            TRY {
                THROW(CAUSE, NULL);
            } CATCH (CAUSE) {
                THROW(ERROR, NULL);
            }
        } CATCH (ERROR) {
            THROW(PROBLEM, NULL);
        }
    } CATCH (PROBLEM) {
        const struct e4c_exception *exception = e4c_get_exception();
        TEST_ASSERT_NOT_NULL(exception->cause);
        TEST_ASSERT_PTR_EQUALS(exception->cause->type, &ERROR);
        TEST_ASSERT_NOT_NULL(exception->cause->cause);
        TEST_ASSERT_PTR_EQUALS(exception->cause->cause->type, &CAUSE);
    }

    TEST_PASS;
}
