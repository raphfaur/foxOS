#include <stddef.h>

#define IO_BUFFER_LENGTH (1000)
#define IO_FLUSH_HANDLER_N (2)

#define IO_CHAR_FLUSH (0xd)

typedef void (*io_flush_handler) (char * data, int data_length);
void io_register_flush_handler( io_flush_handler handler);
void _io_handle_input(char c);
