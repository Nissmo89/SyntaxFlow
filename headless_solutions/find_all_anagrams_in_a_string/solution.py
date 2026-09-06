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
    def findAnagrams(self, s: str, p: str) -> List[int]:
        n, m = len(s), len(p)
        if m > n:
            return []

        # frequency of p
        need = [0] * 26
        for ch in p:
            need[ord(ch) - ord('a')] += 1

        # frequency of the first window
        window = [0] * 26
        for i in range(m):
            window[ord(s[i]) - ord('a')] += 1

        ans = []
        if window == need:
            ans.append(0)

        # slide the window
        for i in range(m, n):
            # remove leftmost char
            left = ord(s[i - m]) - ord('a')
            window[left] -= 1
            # add new char
            right = ord(s[i]) - ord('a')
            window[right] += 1

            if window == need:
                ans.append(i - m + 1)

        return ans
