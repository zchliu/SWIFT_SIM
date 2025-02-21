# SWIFT Processor Simulation Repository

This repository is dedicated to simulating the self-designed SWIFT processor, built using the XiangShan toolchain. **SWIFT** stands for **Simple, Well-integrated, Intelligent, Fast, Tiny**. It adopts a 6-stage pipeline architecture and implements the RISC-V instruction set with the `rv32im_zicsr_zifencei` extensions. The processor features a separate L1 instruction cache (ICache) and data cache (DCache), ensuring efficient memory access. The repository also integrates NEMU difftest and Nanjing University's nanos operating system for comprehensive verification and compatibility testing.

## Project Overview

The SWIFT processor is a RISC-V architecture-based design focused on modular development, ensuring efficient and accurate simulation and verification. This project aims to:

- **6-Stage Pipeline RISC-V Processor**: Simulates a robust and optimized processor pipeline.
- **Processor Functional Verification**: Ensures correctness through differential testing (difftest) with NEMU.
- **Operating System Support**: Validates compatibility with multi-level software using the nanos operating system.

## Repository Structure



```
├── doc
├── README.md
├── simulator
│   ├── IP
│   ├── Makefile
│   ├── nemu
│   ├── script
│   ├── sim
│   └── testcases
└── software
    ├── base-port
    ├── device-test
    ├── functest
    ├── os
    ├── os-app
    ├── picotest
```

### `simulator`
Main directory for simulator-related files, including its core functionality and dependencies:
- **`IP`**  
  Contains hardware IP core-related code or resources used in the simulator.  
- **`Makefile`**  
  A build script for compiling and building simulator-related code.  
- **`nemu`**  
  Includes code or modules related to NEMU (an open-source emulator by Nanjing University), used for instruction set simulation or running programs.  
- **`script`**  
  Contains scripts for tasks like automated testing, deployment, or running the simulator.  
- **`sim`**  
  Holds the core simulator code or executables for performing simulation tasks.  
- **`testcases`**  
  Stores test cases, typically used to validate the functionality and performance of the simulator.  

### `software`
Contains directories for software development and testing, with multiple submodules:
- **`base-port`**  
  Provides low-level interface code or base implementations for hardware communication.  
- **`device-test`**  
  Includes test code and cases for device drivers, verifying interactions between devices and the simulator.  
- **`functest`**  
  Functional test code for checking if core features work as expected.  
- **`os`**  
  Source code for the operating system, likely for the nanos operating system.  
- **`os-app`**  
  Operating system application code, possibly including examples of programs running on the nanos system.  
- **`picotest`**  
  Lightweight test cases for quickly verifying specific features or modules with minimal overhead.  

## Prerequisites

The following tools are required to work with this repository:

### Tools
1. **Verilator**: A fast and free Verilog simulator.
   The version of Verilator installed via APT is too old and does not support SystemVerilog. You need to compile and install the latest version from source.  
   Installation guide: [https://verilator.org/guide/latest/install.html](https://verilator.org/guide/latest/install.html)

2. **GTKWave**: A waveform viewer for signal tracing.  
   Installation guide: [http://gtkwave.sourceforge.net/](http://gtkwave.sourceforge.net/)

3. **RISC-V Toolchain**: `riscv-unknown-linux-gnu` compiler for building RISC-V binaries.  
   Installation guide: [https://github.com/riscv-collab/riscv-gnu-toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain)

4. **x86 GCC**: GCC compiler for x86 platforms.  
   Installation guide: [https://gcc.gnu.org/install/](https://gcc.gnu.org/install/)

### Libraries

To run simulations, the following libraries need to be installed:
```bash
sudo apt install gcc g++ gdb
sudo apt install make
sudo apt install git
sudo apt install llvm-11 llvm-11-dev
sudo apt install man
sudo apt install libsdl2-dev
sudo apt install libreadline-dev
```

## Quick Start

1. Clone the repository:
   ```bash
   git clone https://github.com/zchliu/SWIFT_SIM
   cd SWIFT_SIM
   ```

2. Run functest

    This command will execute all functions under the functest/src directory and compare them using the difftest library to verify the correctness of the results.
   ```bash
   cd software/functest
   make run
   ```
    If you want to test a specific program individually, explicitly specify it using the NAMES parameter
   ```bash
   make run NAMES=add
   ```
    The simulation program runs in batch mode by default. If you want to enter debugging mode, set the ARGS to empty in the command line.
    ```bash
   make run NAMES=add ARGS=
   ```

3. Run OS

    
    Running the following command will compile the nanos operating system and generate a .bin executable file in the os/build directory for the simulator to run the simulation.
   ```bash
   cd software/os
   make run
   ```


### License

This project is licensed under the MIT License.

