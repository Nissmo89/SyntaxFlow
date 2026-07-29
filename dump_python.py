
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
    def addDigits(self, n: int) -> int:
        if n < 10:
            return n
        return 1 + ( n - 1) % 9

# --- RUNNER ---
def run_all_tests():
    manifest = json.loads("{\"entry\":{\"call\":{\"cpp\":\"Solution().addDigits({num})\",\"csharp\":\"new Solution().AddDigits({num})\",\"dart\":\"Solution().addDigits({num})\",\"go\":\"addDigits({num})\",\"java\":\"new Solution().addDigits({num})\",\"kotlin\":\"Solution().addDigits({num})\",\"python2\":\"Solution().addDigits({num})\",\"python3\":\"Solution().addDigits({num})\",\"ruby\":\"add_digits({num})\",\"rust\":\"Solution::add_digits({num})\",\"swift\":\"Solution().addDigits({num})\",\"typescript\":\"addDigits({num})\"},\"id\":258,\"params\":{\"num\":{\"type\":\"int\"}},\"title\":\"add-digits\"},\"judge\":{\"type\":\"exact\"},\"limits\":{\"memory_mb\":256,\"time_ms\":1000},\"oracle\":{\"python3\":{\"call\":\"Checker().check(num, {result})\",\"checker\":\"from typing import Any\\n\\nclass Checker:\\n    def check(self, num: Any, result: Any) -> bool:\\n        if not isinstance(num, int) or not isinstance(result, int):\\n            return False\\n        if num == 0:\\n            expected = 0\\n        else:\\n            expected = 1 + (num - 1) % 9\\n        return result == expected\\n\"}},\"seed\":258258,\"tests\":[{\"in\":{\"num\":0},\"name\":\"zero\",\"out\":0},{\"in\":{\"num\":1},\"name\":\"one\",\"out\":1},{\"in\":{\"num\":2},\"name\":\"two\",\"out\":2},{\"in\":{\"num\":3},\"name\":\"three\",\"out\":3},{\"in\":{\"num\":4},\"name\":\"four\",\"out\":4},{\"in\":{\"num\":5},\"name\":\"five\",\"out\":5},{\"in\":{\"num\":6},\"name\":\"six\",\"out\":6},{\"in\":{\"num\":7},\"name\":\"seven\",\"out\":7},{\"in\":{\"num\":8},\"name\":\"eight\",\"out\":8},{\"in\":{\"num\":9},\"name\":\"nine\",\"out\":9},{\"in\":{\"num\":10},\"name\":\"ten\",\"out\":1},{\"in\":{\"num\":11},\"name\":\"eleven\",\"out\":2},{\"in\":{\"num\":12},\"name\":\"twelve\",\"out\":3},{\"in\":{\"num\":18},\"name\":\"eighteen\",\"out\":9},{\"in\":{\"num\":19},\"name\":\"nineteen\",\"out\":1},{\"in\":{\"num\":27},\"name\":\"twenty_seven\",\"out\":9},{\"in\":{\"num\":38},\"name\":\"thirty_eight\",\"out\":2},{\"in\":{\"num\":49},\"name\":\"forty_nine\",\"out\":4},{\"in\":{\"num\":58},\"name\":\"fifty_eight\",\"out\":4},{\"in\":{\"num\":67},\"name\":\"sixty_seven\",\"out\":4},{\"in\":{\"num\":76},\"name\":\"seventy_six\",\"out\":4},{\"in\":{\"num\":85},\"name\":\"eighty_five\",\"out\":4},{\"in\":{\"num\":94},\"name\":\"ninety_four\",\"out\":4},{\"in\":{\"num\":100},\"name\":\"one_hundred\",\"out\":1},{\"in\":{\"num\":111},\"name\":\"one_hundred_eleven\",\"out\":3},{\"in\":{\"num\":199},\"name\":\"one_hundred_ninety_nine\",\"out\":1},{\"in\":{\"num\":288},\"name\":\"two_hundred_eighty_eight\",\"out\":9},{\"in\":{\"num\":999},\"name\":\"nine_hundred_ninety_nine\",\"out\":9},{\"in\":{\"num\":1000},\"name\":\"one_thousand\",\"out\":1},{\"in\":{\"num\":1000000},\"name\":\"one_million\",\"out\":1},{\"in\":{\"num\":1000001},\"name\":\"one_million_one\",\"out\":2},{\"in\":{\"num\":1000000000},\"name\":\"one_billion\",\"out\":1},{\"in\":{\"num\":2147483646},\"name\":\"max_minus_one\",\"out\":9},{\"in\":{\"num\":2147483647},\"name\":\"max_int\",\"out\":1},{\"in\":{\"num\":2147483637},\"name\":\"multiple_of_nine_near_max\",\"out\":9},{\"in\":{\"num\":2147483640},\"name\":\"large_multiple_of_nine\",\"out\":3},{\"in\":{\"num\":2147483638},\"name\":\"large_non_multiple_of_nine\",\"out\":1}]}")
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
