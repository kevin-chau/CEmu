/*
 * Copyright (c) 2015-2021 CE Programming.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CEMUCORE_CPU_H
#define CEMUCORE_CPU_H

#include "compiler.h"

#include <stdbool.h>
#include <stdint.h>

#if CEMUCORE_BYTE_ORDER == CEMUCORE_ORDER_LITTLE_ENDIAN
# define CEMUCORE_ORDER_SWAP2(a0, a1) a0; a1
#elif CEMUCORE_BYTE_ORDER == CEMUCORE_ORDER_BIG_ENDIAN
# define CEMUCORE_ORDER_SWAP2(a0, a1) a1; a0
#else
# error "Unknown endianness"
#endif
# define CEMUCORE_ORDER_SWAP4(a0, a1,           \
                              a2, a3)           \
    CEMUCORE_ORDER_SWAP2(                       \
        CEMUCORE_ORDER_SWAP2(a0, a1),           \
        CEMUCORE_ORDER_SWAP2(a2, a3))
# define CEMUCORE_ORDER_SWAP8(a0, a1, a2, a3,   \
                              a4, a5, a6, a7)   \
    CEMUCORE_ORDER_SWAP2(                       \
        CEMUCORE_ORDER_SWAP4(a0, a1, a2, a3),   \
        CEMUCORE_ORDER_SWAP4(a4, a5, a6, a7))

typedef struct regs
{
#define CEMUCORE_REGAF(p)                       \
    union                                       \
    {                                           \
        struct                                  \
        {                                       \
            CEMUCORE_ORDER_SWAP2(               \
                uint8_t : 8,                    \
                CEMUCORE_ORDER_SWAP8(           \
                    bool p##sf : 1,             \
                    bool p##zf : 1,             \
                    bool p##yf : 1,             \
                    bool p##hc : 1,             \
                    bool p##xf : 1,             \
                    bool p##pv : 1,             \
                    bool p##nf : 1,             \
                    bool p##cf : 1));           \
        };                                      \
        struct                                  \
        {                                       \
            CEMUCORE_ORDER_SWAP2(               \
                uint8_t p##a,                   \
                uint8_t p##f);                  \
        };                                      \
        uint16_t p##af;                         \
    };
#define CEMUCORE_REG24(hl, h, l)                \
    union                                       \
    {                                           \
        struct                                  \
        {                                       \
            CEMUCORE_ORDER_SWAP4(               \
                uint8_t : 8,                    \
                uint8_t hl##u,                  \
                uint8_t h,                      \
                uint8_t l);                     \
        };                                      \
        struct                                  \
        {                                       \
            CEMUCORE_ORDER_SWAP2(               \
                uint16_t : 16,                  \
                uint16_t hl);                   \
        };                                      \
        struct                                  \
        {                                       \
            CEMUCORE_ORDER_SWAP2(               \
                uint32_t : 8,                   \
                uint32_t u##hl : 24);           \
        };                                      \
    }
    CEMUCORE_REGAF()
    CEMUCORE_REGAF(shadow_)
    CEMUCORE_REG24(bc, b, c);
    CEMUCORE_REG24(de, d, e);
    CEMUCORE_REG24(hl, h, l);
    struct
    {
        CEMUCORE_REG24(bc, b, c);
        CEMUCORE_REG24(de, d, e);
        CEMUCORE_REG24(hl, h, l);
    } shadow;
    union
    {
        struct
        {
            CEMUCORE_REG24(ix, ixh, ixl);
            CEMUCORE_REG24(iy, iyh, iyl);
        };
        CEMUCORE_REG24(hl, h, l) index[2];
    };
    union
    {
        struct
        {
            uint16_t sps : 16, : 16;
            uint32_t spl : 24, : 8;
        };
        CEMUCORE_REG24(hl, h, l) sp[2];
    };
    uint32_t pc : 24, : 8;
    uint32_t rpc : 24, : 8;
    uint16_t i;
    uint8_t r, mb;
    bool adl : 1, madl : 1;
#undef CEMUCORE_REG24
#undef CEMUCORE_REGAF
} regs_t;

typedef struct cpu
{
    regs_t regs;
} cpu_t;

void cpu_init(cpu_t *cpu);
void cpu_destroy(cpu_t *cpu);

#endif