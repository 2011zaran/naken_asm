/**
 *  naken_asm assembler.
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2010-2013 by Michael Kohn
 *
 */

#ifndef _TABLE_POWERPC_H
#define _TABLE_POWERPC_H

#include "assembler.h"

enum
{
  OP_NONE,
};

struct _table_powerpc
{
  char *instr;
  unsigned int opcode;
  unsigned int mask;
  unsigned char type;
  unsigned char cycles;
  unsigned char cycles_max;
};

extern struct _table_powerpc table_powerpc[];

#endif


