#include "Test.h"
#include "CommandLineUI.h"

#include <Board.h>
#include <Utility.h>

#include <iostream>

namespace Test {

	void runAll()
	{
		int i = 0;
		i += moveGeneration1();
		i += moveGeneration2();
		i += moveGeneration3();
		i += moveGeneration4();
		i += moveGeneration5();
		i += moveGeneration6();
		std::cout << i << "/6 successful" << std::endl;
	}

	bool moveGeneration1()
	{
		Core::Board b;

		int moveCount = Core::countMoves(b, 4);
		if (moveCount != 197281) {
			std::cout << "moveGeneration1 Failed" << std::endl;
			return false;
		}
		return true;
	}

	bool moveGeneration2()
	{
		Core::Board b("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");

		int moveCount = Core::countMoves(b, 4);
		if (moveCount != 4085603) {
			std::cout << "moveGeneration2 Failed" << std::endl;
			return false;
		}
		return true;
	}
	bool moveGeneration3()
	{
		Core::Board b("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");

		int moveCount = Core::countMoves(b, 4);
		if (moveCount != 43238) {
			std::cout << "moveGeneration3 Failed" << std::endl;
			return false;
		}
		return true;
	}

	bool moveGeneration4()
	{
		Core::Board b("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

		int moveCount = Core::countMoves(b, 4);
		if (moveCount != 422333) {
			std::cout << "moveGeneration4 Failed" << std::endl;
			return false;
		}
		return true;
	}

	bool moveGeneration5()
	{
		Core::Board b("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");

		int moveCount = Core::countMoves(b, 4);
		if (moveCount != 2103487) {
			std::cout << "moveGeneration5 Failed" << std::endl;
			return false;
		}
		return true;
	}

	bool moveGeneration6()
	{
		Core::Board b("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");

		int moveCount = Core::countMoves(b, 4);
		if (moveCount != 3894594) {
			std::cout << "moveGeneration6 Failed" << std::endl;
			return false;
		}
		return true;
	}
};