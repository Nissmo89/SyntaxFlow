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
    def grayCode(self, n: int) -> List[int]:
        # Using list comprehension for efficiency
        return [i ^ (i >> 1) for i in range(1 << n)]
