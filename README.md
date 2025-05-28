# AIChess
This is a chess game with a computer opponent using AI to assess moves.
I am using a [Kaggle dataset](https://www.kaggle.com/datasets/arevel/chess-games) to train the AIs.

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

## License
- UNLICENSE for this repository (see `UNLICENSE.txt` for more details)
- Premake is licensed under BSD 3-Clause (see included LICENSE.txt file for more details)