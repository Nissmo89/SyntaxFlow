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
    def spellchecker(self, wordlist: List[str], queries: List[str]) -> List[str]:
        def get_mask(w):
            return "".join('#' if c in 'aeiou' else c for c in w.lower())
        
        exact = set(wordlist)
        cap = {}
        vowel = {}
        
        for w in reversed(wordlist):
            cap[w.lower()] = w
            vowel[get_mask(w)] = w
            
        def solve(q):
            if q in exact: return q
            l = q.lower()
            if l in cap: return cap[l]
            m = get_mask(q)
            if m in vowel: return vowel[m]
            return ""
            
        return [solve(q) for q in queries]
