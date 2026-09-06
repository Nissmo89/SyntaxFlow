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

from collections import Counter
from typing import List

class Solution:
    def maxEqualRowsAfterFlips(self, matrix: List[List[int]]) -> int:
        counts = Counter()
        for row in matrix:
            # Normalize: if row[0] is 1, flip all bits so the pattern starts with 0
            first = row[0]
            pattern = tuple(val ^ first for val in row)
            counts[pattern] += 1
        return max(counts.values())
