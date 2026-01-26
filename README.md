# Submit jobs to generate UrQMD data

`version`: U4 1.2 - CEE

`author`: Yige Huang

`Date`: 26.01.2026

## Beschriebung

This program will:

1. Generate unique random seeds for each job;

2. Submit jobs to run UrQMD and collect outputs;

3. Convert plain text outputs to ROOT files;

    * for CEE simulation, we keep `ftn14` and won't convert;

4. Clean up raw outputs to save disk space.

## Benutzung

1. Set collision options in `input.txt`. The meaning of each option can be found in the UrQMD documentation;

2. Set job options in `conf.py`. You need to configure:

    1. Number of jobs `nJobs`, and number of events for each job `nEventsPerJob`;

    2. The path to UrQMD executable and the table (to save time). **Note**: For the `U4` series of `GenQmdTree`, [UrQMD version 4.0](https://itp.uni-frankfurt.de/~bleicher/urqmddownload/urqmd-4.0.tar.gz) is required;

    3. The path to the master random seed list (to avoid duplicate seeds);

    4. Output format tag: `root` / `ROOT` / `r` / `R`, or `cee` / `CEE` / `c` / `C`, or `both` / `BOTH` / `b` / `B`;

3. Submit jobs using the manager system with `python3 manager.py submit`;

    * In CEE farm, you need to install `python3` yourself in case root file is needed.

## Änderungsprotokoll

26.01.2026 `U4 v1.1` - Yige Huang

* Add an option to save `ftn14`

22.01.2026 `U4 v1.1` - Yige Huang

* First complete verson for UrQMD 4.0 (dedicated for CEE farm).

12.12.2025 `U4 v1.0` - Yige Huang

* First complete verson for UrQMD 4.0.
