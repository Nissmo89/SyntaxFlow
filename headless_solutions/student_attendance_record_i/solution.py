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
  def checkRecord(self, s: str) -> bool:
    return s.count('A') <= 1 and 'LLL' not in s
