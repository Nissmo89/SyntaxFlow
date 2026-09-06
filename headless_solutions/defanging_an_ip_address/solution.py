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
    def defangIPaddr(self, address: str) -> str:
        return address.replace('.', '[.]')
