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
    def magicalString(self, n: int) -> int:
        if n <= 0: return 0
        if n <= 3: return 1
        s = [0] * (n + 1)
        s[0], s[1], s[2] = 1, 2, 2
        head, tail, num, count = 2, 3, 1, 1
        while tail < n:
            for _ in range(s[head]):
                s[tail] = num
                if num == 1 and tail < n:
                    count += 1
                tail += 1
            num = 3 - num # Toggle between 1 and 2
            head += 1
        return count
