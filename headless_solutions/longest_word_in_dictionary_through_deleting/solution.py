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
    def findLongestWord(self, s: str, dictionary: List[str]) -> str:
        longest = ""
        for word in dictionary:
            i = 0
            for char in s:
                if i < len(word) and char == word[i]:
                    i += 1
            if i == len(word):
                if len(word) > len(longest) or (len(word) == len(longest) and word < longest):
                    longest = word
        return longest
