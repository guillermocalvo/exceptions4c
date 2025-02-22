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

#include <stdio.h>
#include <signal.h>
#include <exceptions4c.h>

const struct e4c_exception_type MY_ERROR = {NULL, "My error"};

//! [uncaught_handler]
static void my_uncaught_handler(const struct e4c_exception * exception) {
  fprintf(stderr, "UNCAUGHT: %s\n", exception->message);
}

int main(int argc, char * argv[]) {
  e4c_get_context()->uncaught_handler = my_uncaught_handler;
  THROW(MY_ERROR, "Oops");
}
//! [uncaught_handler]
