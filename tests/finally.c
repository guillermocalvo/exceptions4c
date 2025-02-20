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

static void aux1(void);

static void aux2(void);

static void aux3(void);

static void aux4(void);

static void aux5(void);

static const struct e4c_exception_type OOPS = {NULL, "Oops"};

static volatile bool finalized1 = false;
static volatile bool finalized2 = false;
static volatile bool finalized3 = false;

/**
 * Tests macro FINALLY.
 */
int main(void) {
    volatile bool caught = false; /* NOSONAR */
    TRY {
        aux1();
    } CATCH (OOPS) {
        caught = true;
    }
    TEST_ASSERT(caught);
    TEST_ASSERT(finalized1);
    TEST_ASSERT(finalized2);
    TEST_ASSERT(finalized3);
    TEST_PASS;
}

static void aux1(void) {
    aux2();
    exit(EXIT_FAILURE);
}

static void aux2(void) {
    TRY {
        aux3();
    } FINALLY {
        finalized1 = true;
    }
    TEST_FAIL("Reached %s:%d\n", __FILE__, __LINE__);
}

static void aux3(void) {
    aux4();
    TEST_FAIL("Reached %s:%d\n", __FILE__, __LINE__);
}

static void aux4(void) {
    TRY {
        aux5();
    } CATCH (OOPS) {
        THROW(OOPS, NULL);
    } FINALLY {
        finalized2 = true;
    }
    TEST_FAIL("Reached %s:%d\n", __FILE__, __LINE__);
}

static void aux5(void) {
    TRY {
        THROW(OOPS, NULL);
    } FINALLY {
        finalized3 = true;
    }
    TEST_FAIL("Reached %s:%d\n", __FILE__, __LINE__);
}
