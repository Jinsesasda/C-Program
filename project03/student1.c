#include <stdio.h>
#include "machine.h"

static unsigned short first_reg(Wrd instruction, unsigned short reg_nbr);
static unsigned short valid_instr(Wrd instruction);

unsigned short print_instruction(Wrd instruction) {
  Op_code opcode_value = instruction >> 27;
  unsigned int ext_value = instruction >> 24;
  short ext_mask;
  unsigned int reg1_value = instruction >> 21, reg2_value = instruction >> 18,
    reg3_value = instruction >> 15;
  short reg_mask;
  unsigned int addr_value = 0;
  unsigned int addr_temp = instruction;
  short bit_position;

  if (opcode_value > 26)
    return 0;

  switch (opcode_value) {
  case PLUS:
    printf("%c%c%c%-3c", 'p', 'l', 'u', 's');
    break;
  case MINUS:
    printf("%c%c%c%c%-2c", 'm', 'i', 'n', 'u', 's');
    break;
  case TIMES:
    printf("%c%c%c%c%-2c", 't', 'i', 'm', 'e', 's');
    break;
  case DIV:
    printf("%c%c%-4c", 'd', 'i', 'v');
    break;
  case MOD:
    printf("%c%c%-4c", 'm', 'o', 'd');
    break;
  case NEG:
    printf("%c%c%-4c", 'n', 'e', 'g');
    break;
  case ABS:
    printf("%c%c%-4c", 'a', 'b', 's');
    break;
  case SHL:
    printf("%c%c%-4c", 's', 'h', 'l');
    break;
  case SHR:
    printf("%c%c%-4c", 's', 'h', 'r');
    break;
  case BAND:
    printf("%c%c%c%-3c", 'b', 'a', 'n', 'd');
    break;
  case BOR:
    printf("%c%c%-4c", 'b', 'o', 'r');
    break;
  case BXOR:
    printf("%c%c%c%-3c", 'b', 'x', 'o', 'r');
    break;
  case BNEG:
    printf("%c%c%c%-3c", 'b', 'n', 'e', 'g');
    break;
  case AND:
    printf("%c%c%-4c", 'a', 'n', 'd');
    break;
  case OR:
    printf("%c%-5c", 'o', 'r');
    break;
  case NOT:
    printf("%c%c%-4c", 'n', 'o', 't');
    break;
  case EQ:
    printf("%c%-5c", 'e', 'q');
    break;
  case NEQ:
    printf("%c%c%-4c", 'n', 'e', 'q');
    break;
  case LE:
    printf("%c%-5c", 'l', 'e');
    break;
  case LT:
    printf("%c%-5c", 'l', 't');
    break;
  case GE:
    printf("%c%-5c", 'g', 'e');
    break;
  case GT:
    printf("%c%-5c", 'g', 't');
    break;
  case MOVE:
    printf("%c%c%c%-3c", 'm', 'o', 'v', 'e');
    break;
  case LW:
    printf("%c%-5c", 'l', 'w');
    break;
  case SW:
    printf("%c%-5c", 's', 'w');
    break;
  case LI:
    printf("%c%-5c", 'l', 'i');
    break;
  case SYS:
    printf("%c%c%-4c", 's', 'y', 's');
    break;
  default:
    return 0;
  }

  for (ext_mask = 7; ext_mask >= 0; ext_mask--) {
    if ((ext_mask & ext_value) == ext_mask) {
      /* Any value of extension field above 4 is invalid and 4 is supposed to
       * halt the executing program. */
      if (ext_mask > 4)
	return 0;
      if (ext_mask == 4) {
	printf("%2d", ext_mask);
	return 1;
      }
      ext_value = ext_mask;
      ext_mask = -1;
    }
  }

  if (opcode_value == SYS)
    printf("%2d ", ext_value);

  /* For loops to find registration number. */
  for (reg_mask = 7; reg_mask >= 0; reg_mask--) {
    if ((reg_mask & reg1_value) == reg_mask) {
      if (reg_mask == 7)
	return 0;
      reg1_value = reg_mask;
      reg_mask = -1;
    }
  }
  for (reg_mask = 7; reg_mask >= 0; reg_mask--) {
    if ((reg_mask & reg2_value) == reg_mask) {
      if (opcode_value > 22 && reg_mask != 0)
	return 0;
      if (reg_mask == 7)
	return 0;
      reg2_value = reg_mask;
      reg_mask = -1;
    }
  }
  for (reg_mask = 7; reg_mask >= 0; reg_mask--) {
    if ((reg_mask & reg3_value) == reg_mask) {
      if ((opcode_value > 14 || opcode_value == 5 || opcode_value == 6
	   || opcode_value == 12) && reg_mask != 0)
	return 0;
      if (reg_mask == 7)
	return 0;
      reg3_value = reg_mask;
      reg_mask = -1;
    }
  }
  if (opcode_value < 27)
    printf("R%d", reg1_value);
  if (opcode_value <= 22)
    printf(" R%d", reg2_value);
  if (opcode_value <= 14 && opcode_value != 5 && opcode_value != 6
       && opcode_value != 12)
    printf(" R%d", reg3_value);

  /* A for loop to get address/immediate field value; it can only accept
   * up to 32767. */
  for (bit_position = 1; bit_position <= 15; bit_position++) {
    if (bit_position == 1 && (addr_temp & 1) == 1)
      addr_value += 1;
    if (bit_position == 2 && (addr_temp & 1) == 1)
      addr_value += 2;
    if (bit_position == 3 && (addr_temp & 1) == 1)
      addr_value += 4;
    if (bit_position == 4 && (addr_temp & 1) == 1)
      addr_value += 8;
    if (bit_position == 5 && (addr_temp & 1) == 1)
      addr_value += 16;
    if (bit_position == 6 && (addr_temp & 1) == 1)
      addr_value += 32;
    if (bit_position == 7 && (addr_temp & 1) == 1)
      addr_value += 64;
    if (bit_position == 8 && (addr_temp & 1) == 1)
      addr_value += 128;
    if (bit_position == 9 && (addr_temp & 1) == 1)
      addr_value += 256;
    if (bit_position == 10 && (addr_temp & 1) == 1)
      addr_value += 512;
    if (bit_position == 11 && (addr_temp & 1) == 1)
      addr_value += 1024;
    if (bit_position == 12 && (addr_temp & 1) == 1)
      addr_value += 2048;
    if (bit_position == 13 && (addr_temp & 1) == 1)
      addr_value += 4096;
    if (bit_position == 14 && (addr_temp & 1) == 1)
      addr_value += 8192;
    if (bit_position == 15 && (addr_temp & 1) == 1)
      addr_value += 16384;
    addr_temp >>= 1;
  }
  if (opcode_value > 15 && opcode_value < 26 && opcode_value != 22) {
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

unsigned short disassemble(const Wrd program[], unsigned short num_instrs,
			   unsigned short *const bad_instr) {
  unsigned short i, starting_addr = 0;
  if (program == NULL || bad_instr == NULL || num_instrs > NUM_WORDS)
    return 0;
  for (i = 0; i < num_instrs; i++) {
    if (valid_instr(program[i]) == 0) {
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

short where_set(const Wrd program[], unsigned short num_words,
		unsigned short reg_nbr) {
  unsigned int opcode, ext_value;
  unsigned short i;
  short ext_mask;
  if (program == NULL || num_words > NUM_WORDS || reg_nbr > 5)
    return -1;
  for (i = 0; i < sizeof(program); i++) {
    opcode = program[i] >> 27;
    if (opcode > 26)
      return -1;
    if (first_reg(program[i], reg_nbr) == 1) {
      if (opcode < 16 || opcode == 22 || opcode == 23 || opcode == 25)
	return i;
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

unsigned short is_safe(const Wrd program[], unsigned short pgm_size,
		       unsigned short *const bad_instr) {
  unsigned short i;
  if (program == NULL || bad_instr == NULL || pgm_size > NUM_WORDS)
    return 0;
  for (i = 0; i < pgm_size; i++) {
    if (valid_instr(program[i]) == 0) {
      *bad_instr = i;
      return 0;
    }
  }
  return 1;
}

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

static unsigned short valid_instr(Wrd instruction) {
  unsigned int opcode, ext, reg1, reg2, reg3;
  short ext_mask, reg_mask;
  opcode = instruction >> 27;
  if (opcode > 26)
    return 0;
  ext = instruction >> 24;
  for (ext_mask = 7; ext_mask >= 0; ext_mask--) {
    if ((ext_mask & ext) == ext_mask) {
      if (ext_mask > 4)
	return 0;
      ext = ext_mask;
      ext_mask = -1;
    }
  }
  reg1 = instruction >> 21;
  for (reg_mask = 7; reg_mask >= 0; reg_mask--) {
    if ((reg_mask & reg1) == reg_mask) {
      if (reg_mask == 7)
	return 0;
      reg1 = reg_mask;
      reg_mask = -1;
    }
  }
  reg2 = instruction >> 18;
  for (reg_mask = 7; reg_mask >= 0; reg_mask--) {
    if ((reg_mask & reg2) == reg_mask) {
      if (opcode > 22 && reg_mask != 0)
	return 0;
      if (reg_mask == 7)
	return 0;
      reg2 = reg_mask;
      reg_mask = -1;
    }
  }
  reg3 = instruction >> 15;
  for (reg_mask = 7; reg_mask >= 0; reg_mask--) {
    if ((reg_mask & reg3) == reg_mask) {
      if ((opcode > 14 || opcode == 5 || opcode == 6
	   || opcode == 12) && reg_mask != 0)
	return 0;
      if (reg_mask == 7)
	return 0;
      reg3 = reg_mask;
      reg_mask = -1;
    }
  }
  if ((opcode < 16 || opcode == 22 || opcode == 23 || opcode == 25)
      && reg1 == 6)
    return 0;
  if (opcode == 26 && reg1 == 6 && (ext == 0 || ext == 2))
    return 0;
  return 1;
}
