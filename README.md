# AIChess
This is a chess game with a computer opponent using AI to assess moves.
I am experimenting with two Kaggle datasets [1](https://www.kaggle.com/datasets/arevel/chess-games) [2](https://www.kaggle.com/datasets/ronakbadhe/chess-evaluations) to train neural networks.
For now, the only reasonable COM is the symbolic one.

## Plan
1. Refactor the common code in `SymbolicCom` and `NeuroSymbolicCom` to avoid duplication.
2. Implement a hybrid evaluation strategy for the neuro-symbolic COM, aiming to outperform the symbolic one.
3. Train new LibTorch neural networks that perform better than my existing ones.
4. Improve the mating capabilities of `SymbolicCom` by, among other things, adding checks and moves that evade check to the quiescence search.
5. Train a larger neural network for the subsymbolic COM that can ideally avoid hanging pieces without relying on alpha–beta pruning.

## Getting Started
1. Clone this repository (maybe in the future with --recurse-submodules)
2. Download the ([release](https://download.pytorch.org/libtorch/cu118/libtorch-win-shared-with-deps-2.7.0%2Bcu118.zip) | [debug](https://download.pytorch.org/libtorch/cu118/libtorch-win-shared-with-deps-debug-2.7.0%2Bcu118.zip)) archive and place its content as shown below.
   ```text
   AIChess
   ├── README.md                <-- You are here
   ├── Core
   │   ├── Source
   │   │   └── *
   │   ├── Vendor
   │   │   └── libtorch
   │   │       ├── debug        <-- Insert the content of the downloaded "libtorch" folder here
   │           │   ├── bin
   │           │   ├── include
   │           │   ├── lib
   │           │   └── ...
   │           └── release      <-- or here, depending on your desired build configuration
   │               ├── bin
   │               ├── include
   │               ├── lib
   │               └── ...
   └── ...
3. Generate your project files with Premake, or—if you’re on Windows and using Visual Studio—run `Setup-Windows.bat` from the `Scripts` folder.
4. If the `postbuildcommands` don’t work, it may be necessary to copy the required `.dll` files to the folder where the `.exe` is located. For me, the `postbuildcommands` work when I build the project a second time

## License
- UNLICENSE for this repository (see `UNLICENSE.txt` for more details)
- Premake is licensed under BSD 3-Clause (see included LICENSE.txt file for more details)