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
    def wordBreak(self, s: str, wordDict: List[str]) -> List[str]:
        word_set = set(wordDict)
        memo = {}

        def dfs(target):
            if target in memo: return memo[target]
            if not target: return [""]
            
            res = []
            for word in word_set:
                if target.startswith(word):
                    sub_sentences = dfs(target[len(word):])
                    for sub in sub_sentences:
                        res.append((word + " " + sub).strip())
            
            memo[target] = res
            return res
            
        return dfs(s)
