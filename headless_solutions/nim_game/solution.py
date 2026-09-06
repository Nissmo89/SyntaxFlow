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
    def canWinNim(self, n: int) -> bool:
        return n % 4 != 0
