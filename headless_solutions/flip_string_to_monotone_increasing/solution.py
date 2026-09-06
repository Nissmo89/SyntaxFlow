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
    def minFlipsMonoIncr(self, s: str) -> int:
        count_one = 0
        count_flip = 0
        for char in s:
            if char == '1':
                count_one += 1
            else:
                # Min of flipping current '0' or flipping all previous '1's
                count_flip = min(count_flip + 1, count_one)
        return count_flip
