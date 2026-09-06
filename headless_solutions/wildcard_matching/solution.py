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
    def isMatch(self, s: str, p: str) -> bool:
        s_idx, p_idx, star_idx, s_tmp_idx = 0, 0, -1, -1
        while s_idx < len(s):
            if p_idx < len(p) and (p[p_idx] == '?' or p[p_idx] == s[s_idx]):
                s_idx += 1
                p_idx += 1
            elif p_idx < len(p) and p[p_idx] == '*':
                star_idx = p_idx
                s_tmp_idx = s_idx
                p_idx += 1
            elif star_idx != -1:
                p_idx = star_idx + 1
                s_tmp_idx += 1
                s_idx = s_tmp_idx
            else:
                return False
        return all(x == '*' for x in p[p_idx:])
