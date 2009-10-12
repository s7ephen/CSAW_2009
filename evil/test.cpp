#include <stdio.h>

void foo() {
#include "evil.h" // note: can only include evil.h ONCE per function
printf("oh really!\n");
}

int main(void) {
printf("hi there\n");
#include "evil.h" // note: can only include evil.h ONCE per function
printf("good bye!\n");
foo();
}
