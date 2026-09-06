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
    def findSubstringInWraproundString(self, s: str) -> int:
        count = [0] * 26
        max_len_cur = 0
        for i in range(len(s)):
            if i > 0 and (ord(s[i]) - ord(s[i-1]) == 1 or (s[i-1] == 'z' and s[i] == 'a')):
                max_len_cur += 1
            else:
                max_len_cur = 1
            idx = ord(s[i]) - ord('a')
            count[idx] = max(count[idx], max_len_cur)
        return sum(count)
