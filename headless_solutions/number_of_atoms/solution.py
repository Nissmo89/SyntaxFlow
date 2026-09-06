import collections
import functools
import itertools
import math
import heapq
import bisect
from typing import *
from collections import *
from functools import *
from heapq import *
from bisect import *

class Solution:
    def countOfAtoms(self, formula: str) -> str:
        stack, curr, i, n = [], {}, 0, len(formula)
        while i < n:
            if formula[i] == '(':
                stack.append(curr)
                curr = {}
                i += 1
            elif formula[i] == ')':
                i += 1
                start = i
                while i < n and formula[i].isdigit(): i += 1
                mult = int(formula[start:i] or 1)
                prev = stack.pop()
                for atom, count in curr.items():
                    prev[atom] = prev.get(atom, 0) + count * mult
                curr = prev
            else:
                start = i
                i += 1
                while i < n and formula[i].islower(): i += 1
                name = formula[start:i]
                start = i
                while i < n and formula[i].isdigit(): i += 1
                count = int(formula[start:i] or 1)
                curr[name] = curr.get(name, 0) + count
        return "".join(f"{k}{v if v > 1 else ''}" for k, v in sorted(curr.items()))
