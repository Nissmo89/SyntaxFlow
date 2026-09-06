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
    def largestValsFromLabels(self, values: List[int], labels: List[int], numWanted: int, useLimit: int) -> int:
        items = sorted(zip(values, labels), key=lambda x: x[0], reverse=True)
        label_counts = {}
        total_sum = 0
        count = 0
        
        for val, label in items:
            if count == numWanted:
                break
            if label_counts.get(label, 0) < useLimit:
                total_sum += val
                label_counts[label] = label_counts.get(label, 0) + 1
                count += 1
        return total_sum
