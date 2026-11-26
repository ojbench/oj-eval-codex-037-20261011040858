# Student record manager

Build with `cmake -S . -B build && cmake --build build`; the executable is
`build/code`. The judge's in-source build produces `code` in the project root.
Run `python3 tests/check.py` for the reference-model tests.

## Design

Students live in a vector, and an unordered map resolves names to vector indices.
A set of indices orders students by descending integer average, descending
lexicographic score array, and ascending name. An update removes its index from
the set before changing scores, then reinserts it, preserving the set invariant.

START and FLUSH copy the current ordering to a published vector and cache each
student's rank. UPDATE leaves this snapshot untouched, while PRINTLIST reads
live averages in snapshot order. Thus scores are immediately visible but ranks
change only on refresh. A refresh without intervening score changes does no work.

An update costs O(log n), a name lookup takes expected O(1), and a refresh or
list output costs O(n). Initial sorting costs O(n log n); storage is O(n).
This follows the statement's suggested set and cached-rank approach.

## Validation

- 100 deterministic randomized scenarios matched an independent sorting model.
- Cases cover integer-average ties, score and name tie breakers, duplicate names,
  rejected additions, missing students, stale ranks, and an empty roster.
- The same checks passed with UndefinedBehaviorSanitizer enabled.
- A 300,000-operation workload with 10,000 students and the documented operation
  frequencies completed in 3.33 seconds locally.
- A deliberately atypical workload alternating 145,000 updates and refreshes
  took 27.52 seconds. Full refreshes are linear, so this implementation relies on
  the documented low refresh frequency for maximum-size inputs.

The local sample-data directory named in README.md was not present.
