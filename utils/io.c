#include "io.h"

static char IO_BUF[IO_BUFFER_LENGTH] = {0};
static int cursor = 0;
static io_flush_handler HANDLERS[IO_FLUSH_HANDLER_N] = { NULL};
static int handler_n = 0;

void io_register_flush_handler( io_flush_handler handler) {
    HANDLERS[handler_n++] = handler;
}

void _io_flush() {
    for (int h = 0; h < handler_n; h ++) {
        HANDLERS[h](IO_BUF, cursor);
    }
    for (int c = 0; c < cursor; c ++) {
        IO_BUF[c] = 0;
    }
    cursor = 0;
}

void _io_handle_input(char c) {
    if (IO_CHAR_FLUSH == c) {
        _io_flush();
    } else {
        IO_BUF[cursor++] = c;
    }
}
