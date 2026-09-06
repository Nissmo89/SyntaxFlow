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
    def partitionLabels(self, s: str) -> List[int]:
        last = {char: i for i, char in enumerate(s)}
        result = []
        start = end = 0
        
        for i, char in enumerate(s):
            end = max(end, last[char])
            if i == end:
                result.append(i - start + 1)
                start = i + 1
        return result
