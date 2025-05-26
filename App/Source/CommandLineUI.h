#pragma once
#include "Core/Board.h"
#include <Core/Actor.h>

void askCompatibility();

std::pair<std::unique_ptr<Core::Actor>, std::unique_ptr<Core::Actor>> getMatchUp();

void display(const Core::Board& b);

Core::Move getUserMove(const Core::Board& b);
