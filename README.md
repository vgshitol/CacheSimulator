# Cache Simulator

This project simulates a two-level cache hierarchy with optional Victim Cache support.
It reads memory traces and reports hit/miss metrics, swap behavior, writebacks, and memory traffic.

## Current Scope

- L1 cache with configurable block size, cache size, and associativity.
- Optional Victim Cache between L1 and L2.
- Optional L2 cache with configurable size and associativity.
- Simulation summary including L1/L2 miss rates, swaps, and writebacks.

## Scope for Improvement

- Add automated CI checks (build + validation script) for every pull request.
- Add unit-level tests for cache operations and edge-case configurations.
- Split core simulator logic from CLI for easier embedding as a library.
- Add standard benchmark traces and performance baselines for regression checks.
- Migrate configuration parsing to safer and more descriptive command-line flags.

## What Was Improved

- Added stronger build setup (`make`, `make run`, `make help`) via updated makefiles.
- Added basic input validation for invalid cache configurations.
- Added guard for L2 miss-rate divide-by-zero edge case.
- Added repository-level `Makefile` for easier root-level build commands.
- Added executable validation script at `scripts/validate.sh`.
- Added `.gitignore` for common build artifacts.

## Build

From project root:

```bash
make
```

To clean:

```bash
make clean
```

## Run

From project root:

```bash
make run
```

Or run directly:

```bash
./code/sim_cache 32 8192 4 7 65536 8 ./ValidationTraces/gcc_trace.txt
```

## Validate Against Reference Outputs

```bash
./scripts/validate.sh
```

The script rebuilds the simulator, runs standard validation configurations, and compares outputs to files in `ValidationResults/`.

## Output

Simulation output includes:

- Cache contents (L1, optional VC, optional L2)
- Read/write counters
- Miss rates
- Swaps and writebacks
- Total memory traffic
