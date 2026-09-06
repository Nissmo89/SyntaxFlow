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
    def prisonAfterNDays(self, cells: List[int], n: int) -> List[int]:
        seen = {}
        while n > 0:
            seen[tuple(cells)] = n
            n -= 1
            cells = [0] + [1 if cells[i-1] == cells[i+1] else 0 for i in range(1, 7)] + [0]
            if tuple(cells) in seen:
                n %= (seen[tuple(cells)] - n)
        return cells
