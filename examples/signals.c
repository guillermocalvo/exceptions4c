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

//! [null_pointer]
#include <stdio.h>
#include <signal.h>
#include <exceptions4c.h>

const struct e4c_exception_type SEGFAULT = {NULL, "Segmentation fault"};

void segfault(int sigsegv) {
  signal(sigsegv, segfault);
  THROW(SEGFAULT, NULL);
}

int main(void) {
  const int * null_pointer = NULL;

  signal(SIGSEGV, segfault);

  TRY {
    printf("Oh no %d", *null_pointer);
  } CATCH (SEGFAULT) {
    printf("Danger avoided!\n");
  }

  return EXIT_SUCCESS;
}
//! [null_pointer]
