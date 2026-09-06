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
    def findLUSlength(self, a: str, b: str) -> int:
        # If a and b are equal, return -1
        if a == b:
            return -1
        # Otherwise, return the maximum length of a and b
        return max(len(a), len(b))
