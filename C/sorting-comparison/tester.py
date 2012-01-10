#!/usr/bin/env python

import subprocess

goal = 1000000
step = 50000

for method in ["quicksort", "mergesort"]:
   i = 100000
   while i <= goal:
      print "%s: sorting %d" % (method, i)
      subprocess.call("./%s numbers.bin %d >> %s_results.txt" % (method, i, method), shell=True)
      i += step