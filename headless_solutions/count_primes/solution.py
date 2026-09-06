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
    def countPrimes(self, n: int) -> int:
        if n <= 2:
            return 0
        
        is_prime = [True] * n
        is_prime[0] = is_prime[1] = False
        
        # Outer loop up to sqrt(n)
        for i in range(2, int(n ** 0.5) + 1):
            if is_prime[i]:
                # Slice assignment provides C-level speedups in CPython
                is_prime[i * i : n : i] = [False] * len(range(i * i, n, i))
                
        return sum(is_prime)
