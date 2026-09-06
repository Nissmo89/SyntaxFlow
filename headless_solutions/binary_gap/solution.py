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
    def binaryGap(self, n: int) -> int:
        max_distance = 0
        last_pos = -1
        pos = 0
        while n > 0:
            if n % 2 == 1:
                if last_pos != -1:
                    max_distance = max(max_distance, pos - last_pos)
                last_pos = pos
            n //= 2
            pos += 1
        return max_distance
