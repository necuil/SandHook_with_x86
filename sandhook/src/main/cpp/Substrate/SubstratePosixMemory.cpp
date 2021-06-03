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

#define SubstrateInternal
#include "CydiaSubstrate.h"
#include "SubstrateLog.hpp"

#include <sys/mman.h>

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/fcntl.h>
#include <syscall.h>

extern "C" void __clear_cache (void *beg, void *end);

struct __SubstrateMemory {
    void *address_;
    size_t width_;
    int prot_;

    __SubstrateMemory(void *address, size_t width, int prot) :
        address_(address),
        width_(width),
        prot_(prot)
    {
    }
};

static int MemelabAlignMapsMemory(void *addr, void *&outaddr, size_t &outsize, int &outprot);

extern "C" SubstrateMemoryRef SubstrateMemoryCreate(SubstrateAllocatorRef allocator, SubstrateProcessRef process, void *data, size_t size) {
    if (allocator != NULL) {
        MSLog(MSLogLevelError, "MS:Error:allocator != %d", 0);
        return NULL;
    }

    if (size == 0)
        return NULL;

    long page(sysconf(_SC_PAGESIZE)); // Portable applications should employ sysconf(_SC_PAGESIZE) instead of getpagesize

    uintptr_t base(reinterpret_cast<uintptr_t>(data) / page * page);
    size_t width(((reinterpret_cast<uintptr_t>(data) + size - 1) / page + 1) * page - base);
    void *address(reinterpret_cast<void *>(base));

    void *fixed_addr;
    size_t fixed_size;
    int fixed_prot;

    if (MemelabAlignMapsMemory(address, fixed_addr, fixed_size, fixed_prot) != 0) {
        MSLog(MSLogLevelError, "MS:Error:MemelabAlighMapsMemory(%p) => -1", address);
        fixed_addr = address;
        fixed_size = width;
        fixed_prot = PROT_READ | PROT_EXEC;
    }

    if (mprotect(fixed_addr, fixed_size, PROT_READ | PROT_WRITE | PROT_EXEC) == -1) {
        MSLog(MSLogLevelError, "MS:Error:mprotect() = %d", errno);
        return NULL;
    }

    return new __SubstrateMemory(fixed_addr, fixed_size, fixed_prot);
}

extern "C" void SubstrateMemoryRelease(SubstrateMemoryRef memory) {
    if (mprotect(memory->address_, memory->width_, memory->prot_) == -1)
        MSLog(MSLogLevelError, "MS:Error:mprotect() = %d", errno);

    __clear_cache(reinterpret_cast<char *>(memory->address_), reinterpret_cast<char *>(memory->address_) + memory->width_);

    delete memory;
}

static int MemelabAlignMapsMemory(void *addr, void *&outaddr, size_t &outsize, int &outprot) {
    auto laddr = reinterpret_cast<uintptr_t>(addr);
    auto maps_fd = syscall(SYS_openat, AT_FDCWD, "/proc/self/maps", O_RDONLY, 0);
    auto maps_f = fdopen(maps_fd, "r");

    char line[PATH_MAX] = {0};

    while (fgets(line, PATH_MAX, maps_f)) {
        uintptr_t start;
        uintptr_t end;
        char r;
        char w;
        char x;
        char p;
#       if defined(__LP64__)
        sscanf(line, "%lx-%lx %c%c%c%c", &start, &end, &r, &w, &x, &p);
#       else
        sscanf(line, "%x-%x %c%c%c%c", &start, &end, &r, &w, &x, &p);
#       endif
        if (start <= laddr && laddr < end) {
            outaddr = reinterpret_cast<void *>(start);
            outsize = end - start;
            outprot = PROT_NONE;
            if (r == 'r') {
                outprot |= PROT_READ;
            }
            if (w == 'w') {
                outprot |= PROT_WRITE;
            }
            if (x == 'x') {
                outprot |= PROT_EXEC;
            }
            return 0;
        }
    }
    return -1;
}