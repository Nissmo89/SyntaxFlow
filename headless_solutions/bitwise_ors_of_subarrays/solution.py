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
    def subarrayBitwiseORs(self, arr: List[int]) -> int:
        ans = set()
        s = set()
        for x in arr:
            s = {x | y for y in s} | {x}
            ans |= s
        return len(ans)
