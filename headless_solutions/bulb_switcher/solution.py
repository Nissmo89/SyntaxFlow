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

import math

class Solution:
    def bulbSwitch(self, n: int) -> int:
        # math.isqrt returns the integer square root directly.
        return math.isqrt(n)
