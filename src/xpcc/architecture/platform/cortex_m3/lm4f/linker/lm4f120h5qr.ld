/*
 *                           LINKER  SCRIPT
 *
 * The Linker Script defines how the code and data emitted by the GNU C compiler
 * and assembler are  to be loaded into memory (code goes into ROM, variables
 * go into RAM).
 * 
 * Any symbols defined in the Linker Script are automatically global and
 * available to the rest of the program.
 */
 
 /* ---- May move to separate file */
 
MEMORY
{
	ROM (rx)      : ORIGIN = 0x00000000, LENGTH = 0x00040000
    RAM (rwx)     : ORIGIN = 0x20000000, LENGTH = 0x00008000
}

INCLUDE ../../common/memory_sections.ld