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
    def orderlyQueue(self, s: str, k: int) -> str:
        if k > 1:
            return "".join(sorted(s))
        
        # For k=1, check all cyclic shifts
        res = s
        for i in range(1, len(s)):
            res = min(res, s[i:] + s[:i])
        return res
