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
    def characterReplacement(self, s: str, k: int) -> int:
        count = {}
        left = max_freq = max_len = 0
        for right in range(len(s)):
            count[s[right]] = count.get(s[right], 0) + 1
            max_freq = max(max_freq, count[s[right]])
            
            if (right - left + 1) - max_freq > k:
                count[s[left]] -= 1
                left += 1
            
            max_len = max(max_len, right - left + 1)
        return max_len
