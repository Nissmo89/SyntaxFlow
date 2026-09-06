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
    def groupAnagrams(self, strs: List[str]) -> List[List[str]]:
        anagram_map = {}
        for s in strs:
            # Sort the string to create a canonical key
            key = "".join(sorted(s))
            if key not in anagram_map:
                anagram_map[key] = []
            anagram_map[key].append(s)
        return list(anagram_map.values())
