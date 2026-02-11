# Distributed Computing Framework

## What is this project?

This project is a **distributed computing framework** implemented in **C++** for running computational workloads across multiple processes/nodes. It is designed around a **master-worker architecture** where a central coordinator distributes tasks and worker processes execute them in parallel.

It includes the core building blocks commonly used in distributed systems:
- task scheduling
- load balancing
- fault handling and retry support
- result collection/aggregation

## How it works

At a high level, execution follows this flow:

1. The **master node** receives or creates a batch of tasks.
2. Tasks are partitioned and assigned to **worker nodes**.
3. A **load balancer** helps distribute work to avoid hotspots.
4. Workers execute tasks and report results back to the master.
5. The **fault manager** monitors failures/timeouts and retries or reassigns failed work.
6. Final results are aggregated and returned.

Core implementation areas:
- `src/core/` for task, master, worker, and result lifecycle
- `src/load_balancing/` for balancing logic
- `src/fault_tolerance/` for failure handling
- `src/examples/` for runnable sample workloads

## Tech stack

- **Language:** C++17
- **Build system:** CMake
- **Parallel/distributed runtime:** MPI (OpenMPI/MPICH/MS-MPI depending on platform)
- **Tooling/scripts:** Bash and Batch scripts for easy build/run on Linux/Windows

## How to run

### 1) Build with CMake

```bash
cmake -S . -B build
cmake --build build
```

### 2) Run examples (MPI)

```bash
# Main framework demo (binary name can vary by generator/platform)
mpirun -np 4 ./distributed_framework_main

# Matrix multiplication example
mpirun -np 4 ./matrix_multiply 1000

# Monte Carlo Pi example
mpirun -np 8 ./monte_carlo_pi 100000000
```

> On Windows with MS-MPI, use `mpiexec` and `.exe` binaries.
 

## To-do list

- [ ] Add automated unit tests for core components (`task`, `master_node`, `worker_node`).
- [ ] Add integration tests that simulate worker failure and recovery.
- [ ] Add benchmark scripts and publish repeatable performance baselines.
- [ ] Add Docker-based local cluster setup for easier onboarding.
- [ ] Improve configuration docs for `config/framework.conf` options.
- [ ] Add architecture diagram showing message flow between master and workers.
- [ ] Add CI pipeline for build + test on Linux and Windows.

