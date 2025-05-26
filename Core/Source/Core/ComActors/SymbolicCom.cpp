#include "SymbolicCom.h"

namespace Core {
    Move Core::SymbolicCom::getMove(const Board& b)
    {
        return b.getMoves()[0];
    }
};