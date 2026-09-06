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
    def reverseWords(self, s: str) -> str:
        strIndex = 0
        while strIndex < len(s):
            # Find the start and end index of the word
            start = strIndex
            while strIndex < len(s) and s[strIndex] != ' ':
                strIndex += 1
            # Reverse the word
            s = s[:start] + s[start:strIndex][::-1] + s[strIndex:]
            strIndex += 1
        return s
