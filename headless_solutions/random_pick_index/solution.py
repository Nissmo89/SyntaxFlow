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

import random
from collections import defaultdict
from typing import List

class Solution:

    def __init__(self, nums: List[int]):
        # Map target value to list of indices
        self.index_map = defaultdict(list)
        for i, num in enumerate(nums):
            self.index_map[num].append(i)

    def pick(self, target: int) -> int:
        # Uniformly choice from pre-stored indices
        return random.choice(self.index_map[target])


# Your Solution object will be instantiated and called as such:
# obj = Solution(nums)
# param_1 = obj.pick(target)
