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
    def rearrangeBarcodes(self, barcodes: list[int]) -> list[int]:
        count = Counter(barcodes)
        # Sort by frequency descending
        sorted_items = count.most_common()
        n = len(barcodes)
        res = [0] * n
        pos = 0
        
        for val, freq in sorted_items:
            for _ in range(freq):
                if pos >= n:
                    pos = 1
                res[pos] = val
                pos += 2
        return res
