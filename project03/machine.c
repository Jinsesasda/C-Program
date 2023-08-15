/* This program manipulates the instructions of N2 version of CPU. It contains
 * the 4 major functions which are print_instruction, disassemble, where_set,
 * and is_safe. The function print_instruction() prints information for each
 * given instruction. disassemble() converts machine language to assembly
 * language. where_set() detects whether any register in an assembly program
 * is used before it is given a value. is_safe() function checks if there
 * are any invalid instructions or parameters.
 * Each instruction will be a 32 bit (or 4 byte) word. One instruction word
 * consists of 6 different values which are opcode, extension, register1,
 * register2, register3, and memory address or constant values. Each value
 * takes certain number of bits:
 * 5 bits for opcode, 3 bits for extension, 3 bits for register1, 3 bits for
 * register2, 3 bits for register3, and finally 15 bits for memory address
 * or constant values.
 * Kinds of instructions that are invalid include:
 * an opcode value of 27 or more, using a register number of 7, using a
 * memory address that is not divisible by 4, modifying the special
 * unmodifiable program counter register R6, an extension field value of
 * 5 or more. */

#include <stdio.h>
#include "machine.h"

static unsigned short first_reg(Wrd instruction, unsigned short reg_nbr);
static unsigned short *valid_instr(Wrd instruction, unsigned short *instr);

/* This function prints information for each given instruction as long as
 * it is not an invalid one. To check whether the parameter instruction is
 * valid, it calls the valid_instr() function inside this function. This
 * function also returns 1 if everything is printed properly and 0 if there
 * were any invalid instructions.
 */
unsigned short print_instruction(Wrd instruction) {
  unsigned short *arr, instr[6];
  Op_code opcode_value;
  unsigned int ext_value, reg1_value, reg2_value, reg3_value, addr_value;

  /* The function valid_instr() checks whether the given instruction is
   * invalid and returns unsigned short array with the size of 6. The
   * returned array will contain the opcode value in its first element,
   * extension value in its second element, etc. */
  arr = valid_instr(instruction, instr);

  /* valid_instr() function also sets the first element = 27 if there
   * were any invalid instructions. */
  if (instr[0] == 27)
    return 0;

  opcode_value = arr[0];
  /* This switch statement prints the proper opcode name according to
   * the opcode chart (0 == plus, 1 == minus, 2 == times, etc). */
  switch (opcode_value) {
  case PLUS:
    printf("%c%c%c%-4c", 'p', 'l', 'u', 's');
    break;
  case MINUS:
    printf("%c%c%c%c%-3c", 'm', 'i', 'n', 'u', 's');
    break;
  case TIMES:
    printf("%c%c%c%c%-3c", 't', 'i', 'm', 'e', 's');
    break;
  case DIV:
    printf("%c%c%-5c", 'd', 'i', 'v');
    break;
  case MOD:
    printf("%c%c%-5c", 'm', 'o', 'd');
    break;
  case NEG:
    printf("%c%c%-5c", 'n', 'e', 'g');
    break;
  case ABS:
    printf("%c%c%-5c", 'a', 'b', 's');
    break;
  case SHL:
    printf("%c%c%-5c", 's', 'h', 'l');
    break;
  case SHR:
    printf("%c%c%-5c", 's', 'h', 'r');
    break;
  case BAND:
    printf("%c%c%c%-4c", 'b', 'a', 'n', 'd');
    break;
  case BOR:
    printf("%c%c%-5c", 'b', 'o', 'r');
    break;
  case BXOR:
    printf("%c%c%c%-4c", 'b', 'x', 'o', 'r');
    break;
  case BNEG:
    printf("%c%c%c%-4c", 'b', 'n', 'e', 'g');
    break;
  case AND:
    printf("%c%c%-5c", 'a', 'n', 'd');
    break;
  case OR:
    printf("%c%-6c", 'o', 'r');
    break;
  case NOT:
    printf("%c%c%-5c", 'n', 'o', 't');
    break;
  case EQ:
    printf("%c%-6c", 'e', 'q');
    break;
  case NEQ:
    printf("%c%c%-5c", 'n', 'e', 'q');
    break;
  case LE:
    printf("%c%-6c", 'l', 'e');
    break;
  case LT:
    printf("%c%-6c", 'l', 't');
    break;
  case GE:
    printf("%c%-6c", 'g', 'e');
    break;
  case GT:
    printf("%c%-6c", 'g', 't');
    break;
  case MOVE:
    printf("%c%c%c%-4c", 'm', 'o', 'v', 'e');
    break;
  case LW:
    printf("%c%-6c", 'l', 'w');
    break;
  case SW:
    printf("%c%-6c", 's', 'w');
    break;
  case LI:
    printf("%c%-6c", 'l', 'i');
    break;
  case SYS:
    printf("%c%c%-5c", 's', 'y', 's');
    break;
  default:
    return 0;
  }

  /* If an extension value is 4, it halts the program without printing
   * anything after. It returns 1 becuase it is not considered invalid */
  ext_value = arr[1];
  if (opcode_value == SYS) {
    if (instr[1] == 4) {
      printf("%2d", ext_value);
      return 1;
    }
    printf("%2d ", ext_value);
  }

  reg1_value = arr[2];
  reg2_value = arr[3];
  reg3_value = arr[4];
  printf("R%d", reg1_value);

  if (opcode_value <= 22)
    printf(" R%d", reg2_value);

  /* If an opcode value is the following, it cannot have reg3_value. 
  * This is to prevent printing zeros. */
  if (opcode_value <= 14 && opcode_value != 5 && opcode_value != 6
       && opcode_value != 12)
    printf(" R%d", reg3_value);

  addr_value = arr[5];
  if (opcode_value > 15 && opcode_value < 26 && opcode_value != 22) {
    /* li instructions do not have to have leading zeros printed */
    if (opcode_value == 25)
      printf(" %d", addr_value);
    else {
      if (addr_value % 4 != 0)
	return 0;
      printf(" %05d", addr_value);
    }
  }

  return 1;
}

/* This function converts machine language to assembly language. Parameters
 * program is the array that contains all instructions, num_instrs is the
 * number of instructions (size of program), and bad_instr is a pointer that
 * stores the location of the first invalid instruction in program[]. It
 * returns 1 if everything works properly, and 0 if there was at least one
 * invalid instruction. */
unsigned short disassemble(const Wrd program[], unsigned short num_instrs,
			   unsigned short *const bad_instr) {
  unsigned short i, *arr, instr[6], starting_addr = 0;

  if (program == NULL || bad_instr == NULL || num_instrs > NUM_WORDS)
    return 0;

  for (i = 0; i < num_instrs; i++) {
    arr = valid_instr(program[i], instr);
    /* Again, if the first element of the array initialized by the
     * valid_instr() function is 27, the given instruction is invalid. */
    if (arr[0] == 27) {
      /* bad_instr then points to the location accordingly here. */
      *bad_instr = i;
      return 0;
    }
    printf("%04x: ", starting_addr);
    print_instruction(program[i]);
    printf("\n");
    starting_addr += 4;
  }

  return 1;
}

/* This function detects any register in an assembly program that is used
 * before it is given a value, which may be a bug. The parameters program
 * contains all instructions, num_words is the number of instructions in
 * the array, and reg_nbr is the register number. It should return -1 if
 * program is NULL, or num_words is larger than the number of 4-byte words,
 * or reg_nbr is not given a value for any instructions. If everything
 * is executed properly, it will return the index of program[] that has
 * the first instruction that matches with reg_nbr.
 * */
short where_set(const Wrd program[], unsigned short num_words,
		unsigned short reg_nbr) {
  unsigned int opcode, ext_value;
  unsigned short i;
  short ext_mask;

  if (program == NULL || num_words > NUM_WORDS || reg_nbr > 5)
    return -1;

  /* This for loop checks every instruction in the array program. */
  for (i = 0; i < sizeof(program); i++) {
    opcode = program[i] >> 27;
    if (opcode > 26)
      return -1;
    /* The function first_reg() returns 1 if the current instruction has
     * the same registration value as reg_nbr. */
    if (first_reg(program[i], reg_nbr) == 1) {
      /* It will first check whether it has an appropriate opcode value. */
      if (opcode < 16 || opcode == 22 || opcode == 23 || opcode == 25)
	return i;
      /* SYS instruction is separate because it depends on its extension
       * value (only 1, 3, and 4 are valid). */
      if (opcode == 26) {
	ext_value = program[i] >> 24;
	for (ext_mask = 7; ext_mask >= 0; ext_mask--) {
	  if ((ext_mask & ext_value) == ext_mask) {
	    if (ext_mask > 4)
	      return -1;
	    ext_value = ext_mask;
	    ext_mask = -1;
	  }
	}
	if (ext_value != 0 && ext_value != 2)
	  return i;
	return -1;
      }
    }
  }

  return -1;
}

/* This function checks whether there are any invalid instructions
 * in the Wrd array program[]. pgm_size indicates the size of program.
 * bad_instr is a pointer that may be used to indicate the location
 * of the first invalid instruction. It will return 1 unless program
 * is NULL, or bad_instr is NULL, or pgm_size exceeds NUM_WORDS.
 * (NUM_WORDS is the maximum number of four-byte words that can be taken) */
unsigned short is_safe(const Wrd program[], unsigned short pgm_size,
		       unsigned short *const bad_instr) {
  unsigned short i, *arr, instr[6];

  if (program == NULL || bad_instr == NULL || pgm_size > NUM_WORDS)
    return 0;

  for (i = 0; i < pgm_size; i++) {
    arr = valid_instr(program[i], instr);
    if (arr[0] == 27) {
      *bad_instr = i;
      return 0;
    }
  }

  return 1;
}

/* This is a static function that is called by one of the major functions,
 * where_set(). It has parameters instruction and reg_nbr, which is a
 * registration number. It will use instruction to find its registration
 * number and then compare it with reg_nbr. It should return 0 if they
 * are not the same and 1 if they are the same. */
static unsigned short first_reg(Wrd instruction, unsigned short reg_nbr) {
  unsigned int reg1_value = instruction >> 21;
  short reg_mask;
  for (reg_mask = 7; reg_mask >= 0; reg_mask--) {
    if ((reg_mask & reg1_value) == reg_mask) {
      if (reg_mask == 7)
	return 0;
      reg1_value = reg_mask;
      reg_mask = -1;
    }
  }

  if (reg_nbr == reg1_value)
    return 1;

  return 0;
}

/* This is a static function that is called by three of the major functions,
 * print_instruction(), disassemble(), and is_safe(). It takes a pointer
 * instr that points to the caller's array which will store opcode, extension,
 * register 1, register 2, register 3, and addr/imme values. It will manipulate
 * the given instruction with bitwise operators to extract 6 different values
 * which are mentioned in the previous sentence. If any values of the
 * instruction are invalid, it will set the first element to 27 and return
 * instr, which will indicate that the given instruction is invalid. Otherwise,
 * it will set all the values correctly to the array and then return instr. */
static unsigned short *valid_instr(Wrd instruction, unsigned short *instr) {
  unsigned int opcode, ext, reg1, reg2, reg3, addr = 0;
  short ext_mask, reg_mask, bit_position;

  /* The first 5 bits holds the opcode value. */
  opcode = instruction >> 27;
  if (opcode > 26) {
    instr[0] = 27;
    return instr;
  }

  /* Using bit masking, it will compare up to 3 bits after opcode. */
  ext = instruction >> 24;
  for (ext_mask = 7; ext_mask >= 0; ext_mask--) {
    if ((ext_mask & ext) == ext_mask) {
      if (ext_mask > 4) {
	instr[0] = 27;
	return instr;
      }
      if (ext_mask == 4) {
	instr[0] = opcode;
	instr[1] = 4;
	return instr;
      }
      ext = ext_mask;
      ext_mask = -1;
    }
  }

  /* Using bit masking, it will compare up to 3 bits after extension. */
  reg1 = instruction >> 21;
  for (reg_mask = 7; reg_mask >= 0; reg_mask--) {
    if ((reg_mask & reg1) == reg_mask) {
      if (reg_mask == 7) {
	instr[0] = 27;
	return instr;
      }
      reg1 = reg_mask;
      reg_mask = -1;
    }
  }

  /* Advances 3 less bits to extract next register number. */
  reg2 = instruction >> 18;
  for (reg_mask = 7; reg_mask >= 0; reg_mask--) {
    if ((reg_mask & reg2) == reg_mask) {
      /* Any opcode values above 22 are not valid for reg2. */
      if (opcode > 22 && reg_mask != 0) {
	instr[0] = 27;
	return instr;
      }
      if (reg_mask == 7) {
	instr[0] = 27;
	return instr;
      }
      reg2 = reg_mask;
      reg_mask = -1;
    }
  }

  /* 3 less bits again to extract the next register number. */
  reg3 = instruction >> 15;
  for (reg_mask = 7; reg_mask >= 0; reg_mask--) {
    if ((reg_mask & reg3) == reg_mask) {
      /* The following opcodes cannot hold reg3 values. */
      if ((opcode > 14 || opcode == 5 || opcode == 6
	   || opcode == 12) && reg_mask != 0) {
	instr[0] = 27;
	return instr;
      }
      if (reg_mask == 7) {
	instr[0] = 27;
	return instr;
      }
      reg3 = reg_mask;
      reg_mask = -1;
    }
  }

  /* The following opcodes are the instructions that modify their first
   * register and it is not valid to hold register value of 6. */
  if ((opcode < 16 || opcode == 22 || opcode == 23 || opcode == 25)
      && reg1 == 6) {
    instr[0] = 27;
    return instr;
  }

  /* SYS instructions depend on their extension values. */
  if (opcode == 26 && reg1 == 6 && (ext == 0 || ext == 2)) {
    instr[0] = 27;
    return instr;
  }

  /* This for loop extracts addr/imme values. It keeps iterating and shifting
   * one bit and another until there are 15 bits. */
  for (bit_position = 1; bit_position <= 15; bit_position++) {
    if (bit_position == 1 && (instruction & 1) == 1)
      addr += 1;
    if (bit_position == 2 && (instruction & 1) == 1)
      addr += 2;
    if (bit_position == 3 && (instruction & 1) == 1)
      addr += 4;
    if (bit_position == 4 && (instruction & 1) == 1)
      addr += 8;
    if (bit_position == 5 && (instruction & 1) == 1)
      addr += 16;
    if (bit_position == 6 && (instruction & 1) == 1)
      addr += 32;
    if (bit_position == 7 && (instruction & 1) == 1)
      addr += 64;
    if (bit_position == 8 && (instruction & 1) == 1)
      addr += 128;
    if (bit_position == 9 && (instruction & 1) == 1)
      addr += 256;
    if (bit_position == 10 && (instruction & 1) == 1)
      addr += 512;
    if (bit_position == 11 && (instruction & 1) == 1)
      addr += 1024;
    if (bit_position == 12 && (instruction & 1) == 1)
      addr += 2048;
    if (bit_position == 13 && (instruction & 1) == 1)
      addr += 4096;
    if (bit_position == 14 && (instruction & 1) == 1)
      addr += 8192;
    if (bit_position == 15 && (instruction & 1) == 1)
      addr += 16384;
    instruction >>= 1;
    /* The following opcodes cannot hold any add/imm values. */
    if ((opcode < 16 || opcode == 22 || opcode == 26) && addr > 0) {
      instr[0] = 27;
      return instr;
    }
  }

  /* Finally it stores all the extracted values accordingly. */
  instr[0] = opcode;
  instr[1] = ext;
  instr[2] = reg1;
  instr[3] = reg2;
  instr[4] = reg3;
  instr[5] = addr;
  return instr;
}
