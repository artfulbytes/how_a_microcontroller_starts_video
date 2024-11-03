#include <string.h>

__attribute((constructor))
static void function_called_before_main(void) {
    while(1);
}

int global_variable = 10;
int uninit_global_variable; // Expect 0 according to C standard
int from[100];
int to[100];
int main(void) {
    memcpy(to, from, sizeof(from));
    int local_variable = 20;
    static int static_local_variable = 30;
    static int uninit_static_local_variable; // Expect 0 according to C standard
    static int sum; // Expect 0 according to C standard
    sum = global_variable + uninit_global_variable +
          local_variable + static_local_variable +
          uninit_static_local_variable;
    if (sum == 60) {
        while (1) { }
    } else {
        return 0;
    }
}
