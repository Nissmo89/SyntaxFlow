
import sys
import json
import time
import io
import traceback
from typing import List, Optional, Any

# --- UTILITIES ---
class TreeNode:
    def __init__(self, val=0, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right

def to_tree_node(arr):
    if not arr or arr[0] is None:
        return None
    root = TreeNode(arr[0])
    queue = [root]
    i = 1
    while queue and i < len(arr):
        node = queue.pop(0)
        if i < len(arr) and arr[i] is not None:
            node.left = TreeNode(arr[i])
            queue.append(node.left)
        i += 1
        if i < len(arr) and arr[i] is not None:
            node.right = TreeNode(arr[i])
            queue.append(node.right)
        i += 1
    return root

def tree_node_to_array(root):
    if not root:
        return []
    res, queue = [], [root]
    while queue:
        node = queue.pop(0)
        if node:
            res.append(node.val)
            queue.append(node.left)
            queue.append(node.right)
        else:
            res.append(None)
    while res and res[-1] is None:
        res.pop()
    return res

class ListNode:
    def __init__(self, val=0, next=None):
        self.val = val
        self.next = next

def to_list_node(arr: List[int]) -> Optional[ListNode]:
    if not arr:
        return None
    head = ListNode(arr[0])
    cur = head
    for v in arr[1:]:
        cur.next = ListNode(v)
        cur = cur.next
    return head

def list_node_to_array(head: Optional[ListNode]) -> List[int]:
    res = []
    while head:
        res.append(head.val)
        head = head.next
    return res

def to_json(obj: Any) -> str:
    seen = set()
    return _encode(obj, seen)

def _encode(obj: Any, seen: set) -> str:
    if obj is None:
        return "null"
    if obj is True:
        return "true"
    if obj is False:
        return "false"
    if isinstance(obj, int):
        return str(obj)
    if isinstance(obj, float):
        return repr(obj)
    if isinstance(obj, str):
        return _quote(obj)
    if isinstance(obj, (bytes, bytearray, memoryview)):
        return _quote(bytes(obj).hex())
    try:
        import enum
        if isinstance(obj, enum.Enum):
            return _quote(obj.name)
    except Exception:
        pass
    oid = id(obj)
    if oid in seen:
        raise ValueError("Cycle detected while encoding to json")
    seen.add(oid)
    try:
        if isinstance(obj, dict):
            items = []
            for k, v in obj.items():
                if not isinstance(k, str):
                    k = str(k)
                items.append(_quote(k) + ":" + _encode(v, seen))
            return "{" + ",".join(items) + "}"
        if isinstance(obj, (list, tuple, set, frozenset)):
            return "[" + ",".join(_encode(x, seen) for x in obj) + "]"
        if hasattr(obj, "__dict__"):
            d = {}
            for k, v in obj.__dict__.items():
                if k.startswith("_"):
                    continue
                d[k] = v
            return _encode(d, seen)
        slots = getattr(obj, "__slots__", None)
        if slots:
            d = {}
            if isinstance(slots, str):
                slots = [slots]
            for k in slots:
                if k.startswith("_"):
                    continue
                if hasattr(obj, k):
                    d[k] = getattr(obj, k)
            return _encode(d, seen)
        return _quote(str(obj))
    finally:
        seen.remove(oid)

def _quote(s: str) -> str:
    out = ['"']
    for ch in s:
        o = ord(ch)
        if ch == '"':
            out.append('\\"')
        elif ch == "\\":
            out.append("\\\\")
        elif ch == "\b":
            out.append("\\b")
        elif ch == "\f":
            out.append("\\f")
        elif ch == "\n":
            out.append("\\n")
        elif ch == "\r":
            out.append("\\r")
        elif ch == "\t":
            out.append("\\t")
        elif o < 0x20:
            out.append("\\u%04x" % o)
        else:
            out.append(ch)
    out.append('"')
    return "".join(out)

def compare_ignore_order(a, b):
    def sort_recursive(obj):
        if isinstance(obj, list):
            return sorted([sort_recursive(x) for x in obj], key=lambda x: str(x))
        elif isinstance(obj, dict):
            return {k: sort_recursive(v) for k, v in obj.items()}
        else:
            return obj
    return sort_recursive(a) == sort_recursive(b)

def deserialize_val(val, val_type):
    if val is None:
        return None
    if isinstance(val, dict) and 'elemType' in val and 'value' in val:
        val = val['value']
    if val_type == 'list_node':
        return to_list_node(val)
    elif val_type == 'tree_node':
        return to_tree_node(val)
    elif val_type == 'array':
        return val
    else:
        return val

# --- USER SOLUTION ---
class Solution:
    def addBinary(self, a: str, b: str) -> str:
        c = int(a, 2) + int(b,2)
        return bin(c)[2:]

# --- RUNNER ---
def run_all_tests():
    manifest = json.loads("{\"entry\":{\"call\":{\"cpp\":\"Solution().addBinary({a}, {b})\",\"csharp\":\"new Solution().AddBinary({a}, {b})\",\"dart\":\"Solution().addBinary({a}, {b})\",\"go\":\"addBinary({a}, {b})\",\"java\":\"new Solution().addBinary({a}, {b})\",\"kotlin\":\"Solution().addBinary({a}, {b})\",\"python2\":\"Solution().addBinary({a}, {b})\",\"python3\":\"Solution().addBinary({a}, {b})\",\"ruby\":\"add_binary({a}, {b})\",\"rust\":\"Solution::add_binary({a}, {b})\",\"swift\":\"Solution().addBinary({a}, {b})\",\"typescript\":\"addBinary({a}, {b})\"},\"id\":67,\"params\":{\"a\":{\"type\":\"string\"},\"b\":{\"type\":\"string\"}},\"title\":\"add-binary\"},\"judge\":{\"type\":\"exact\"},\"limits\":{\"memory_mb\":256,\"time_ms\":1000},\"oracle\":{\"python3\":{\"call\":\"Checker().addBinary(a, b, {result})\",\"checker\":\"class Checker:\\n    def addBinary(self, a, b, result):\\n        if not isinstance(a, str) or not isinstance(b, str) or not isinstance(result, str):\\n            return False\\n        if not a or not b or not result:\\n            return False\\n        if any(ch not in \\\"01\\\" for ch in a):\\n            return False\\n        if any(ch not in \\\"01\\\" for ch in b):\\n            return False\\n        if any(ch not in \\\"01\\\" for ch in result):\\n            return False\\n        return result == bin(int(a, 2) + int(b, 2))[2:]\\n\"}},\"seed\":67,\"tests\":[{\"in\":{\"a\":\"0\",\"b\":\"0\"},\"name\":\"zero_plus_zero\"},{\"in\":{\"a\":\"0\",\"b\":\"1\"},\"name\":\"zero_plus_one\"},{\"in\":{\"a\":\"1\",\"b\":\"0\"},\"name\":\"one_plus_zero\"},{\"in\":{\"a\":\"1\",\"b\":\"1\"},\"name\":\"one_plus_one\"},{\"in\":{\"a\":\"10\",\"b\":\"1\"},\"name\":\"carry_simple_1\"},{\"in\":{\"a\":\"11\",\"b\":\"1\"},\"name\":\"carry_simple_2\"},{\"in\":{\"a\":\"1010\",\"b\":\"1011\"},\"name\":\"example_1\"},{\"in\":{\"a\":\"1111\",\"b\":\"1\"},\"name\":\"all_ones_plus_one_4\"},{\"in\":{\"a\":\"1000\",\"b\":\"1000\"},\"name\":\"same_length_carry\"},{\"in\":{\"a\":\"101010\",\"b\":\"110011\"},\"name\":\"mixed_bits_1\"},{\"in\":{\"a\":\"100000\",\"b\":\"1\"},\"name\":\"different_length_with_carry\"},{\"in\":{\"a\":\"111111\",\"b\":\"111111\"},\"name\":\"all_ones_double\"},{\"in\":{\"a\":\"101\",\"b\":\"101\"},\"name\":\"palindromic_pair\"},{\"in\":{\"a\":\"1001\",\"b\":\"111\"},\"name\":\"carry_into_new_digit\"},{\"in\":{\"a\":\"10000\",\"b\":\"1111\"},\"name\":\"wide_carry_chain\"},{\"in\":{\"a\":\"110010\",\"b\":\"101101\"},\"name\":\"uneven_length_1\"},{\"in\":{\"a\":\"1\",\"b\":\"1111111111111111\"},\"name\":\"one_and_long_ones\"},{\"in\":{\"a\":\"1010101010101010\",\"b\":\"0101010101010101\"},\"name\":\"alternating_bits\"},{\"in\":{\"a\":\"1000000000000000\",\"b\":\"1000000000000000\"},\"name\":\"power_of_two_double\"},{\"in\":{\"a\":\"111000111000\",\"b\":\"111000111000\"},\"name\":\"repeated_pattern\"},{\"in\":{\"a\":\"100000000000000000000\",\"b\":\"1\"},\"name\":\"large_sparse_1\"},{\"in\":{\"a\":\"10101111000011110000\",\"b\":\"1111000011110000\"},\"name\":\"mixed_long_1\"},{\"in\":{\"a\":\"101\",\"b\":\"0\"},\"name\":\"add_zero_right\"},{\"in\":{\"a\":\"0\",\"b\":\"101\"},\"name\":\"add_zero_left\"},{\"in\":{\"a\":\"100\",\"b\":\"100\"},\"name\":\"single_carry_middle\"},{\"in\":{\"a\":\"1001\",\"b\":\"1\"},\"name\":\"increment_small\"},{\"in\":{\"a\":\"1110\",\"b\":\"10\"},\"name\":\"carry_with_zero_suffix\"},{\"in\":{\"a\":\"1000001\",\"b\":\"111111\"},\"name\":\"carry_to_next_width\"},{\"in\":{\"a\":\"1000000000000000000000000000000\",\"b\":\"1\"},\"name\":\"very_large_sparse\"},{\"in\":{\"a\":\"1111111111111111111111111111111\",\"b\":\"1\"},\"name\":\"all_ones_31_plus_one\"},{\"in\":{\"a\":\"1010101010101010101010101010101\",\"b\":\"1010101010101010101010101010101\"},\"name\":\"symmetric_long_pattern\"},{\"in\":{\"a\":\"1000000000000000000000000000000\",\"b\":\"1000000000000000000000000000000\"},\"name\":\"sparse_large_double\"},{\"in\":{\"a\":\"110110110110110110110\",\"b\":\"101001011010010110100\"},\"name\":\"mixed_long_2\"},{\"in\":{\"a\":\"100110011001100110011\",\"b\":\"111000111000111000111\"},\"name\":\"mixed_long_3\"},{\"in\":{\"a\":\"1111111111111111111111111111111111111111111111111111111111111111\",\"b\":\"1\"},\"name\":\"sixty_four_ones_plus_one\"},{\"in\":{\"a\":\"1111111111111111111111111111111111111111111111111111111111111110\",\"b\":\"1\"},\"name\":\"sixty_four_ones_minus_one_plus_one\"}]}")
    test_cases = manifest.get('tests', [])
    entry = manifest.get('entry', {})
    params_schema = entry.get('params', {})
    call_dict = entry.get('call', {})
    call_template = call_dict.get('python3', call_dict.get('python', call_dict.get('python2', '')))
    judge_type = manifest.get('judge', {}).get('type', 'exact')
    
    oracle = manifest.get('oracle', {}).get('python3', {})
    oracle_code = oracle.get('checker', '')
    oracle_call = oracle.get('call', '')
    
    if oracle_code:
        exec(oracle_code, globals())

    eval_expr = call_template
    for p_name in params_schema.keys():
        eval_expr = eval_expr.replace(f"{{{p_name}}}", p_name)

    results = []
    for idx, tc in enumerate(test_cases):
        captured_stdout = io.StringIO()
        captured_stderr = io.StringIO()
        old_stdout = sys.stdout
        old_stderr = sys.stderr
        sys.stdout = captured_stdout
        sys.stderr = captured_stderr
        
        status = "Accepted"
        actual_str = ""
        elapsed_ms = 0
        
        try:
            local_vars = {}
            for p_name, p_schema in params_schema.items():
                p_type = p_schema.get('type', 'int')
                raw_val = tc['in'][p_name]
                local_vars[p_name] = deserialize_val(raw_val, p_type)
            
            start_time = time.perf_counter()
            res_val = eval(eval_expr, globals(), local_vars)
            end_time = time.perf_counter()
            elapsed_ms = int((end_time - start_time) * 1000)
            
            actual_json = to_json(res_val)
            actual_str = actual_json
            
            exp_val = tc.get('out')
            if exp_val is not None:
                exp_json = to_json(exp_val)
                if judge_type == 'custom':
                    try:
                        is_correct = eval(oracle_call, globals(), {**local_vars, "res": res_val})
                        if not is_correct:
                            status = "Wrong Answer"
                    except Exception as e:
                        status = "System Error"
                        actual_str = f"Checker error: {str(e)}"
                elif judge_type == 'unordered':
                    if not compare_ignore_order(res_val, exp_val):
                        status = "Wrong Answer"
                else:
                    if actual_json != exp_json:
                        status = "Wrong Answer"
                        
        except Exception as e:
            status = "Runtime Error"
            actual_str = traceback.format_exc()
            
        finally:
            sys.stdout = old_stdout
            sys.stderr = old_stderr
            
        results.append({
            "status": status,
            "actual": actual_str,
            "expected": to_json(tc.get('out')) if tc.get('out') is not None else "",
            "elapsedMs": elapsed_ms,
            "stdout": captured_stdout.getvalue() + captured_stderr.getvalue()
        })
        
    print("SF_JSON_SUMMARY_START")
    print(json.dumps(results))
    print("SF_JSON_SUMMARY_END")
run_all_tests()
