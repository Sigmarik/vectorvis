#include "main_utils.h"

#include <stdlib.h>

#include "lib/logger/logger.h"
#include "src/config.h"

void print_label() {
    puts(PROGRAM_NAME);
    printf("Build from\n%s %s\n", __DATE__, __TIME__);
    log_printf(ABSOLUTE_IMPORTANCE, "build info", "Build from %s %s.\n",
               __DATE__, __TIME__);
}

Mat33d get_screen_matrix(unsigned size_x, unsigned size_y) {
    // clang-format off
    return Mat33d(50.0 / size_x, 0.0, 0.5,
                  0.0, -50.0 / size_y, 0.5,
                  0.0, 0.0, 1.0);
    // clang-format on
}

void construct_gui(Panel& panel) {}
