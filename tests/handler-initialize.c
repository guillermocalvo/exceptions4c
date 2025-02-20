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

static void custom_initializer(struct e4c_exception *exception);

static int my_custom_data = 123;
static const struct e4c_exception_type OOPS = {NULL, "Oops"};

/**
 * Tests that exception initializers are called.
 */
int main(void) {
    int *data = NULL;
    e4c_get_context()->initialize_exception = &custom_initializer;

    TRY {
        THROW(OOPS, NULL);
    } CATCH (OOPS) {
        data = e4c_get_exception()->data;
    }

    TEST_ASSERT_NOT_NULL(data);
    TEST_ASSERT_PTR_EQUALS(data, &my_custom_data);
    TEST_PASS;
}

static void custom_initializer(struct e4c_exception *exception) {
    exception->data = &my_custom_data;
}
