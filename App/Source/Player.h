#pragma once
#include "Actor.h"

class Player :
    public Core::Actor
{
    Core::Move getMove(const Core::Board& b) override;
};

