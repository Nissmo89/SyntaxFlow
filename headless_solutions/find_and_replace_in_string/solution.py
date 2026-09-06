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
    def findReplaceString(self, s: str, indices: List[int], sources: List[str], targets: List[str]) -> str:
        lookup = {idx: i for i, idx in enumerate(indices) if s.startswith(sources[i], idx)}
        res = []
        i = 0
        while i < len(s):
            if i in lookup:
                res.append(targets[lookup[i]])
                i += len(sources[lookup[i]])
            else:
                res.append(s[i])
                i += 1
        return "".join(res)
