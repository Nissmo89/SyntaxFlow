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
    def allCellsDistOrder(self, rows: int, cols: int, rCenter: int, cCenter: int) -> List[List[int]]:
        result = [[r, c] for r in range(rows) for c in range(cols)]
        result.sort(key=lambda x: abs(x[0] - rCenter) + abs(x[1] - cCenter))
        return result
