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

static void custom_uncaught_handler(const struct e4c_exception *);

static const struct e4c_exception_type OOPS = {NULL, "Oops"};

/**
 * Tests that uncaught handlers are called.
 */
int main(void) {
    e4c_get_context()->uncaught_handler = &custom_uncaught_handler;
    THROW(OOPS, NULL);
}

static void custom_uncaught_handler(const struct e4c_exception *exception) {
    (void) exception;
    TEST_PRINT_OUT("Uncaught handler %s:%d\n", __FILE__, __LINE__);
    TEST_PASS;
}
