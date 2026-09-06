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
        # split() without arguments automatically handles multiple spaces
        # and strips leading/trailing whitespace.
        words = s.split()
        return " ".join(reversed(words))
