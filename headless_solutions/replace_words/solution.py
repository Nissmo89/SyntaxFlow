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
    def replaceWords(self, dictionary: List[str], sentence: str) -> str:
        trie = {}
        for root in dictionary:
            node = trie
            for char in root:
                node = node.setdefault(char, {})
            node['#'] = True  # End of word marker
            
        def find_root(word):
            node = trie
            path = []
            for char in word:
                if char not in node: break
                path.append(char)
                node = node[char]
                if '#' in node: return "".join(path)
            return word
            
        return " ".join(map(find_root, sentence.split()))
