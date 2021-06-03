//
// Created by SwiftGan on 2019/2/11.
//

#if defined(__i386__)

#include <hde64.h>
#include "../includes/inst.h"
#include "../includes/trampoline.h"

namespace SandHook {

    class InstI386 : public Inst {

    public:

        hde64s* hde64S;

        InstI386(hde64s* hde64S) {
            this->hde64S = hde64S;
        }

    private:

        int instLen() const override {
            return hde64S->len;
        }

        InstArch instArch() const override {
            return X86;
        }

        bool pcRelated() override {
            return (hde64S->flags & F_RELATIVE) == F_RELATIVE;
        }

    };

    void InstDecode::decode(void *codeStart, Size codeLen, InstVisitor *visitor) {
        Size offset = 0;
        Inst *inst;
        hde64s hde64S;
        while (offset < codeLen) {
            hde64_disasm(codeStart, &hde64S);
            inst = new InstI386(&hde64S);
            if((hde64S.flags & F_ERROR) == F_ERROR){
                hde64S.len = 0xFF;
            }
            if (!visitor->visit(inst, offset, codeLen)) {
                delete inst;
                break;
            }
            offset += inst->instLen();
            delete inst;
        }
    }
}

#endif