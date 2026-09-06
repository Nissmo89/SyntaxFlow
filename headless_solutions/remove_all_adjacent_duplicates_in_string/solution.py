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
    def removeDuplicates(self, s: str) -> str:
        res = ""
        for c in s:
            if res and c == res[-1]:
                res = res[:-1]
            else:
                res += c
        return res
