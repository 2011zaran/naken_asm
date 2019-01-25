/**
 *  naken_asm assembler.
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2010-2019 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "disasm/xtensa.h"
#include "table/xtensa.h"

//#define READ_RAM(a) memory_read_m(memory, a)
//#define READ_RAM16(a) (memory_read_m(memory, a)<<8)|memory_read_m(memory, a+1)

int get_cycle_count_xtensa(unsigned short int opcode)
{
  return -1;
}

static int disasm_xtensa_le(struct _memory *memory, uint32_t address, char *instruction, int *cycles_min, int *cycles_max)
{
  uint32_t opcode, opcode16;
  int at, as, ar, ft, fs, fr, bt, bs, br, i;
  int n;

  opcode = memory_read_m(memory, address) |
          (memory_read_m(memory, address + 1) << 8) |
          (memory_read_m(memory, address + 2) << 16);

  opcode16 = memory_read_m(memory, address) |
            (memory_read_m(memory, address + 1) << 8);

  n = 0;

  while(table_xtensa[n].instr != NULL)
  {
    uint32_t mask = mask_xtensa[table_xtensa[n].type].mask_le;

    if (table_xtensa[n].bits == 24)
    {
      if ((opcode & mask) == table_xtensa[n].opcode_le)
      {
        switch(table_xtensa[n].type)
        {
          case XTENSA_OP_AR_AT:
            at = (opcode >> 4) & 0xf;
            ar = (opcode >> 12) & 0xf;
            sprintf(instruction, "%s a%d, a%d", table_xtensa[n].instr, ar, at);
            return 3;
          case XTENSA_OP_FR_FS:
            fs = (opcode >> 8) & 0xf;
            fr = (opcode >> 12) & 0xf;
            sprintf(instruction, "%s f%d, f%d", table_xtensa[n].instr, fr, fs);
            return 3;
          case XTENSA_OP_FR_FS_FT:
            ft = (opcode >> 4) & 0xf;
            fs = (opcode >> 8) & 0xf;
            fr = (opcode >> 12) & 0xf;
            sprintf(instruction, "%s f%d, f%d, f%d", table_xtensa[n].instr, fr, fs, ft);
            return 3;
          case XTENSA_OP_AR_AS_AT:
            at = (opcode >> 4) & 0xf;
            as = (opcode >> 8) & 0xf;
            ar = (opcode >> 12) & 0xf;
            sprintf(instruction, "%s a%d, a%d, a%d", table_xtensa[n].instr, ar, as, at);
            return 3;
          case XTENSA_OP_AT_AS_I8:
            at = (opcode >> 4) & 0xf;
            as = (opcode >> 8) & 0xf;
            i = (opcode >> 16) & 0xff;
            i = (int32_t)((int8_t)i);
            sprintf(instruction, "%s a%d, a%d, %d",
              table_xtensa[n].instr, at, as, i);
            return 3;
          case XTENSA_OP_AT_AS_IM8:
            at = (opcode >> 4) & 0xf;
            as = (opcode >> 8) & 0xf;
            i = ((opcode >> 16) & 0xff) << 8;
            i = (int32_t)((int16_t)i);
            sprintf(instruction, "%s a%d, a%d, %d",
              table_xtensa[n].instr, at, as, i);
            return 3;
          case XTENSA_OP_BT_BS4:
          case XTENSA_OP_BT_BS8:
            bt = (opcode >> 4) & 0xf;
            bs = (opcode >> 8) & 0xf;
            sprintf(instruction, "%s b%d, b%d", table_xtensa[n].instr, bt, bs);
            return 3;
          case XTENSA_OP_BR_BS_BT:
            bt = (opcode >> 4) & 0xf;
            bs = (opcode >> 8) & 0xf;
            br = (opcode >> 12) & 0xf;
            sprintf(instruction, "%s b%d, b%d, b%d", table_xtensa[n].instr, br, bs, bt);
            return 3;
          default:
            return -1;
        }
      }
    }
      else
    {
      if ((opcode16 & mask) == table_xtensa[n].opcode_le)
      {
        switch(table_xtensa[n].type)
        {
          case XTENSA_OP_N_AR_AS_AT:
            at = (opcode >> 4) & 0xf;
            as = (opcode >> 8) & 0xf;
            ar = (opcode >> 12) & 0xf;
            sprintf(instruction, "%s a%d, a%d, a%d", table_xtensa[n].instr, ar, as, at);
            return 2;
          case XTENSA_OP_N_AR_AS_I4:
            i = (opcode >> 4) & 0xf;
            as = (opcode >> 8) & 0xf;
            ar = (opcode >> 12) & 0xf;

            if (i == 0) { i = -1; }

            sprintf(instruction, "%s a%d, a%d, %d", table_xtensa[n].instr, ar, as, i);
            return 2;
          default:
            return -1;
        }
      }
    }

    n++;
  }

  sprintf(instruction, "???");

  return -1;
}

static int disasm_xtensa_be(struct _memory *memory, uint32_t address, char *instruction, int *cycles_min, int *cycles_max)
{
  uint32_t opcode, opcode16;
  int at, as, ar, ft, fs, fr, bt, bs, br, i;
  int n;

  opcode = memory_read_m(memory, address + 2) |
          (memory_read_m(memory, address + 1) << 8) |
          (memory_read_m(memory, address) << 16);

  opcode16 = memory_read_m(memory, address + 1) |
            (memory_read_m(memory, address) << 8);

  n = 0;

  while(table_xtensa[n].instr != NULL)
  {
    uint32_t mask = mask_xtensa[table_xtensa[n].type].mask_be;

    if (table_xtensa[n].bits == 24)
    {
      if ((opcode & mask) == table_xtensa[n].opcode_be)
      {
        switch(table_xtensa[n].type)
        {
          case XTENSA_OP_AR_AT:
            at = (opcode >> 16) & 0xf;
            ar = (opcode >> 8) & 0xf;
            sprintf(instruction, "%s a%d, a%d", table_xtensa[n].instr, ar, at);
            return 3;
          case XTENSA_OP_FR_FS:
            fs = (opcode >> 12) & 0xf;
            fr = (opcode >> 8) & 0xf;
            sprintf(instruction, "%s f%d, f%d", table_xtensa[n].instr, fr, fs);
            return 3;
          case XTENSA_OP_FR_FS_FT:
            ft = (opcode >> 16) & 0xf;
            fs = (opcode >> 12) & 0xf;
            fr = (opcode >> 8) & 0xf;
            sprintf(instruction, "%s f%d, f%d, f%d", table_xtensa[n].instr, fr, fs, ft);
            return 3;
          case XTENSA_OP_AR_AS_AT:
            at = (opcode >> 16) & 0xf;
            as = (opcode >> 12) & 0xf;
            ar = (opcode >> 8) & 0xf;
            sprintf(instruction, "%s a%d, a%d, %d", table_xtensa[n].instr, ar, as, at);
            return 3;
          case XTENSA_OP_AT_AS_I8:
            at = (opcode >> 16) & 0xf;
            as = (opcode >> 12) & 0xf;
            i = opcode & 0xff;
            i = (int32_t)((int8_t)i);
            sprintf(instruction, "%s a%d, a%d, %d",
            table_xtensa[n].instr, at, as, i);
            return 3;
          case XTENSA_OP_AT_AS_IM8:
            at = (opcode >> 16) & 0xf;
            as = (opcode >> 12) & 0xf;
            i = (opcode & 0xff) << 8;
            i = (int32_t)((int16_t)i);
            sprintf(instruction, "%s a%d, a%d, %d",
            table_xtensa[n].instr, at, as, i);
            return 3;
          case XTENSA_OP_BT_BS4:
          case XTENSA_OP_BT_BS8:
            bt = (opcode >> 16) & 0xf;
            bs = (opcode >> 12) & 0xf;
            sprintf(instruction, "%s b%d, b%d", table_xtensa[n].instr, bt, bs);
            return 3;
          case XTENSA_OP_BR_BS_BT:
            bt = (opcode >> 16) & 0xf;
            bs = (opcode >> 12) & 0xf;
            br = (opcode >> 8) & 0xf;
            sprintf(instruction, "%s b%d, b%d, b%d", table_xtensa[n].instr, br, bs, bt);
            return 3;
          default:
            return -1;
        }
      }
    }
      else
    {
      if ((opcode16 & mask) == table_xtensa[n].opcode_be)
      {
        switch(table_xtensa[n].type)
        {
          case XTENSA_OP_N_AR_AS_AT:
            at = (opcode >> 8) & 0xf;
            as = (opcode >> 4) & 0xf;
            ar = opcode & 0xf;
            sprintf(instruction, "%s a%d, a%d, %d", table_xtensa[n].instr, ar, as, at);
            return 2;
          case XTENSA_OP_N_AR_AS_I4:
            i = (opcode >> 8) & 0xf;
            as = (opcode >> 4) & 0xf;
            ar = opcode & 0xf;

            if (i == 0) { i = -1; }

            sprintf(instruction, "%s a%d, a%d, %d", table_xtensa[n].instr, ar, as, i);
            return 2;
          default:
            return -1;
        }
      }
    }

    n++;
  }

  sprintf(instruction, "???");

  return -1;
}

int disasm_xtensa(struct _memory *memory, uint32_t address, char *instruction, int *cycles_min, int *cycles_max)
{
  *cycles_min = -1;
  *cycles_max = -1;

  if (memory->endian == ENDIAN_LITTLE)
  {
    return disasm_xtensa_le(memory, address, instruction, cycles_min, cycles_max);
  }
    else
  {
    return disasm_xtensa_be(memory, address, instruction, cycles_min, cycles_max);
  }
}

static void get_bytes(struct _memory *memory, int address, int count, char *bytes)
{
  if (count == 2)
  {
    if (memory->endian == ENDIAN_LITTLE)
    {
      sprintf(bytes, "  %02x%02x",
        memory_read_m(memory, address + 1),
        memory_read_m(memory, address + 0));
    }
      else
    {
      sprintf(bytes, "  %02x%02x",
        memory_read_m(memory, address + 0),
        memory_read_m(memory, address + 1));
    }
  }
    else
  {
    if (memory->endian == ENDIAN_LITTLE)
    {
      sprintf(bytes, "%02x%02x%02x",
        memory_read_m(memory, address + 2),
        memory_read_m(memory, address + 1),
        memory_read_m(memory, address + 0));
    }
      else
    {
      sprintf(bytes, "%02x%02x%02x",
        memory_read_m(memory, address + 0),
        memory_read_m(memory, address + 1),
        memory_read_m(memory, address + 2));
    }
  }
}

void list_output_xtensa(struct _asm_context *asm_context, uint32_t start, uint32_t end)
{
  int cycles_min, cycles_max;
  char instruction[128];
  char bytes[10];
  int count;

  struct _memory *memory = &asm_context->memory;

  count = disasm_xtensa(memory, start, instruction, &cycles_min, &cycles_max);

  get_bytes(memory, start, count, bytes);

  fprintf(asm_context->list, "0x%04x: %s  %-40s", start, bytes, instruction);
  fprintf(asm_context->list, "\n");
}

void disasm_range_xtensa(struct _memory *memory, uint32_t flags, uint32_t start, uint32_t end)
{
  char instruction[128];
  char bytes[10];
  int cycles_min = 0, cycles_max = 0;
  int count;

  printf("\n");

  printf("%-7s %-5s %-40s Cycles\n", "Addr", "Opcode", "Instruction");
  printf("------- ------ ----------------------------------       ------\n");

  while(start <= end)
  {
    count = disasm_xtensa(memory, start, instruction, &cycles_min, &cycles_max);

    get_bytes(memory, start, count, bytes);

    printf("0x%04x: %s  %-40s\n", start, bytes, instruction);

    start += count;
  }
}
