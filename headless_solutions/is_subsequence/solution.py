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
    def isSubsequence(self, s: str, t: str) -> bool:
        i = j = 0
        while j < len(t):
            if i < len(s) and s[i] == t[j]:
                i += 1
            j += 1
        return i == len(s)
