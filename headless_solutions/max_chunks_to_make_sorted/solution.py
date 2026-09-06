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
    def maxChunksToSorted(self, arr: List[int]) -> int:
        chunks = 0
        max_so_far = 0
        for i, val in enumerate(arr):
            max_so_far = max(max_so_far, val)
            # If max_so_far matches index i, the segment [0...i] 
            # contains exactly the numbers 0...i.
            if max_so_far == i:
                chunks += 1
        return chunks
