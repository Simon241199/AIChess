#pragma once
#include <Board.h>
#include <Actor.h>

void askCompatibility();

std::pair<std::unique_ptr<Core::Actor>, std::unique_ptr<Core::Actor>> getMatchUp();

void display(const Core::Board& b);

Core::Move getUserMove(const Core::Board& b);
