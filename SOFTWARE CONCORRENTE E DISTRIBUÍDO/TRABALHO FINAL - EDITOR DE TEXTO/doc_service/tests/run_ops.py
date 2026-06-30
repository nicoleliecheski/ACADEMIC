#!/usr/bin/env python3
"""Run all unit/integration tests without requiring pytest.

`make test` uses pytest when available and falls back to this runner.
"""
import os
import sys
import traceback

HERE = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(os.path.join(HERE, "..")))

import test_ops          # noqa: E402
import test_replication  # noqa: E402

modules = [test_ops, test_replication]
total = 0
failures = 0
for mod in modules:
    fns = [getattr(mod, n) for n in dir(mod) if n.startswith("test_")]
    for fn in fns:
        total += 1
        try:
            fn()
            print(f"PASS {mod.__name__}.{fn.__name__}")
        except Exception:
            failures += 1
            print(f"FAIL {mod.__name__}.{fn.__name__}")
            traceback.print_exc()

print(f"\n{total - failures}/{total} passed")
sys.exit(1 if failures else 0)
