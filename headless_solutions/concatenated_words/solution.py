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

from typing import List

class Solution:
    def findAllConcatenatedWordsInADict(self, words: List[str]) -> List[str]:
        # Sort by length so that only shorter words are in the set
        words.sort(key=len)
        dict_set = set()
        ans = []

        for w in words:
            n = len(w)
            dp = [False] * (n + 1)
            dp[0] = True

            for i in range(1, n + 1):
                for j in range(i):
                    if dp[j] and w[j:i] in dict_set:
                        dp[i] = True
                        break

            if dp[n]:
                ans.append(w)
            dict_set.add(w)

        return ans
