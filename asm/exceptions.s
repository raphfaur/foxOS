.section .text.exceptions
.extern __exception_gateway

.global _switcher_entry
.global exception_vector_table

exception_vector_table:
.org 0x0000


.org 0x0080


.org 0x0100


.org 0x0180


.org 0x0200


.org 0x0280


.org 0x0300
    timer_handler:
        B _switcher_entry

.org 0x0380


.org 0x0400


.org 0x0480


.org 0x0500


.org 0x0580


.org 0x0600


.org 0x0680


.org 0x0700
    timer_handler_0:
        B _switcher_entry

.org 0x0780
