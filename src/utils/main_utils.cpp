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

plug::Transform get_screen_transform(unsigned size_x, unsigned size_y) {
    return plug::Transform(Vec2d(0.5, 0.5),
                           Vec2d(50.0 / size_x, 50.0 / size_y));
}
