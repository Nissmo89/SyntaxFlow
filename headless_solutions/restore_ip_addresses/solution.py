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
    def restoreIpAddresses(self, s: str) -> List[str]:
        res = []
        def backtrack(start, path):
            if len(path) == 4:
                if start == len(s):
                    res.append(".".join(path))
                return
            for length in range(1, 4):
                if start + length > len(s): break
                sub = s[start:start + length]
                if (len(sub) > 1 and sub[0] == '0') or int(sub) > 255: continue
                backtrack(start + length, path + [sub])
        
        backtrack(0, [])
        return res
