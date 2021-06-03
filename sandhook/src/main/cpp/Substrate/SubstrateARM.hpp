/* Cydia Substrate - Powerful Code Insertion Platform
 * Copyright (C) 2008-2011  Jay Freeman (saurik)
*/

/* GNU Lesser General Public License, Version 3 {{{ */
/*
 * Substrate is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * Substrate is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Substrate.  If not, see <http://www.gnu.org/licenses/>.
**/
/* }}} */

#ifndef SUBSTRATE_ARM_HPP
#define SUBSTRATE_ARM_HPP

enum A$r {
    A$r0, A$r1, A$r2, A$r3,
    A$r4, A$r5, A$r6, A$r7,
    A$r8, A$r9, A$r10, A$r11,
    A$r12, A$r13, A$r14, A$r15,
    A$sp = A$r13,
    A$lr = A$r14,
    A$pc = A$r15
};

enum A$c {
    A$eq, A$ne, A$cs, A$cc,
    A$mi, A$pl, A$vs, A$vc,
    A$hi, A$ls, A$ge, A$lt,
    A$gt, A$le, A$al,
    A$hs = A$cs,
    A$lo = A$cc
};

#define _abs(im) abs((int)im)

#define A$mrs_rm_cpsr(rd) /* mrs rd, cpsr */ \
    (0xe10f0000 | ((rd) << 12))
#define A$msr_cpsr_f_rm(rm) /* msr cpsr_f, rm */ \
    (0xe128f000 | (rm))
#define A$ldr_rd_$rn_im$(rd, rn, im) /* ldr rd, [rn, #im] */ \
    (0xe5100000 | ((im) < 0 ? 0 : 1 << 23) | ((rn) << 16) | ((rd) << 12) | _abs(im))
#define A$str_rd_$rn_im$(rd, rn, im) /* sr rd, [rn, #im] */ \
    (0xe5000000 | ((im) < 0 ? 0 : 1 << 23) | ((rn) << 16) | ((rd) << 12) | _abs(im))
#define A$sub_rd_rn_$im(rd, rn, im) /* sub, rd, rn, #im */ \
    (0xe2400000 | ((rn) << 16) | ((rd) << 12) | (im & 0xff))
#define A$blx_rm(rm) /* blx rm */ \
    (0xe12fff30 | (rm))
#define A$mov_rd_rm(rd, rm) /* mov rd, rm */ \
    (0xe3a00000 | ((rd) << 12) | (rm))
#define A$ldmia_sp$_$rs$(rs) /* ldmia sp!, {rs} */ \
    (0xe8b00000 | (A$sp << 16) | (rs))
#define A$stmdb_sp$_$rs$(rs) /* stmdb sp!, {rs} */ \
    (0xe9200000 | (A$sp << 16) | (rs))
#define A$stmia_sp$_$r0$  0xe8ad0001 /* stmia sp!, {r0}   */
#define A$bx_r0           0xe12fff10 /* bx r0             */
#define A$bx_lr           0xe12fff1E /* bx lr             */
#define A$bx(rs)       /* bx rs */ \
    (0xe12fff10 | (rs))
#define A$push(rs)       /* push {rs} */ \
    (0xe52d0004 | ((rs) << 12))
#define A$pop(rs)        /* pop {rs} */ \
    (0xe49d0004 | ((rs) << 12))
#define A$pop_r0          0xe49d0004




#define T$Label(l, r) \
    (((r) - (l)) * 2 - 4 + ((l) % 2 == 0 ? 0 : 2))

#define T$pop_$r0$ 0xbc01 // pop {r0}
#define T$b(im) /* b im */ \
    (0xde00 | (im & 0xff))
#define T$blx(rm) /* blx rm */ \
    (0x4780 | (rm << 3))
#define T$bx(rm) /* bx rm */ \
    (0x4700 | (rm << 3))
#define T$nop /* nop */ \
    (0x46c0)

#define T$add_rd_rm(rd, rm) /* add rd, rm */ \
    (0x4400 | (((rd) & 0x8) >> 3 << 7) | (((rm) & 0x8) >> 3 << 6) | (((rm) & 0x7) << 3) | ((rd) & 0x7))
#define T$push_r(r) /* push r... */ \
    (0xb400 | (((r) & (1 << A$lr)) >> A$lr << 8) | ((r) & 0xff))
#define T$pop_r(r) /* pop r... */ \
    (0xbc00 | (((r) & (1 << A$pc)) >> A$pc << 8) | ((r) & 0xff))
#define T$mov_rd_rm(rd, rm) /* mov rd, rm */ \
    (0x4600 | (((rd) & 0x8) >> 3 << 7) | (((rm) & 0x8) >> 3 << 6) | (((rm) & 0x7) << 3) | ((rd) & 0x7))
#define T$ldr_rd_$rn_im_4$(rd, rn, im) /* ldr rd, [rn, #im * 4] */ \
    (0x6800 | (((im) & 0x1f) << 6) | ((rn) << 3) | (rd))
#define T$ldr_rd_$pc_im_4$(rd, im) /* ldr rd, [PC, #im * 4] */ \
    (0x4800 | ((rd) << 8) | ((im) & 0xff))
#define T$cmp_rn_$im(rn, im) /* cmp rn, #im */ \
    (0x2000 | ((rn) << 8) | ((im) & 0xff))
#define T$it$_cd(cd, ms) /* it<ms>, cd */ \
    (0xbf00 | ((cd) << 4) | (ms))
#define T$cbz$_rn_$im(op, rn, im) /* cb<op>z rn, #im */ \
    (0xb100 | ((op) << 11) | (((im) & 0x40) >> 6 << 9) | (((im) & 0x3e) >> 1 << 3) | (rn))
#define T$b$_$im(cond, im) /* b<cond> #im */ \
    (cond == A$al ? 0xe000 | (((im) >> 1) & 0x7ff) : 0xd000 | ((cond) << 8) | (((im) >> 1) & 0xff))

#define T1$ldr_rt_$rn_im$(rt, rn, im) /* ldr rt, [rn, #im] */ \
    (0xf850 | ((im < 0 ? 0 : 1) << 7) | (rn))
#define T2$ldr_rt_$rn_im$(rt, rn, im) /* ldr rt, [rn, #im] */ \
    (((rt) << 12) | _abs(im))

#define T1$mrs_rd_apsr(rd) /* mrs rd, apsr */ \
    (0xf3ef)
#define T2$mrs_rd_apsr(rd) /* mrs rd, apsr */ \
    (0x8000 | ((rd) << 8))

#define T1$msr_apsr_nzcvqg_rn(rn) /* msr apsr, rn */ \
    (0xf380 | (rn))
#define T2$msr_apsr_nzcvqg_rn(rn) /* msr apsr, rn */ \
    (0x8c00)
#define T$msr_apsr_nzcvqg_rn(rn) /* msr apsr, rn */ \
    (T2$msr_apsr_nzcvqg_rn(rn) << 16 | T1$msr_apsr_nzcvqg_rn(rn))

#endif//SUBSTRATE_ARM_HPP
