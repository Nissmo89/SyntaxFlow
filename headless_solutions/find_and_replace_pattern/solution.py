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
    def findAndReplacePattern(self, words: List[str], pattern: str) -> List[str]:
        def normalize(s):
            m = {}
            return [m.setdefault(c, len(m)) for c in s]
        
        p_norm = normalize(pattern)
        return [w for w in words if normalize(w) == p_norm]
