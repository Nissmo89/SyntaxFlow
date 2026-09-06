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
    def isAnagram(self, s: str, t: str) -> bool:
        if len(s) != len(t): return False
        
        alphabet = [0] * 26
        for c in s: alphabet[ord(c) - ord('a')] += 1
        for c in t: alphabet[ord(c) - ord('a')] -= 1
        
        for count in alphabet:
            if count != 0: return False
        
        return True
