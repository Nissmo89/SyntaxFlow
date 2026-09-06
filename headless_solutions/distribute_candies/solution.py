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
    def distributeCandies(self, candyType: List[int]) -> int:
        unique_candies = set(candyType)
        return min(len(unique_candies), len(candyType) // 2)
