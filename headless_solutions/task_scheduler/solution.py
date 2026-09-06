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

class Solution:
    def leastInterval(self, tasks: List[str], n: int) -> int:
        counts = Counter(tasks)
        max_freq = max(counts.values())
        # Count how many tasks have the max frequency
        count_max = sum(1 for f in counts.values() if f == max_freq)
        
        # Formula: (max_freq - 1) * (n + 1) + count_max
        result = (max_freq - 1) * (n + 1) + count_max
        return max(len(tasks), result)
