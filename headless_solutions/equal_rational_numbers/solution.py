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
    def isRationalEqual(self, s: str, t: str) -> bool:
        
        def gcd(a, b):
            return math.gcd(a, b)

        def parse_rational(num_str):
            integer_part = 0
            non_repeating_part = 0
            repeating_part = 0
            non_repeating_len = 0
            repeating_len = 0

            dot_pos = num_str.find('.')
            paren_open_pos = num_str.find('(')
            paren_close_pos = num_str.find(')')

            if dot_pos == -1:
                integer_part = int(num_str)
            else:
                integer_part = int(num_str[:dot_pos])
                if paren_open_pos == -1:
                    # No repeating part
                    non_repeating_str = num_str[dot_pos + 1:]
                    if non_repeating_str:
                        non_repeating_part = int(non_repeating_str)
                        non_repeating_len = len(non_repeating_str)
                else:
                    # Has repeating part
                    non_repeating_str = num_str[dot_pos + 1:paren_open_pos]
                    if non_repeating_str:
                        non_repeating_part = int(non_repeating_str)
                        non_repeating_len = len(non_repeating_str)
                    
                    repeating_str = num_str[paren_open_pos + 1:paren_close_pos]
                    if repeating_str:
                        repeating_part = int(repeating_str)
                        repeating_len = len(repeating_str)

            # Calculate the fraction
            numerator = 0
            denominator = 1

            if repeating_len == 0:
                # Case: Integer or Integer.NonRepeating
                numerator = integer_part * (10**non_repeating_len) + non_repeating_part
                denominator = 10**non_repeating_len
            else:
                # Case: Integer.NonRepeating(Repeating)
                # Value = Integer + NonRepeating / 10^L_N + Repeating / (10^L_N * (10^L_R - 1))
                # Combine:
                # Numerator = I * 10^L_N * (10^L_R - 1) + N * (10^L_R - 1) + R
                # Denominator = 10^L_N * (10^L_R - 1)
                
                rep_den = (10**repeating_len) - 1 # 10^L_R - 1
                common_den = (10**non_repeating_len) * rep_den # 10^L_N * (10^L_R - 1)
                
                numerator = integer_part * common_den + non_repeating_part * rep_den + repeating_part
                denominator = common_den

            # Simplify the fraction
            if numerator == 0:
                return (0, 1)
            
            common_divisor = gcd(numerator, denominator)
            return (numerator // common_divisor, denominator // common_divisor)

        s_frac = parse_rational(s)
        t_frac = parse_rational(t)

        return s_frac == t_frac
