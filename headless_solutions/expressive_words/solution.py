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
    def expressiveWords(self, s: str, words: List[str]) -> int:
        def get_groups(string):
            groups = []
            i = 0
            while i < len(string):
                j = i
                while j < len(string) and string[j] == string[i]:
                    j += 1
                groups.append((string[i], j - i))
                i = j
            return groups

        s_groups = get_groups(s)
        count = 0
        for word in words:
            w_groups = get_groups(word)
            if len(s_groups) != len(w_groups):
                continue
            
            possible = True
            for (char1, len1), (char2, len2) in zip(s_groups, w_groups):
                if char1 != char2 or len1 < len2 or (len1 > len2 and len1 < 3):
                    possible = False
                    break
            if possible:
                count += 1
        return count
