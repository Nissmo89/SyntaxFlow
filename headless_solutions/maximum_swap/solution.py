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
    def maximumSwap(self, num: int) -> int:
        s = list(str(num))
        last = {int(digit): i for i, digit in enumerate(s)}
        
        for i, digit in enumerate(s):
            for d in range(9, int(digit), -1):
                if last.get(d, -1) > i:
                    s[i], s[last[d]] = s[last[d]], s[i]
                    return int("".join(s))
        return num
