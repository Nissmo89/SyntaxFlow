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
    def longestPalindrome(self, s: str) -> int:
        frequencyMap = {}
        for c in s:
            frequencyMap[c] = frequencyMap.get(c, 0) + 1
        res = 0
        hasOddFrequency = False
        for pair in frequencyMap.items():
            if pair[1] % 2 == 0:
                res += pair[1]
            else:
                res += pair[1] - 1
                hasOddFrequency = True
        if hasOddFrequency:
            return res + 1
        else:
            return res
