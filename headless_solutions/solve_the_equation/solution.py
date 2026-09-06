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

import re

class Solution:
    def solveEquation(self, equation: str) -> str:
        def parse(expr):
            # Find all terms: sign, number, and x presence
            tokens = re.findall(r'([+-]?\d*)(x?)', expr)
            x_sum, n_sum = 0, 0
            for coeff, has_x in tokens:
                if not coeff and not has_x: continue
                if has_x:
                    x_sum += int(coeff) if coeff not in ('', '+', '-') else (1 if coeff != '-' else -1)
                else:
                    n_sum += int(coeff)
            return x_sum, n_sum

        left, right = equation.split('=')
        lx, ln = parse(left)
        rx, rn = parse(right)
        
        x = lx - rx
        n = rn - ln
        
        if x == 0:
            return "Infinite solutions" if n == 0 else "No solution"
        return f"x={n // x}"
