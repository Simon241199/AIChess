#include "Player.h"
#include <CommandLineUI.h>

Core::Move Player::getMove(const Core::Board& b)
{
	return getUserMove(b);
}
