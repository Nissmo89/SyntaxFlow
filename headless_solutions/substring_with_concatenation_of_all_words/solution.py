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

from collections import Counter

class Solution:
    def findSubstring(self, s: str, words: list[str]) -> list[int]:
        if not s or not words: return []
        word_counts = Counter(words)
        n, m, l = len(s), len(words), len(words[0])
        res = []
        
        for i in range(l):
            seen = Counter()
            left = i
            count = 0
            for j in range(i, n - l + 1, l):
                word = s[j:j+l]
                if word in word_counts:
                    seen[word] += 1
                    count += 1
                    while seen[word] > word_counts[word]:
                        left_word = s[left:left+l]
                        seen[left_word] -= 1
                        count -= 1
                        left += l
                    if count == m:
                        res.append(left)
                else:
                    seen.clear()
                    count = 0
                    left = j + l
        return res
