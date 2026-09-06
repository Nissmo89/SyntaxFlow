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

from typing import List

class Solution:
    def tallestBillboard(self, rods: List[int]) -> int:
        # dp[diff] = max height of the shorter support
        dp = {0: 0}
        for r in rods:
            curr = dp.copy()
            for diff, h in curr.items():
                # Add to taller
                dp[diff + r] = max(dp.get(diff + r, 0), h)
                # Add to shorter
                new_diff = abs(diff - r)
                dp[new_diff] = max(dp.get(new_diff, 0), h + min(diff, r))
        return dp[0]
