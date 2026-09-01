
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
    def isAdditiveNumber(self, num: str) -> bool:
        for i in range(1, len(num) // 2 + 1):
            for j in range(1, len(num) - i):
                num1 = num[:i]
                num2 = num[i:i+j]
                remainder = num[i+j:]
                
                if (len(num1) > 1 and num1[0] == '0') or (len(num2) > 1 and num2[0] == '0'):
                    continue
                if self.isValid(num1, num2, remainder):
                    return True
        return False
    
    def isValid(self, num1, num2, remainder):
        while remainder:
            sum_val = self.add(num1, num2)
            if not remainder.startswith(sum_val):
                return False
            num1, num2 = num2, sum_val
            remainder = remainder[len(sum_val):]
        return True
    
    def add(self, n, m):
        res = ''
        i, j, carry = len(n) - 1, len(m) - 1, 0
        while i >= 0 or j >= 0:
            sum_val = carry + (int(n[i]) if i >= 0 else 0) + (int(m[j]) if j >= 0 else 0)
            res = str(sum_val % 10) + res
            carry = sum_val // 10
            i -= 1
            j -= 1
        return (str(carry) + res) if carry else res

# --- RUNNER ---
def run_all_tests():
    manifest = json.loads("{\"entry\":{\"call\":{\"cpp\":\"Solution().isAdditiveNumber({num})\",\"csharp\":\"new Solution().IsAdditiveNumber({num})\",\"dart\":\"Solution().isAdditiveNumber({num})\",\"go\":\"isAdditiveNumber({num})\",\"java\":\"new Solution().isAdditiveNumber({num})\",\"kotlin\":\"Solution().isAdditiveNumber({num})\",\"python2\":\"Solution().isAdditiveNumber({num})\",\"python3\":\"Solution().isAdditiveNumber({num})\",\"ruby\":\"is_additive_number({num})\",\"rust\":\"Solution::is_additive_number({num}.to_string())\",\"swift\":\"Solution().isAdditiveNumber({num})\",\"typescript\":\"isAdditiveNumber({num})\"},\"id\":306,\"params\":{\"num\":{\"type\":\"string\"}},\"title\":\"additive-number\"},\"judge\":{\"type\":\"exact\"},\"limits\":{\"memory_mb\":256,\"time_ms\":1000},\"oracle\":{\"python3\":{\"call\":\"Checker().isAdditiveNumber(num, {result})\",\"checker\":\"class Checker:\\n    def isAdditiveNumber(self, num, result):\\n        if not isinstance(result, bool):\\n            return False\\n\\n        def add_strings(a, b):\\n            i, j = len(a) - 1, len(b) - 1\\n            carry = 0\\n            out = []\\n            while i >= 0 or j >= 0 or carry:\\n                s = carry\\n                if i >= 0:\\n                    s += ord(a[i]) - 48\\n                    i -= 1\\n                if j >= 0:\\n                    s += ord(b[j]) - 48\\n                    j -= 1\\n                out.append(chr(48 + (s % 10)))\\n                carry = s // 10\\n            return ''.join(reversed(out))\\n\\n        n = len(num)\\n        expected = False\\n        for i in range(1, n):\\n            if num[0] == '0' and i > 1:\\n                break\\n            for j in range(i + 1, n):\\n                if num[i] == '0' and j > i + 1:\\n                    break\\n                a = num[:i]\\n                b = num[i:j]\\n                k = j\\n                count = 2\\n                while k < n:\\n                    c = add_strings(a, b)\\n                    if not num.startswith(c, k):\\n                        break\\n                    k += len(c)\\n                    a, b = b, c\\n                    count += 1\\n                if k == n and count >= 3:\\n                    expected = True\\n                    return result == expected\\n        return result == expected\\n\"}},\"seed\":306306,\"tests\":[{\"in\":{\"num\":\"112358\"},\"name\":\"example 1\",\"out\":true},{\"in\":{\"num\":\"199100199\"},\"name\":\"example 2\",\"out\":true},{\"in\":{\"num\":\"1\"},\"name\":\"too short\",\"out\":false},{\"in\":{\"num\":\"12\"},\"name\":\"two digits\",\"out\":false},{\"in\":{\"num\":\"123\"},\"name\":\"simple additive\",\"out\":true},{\"in\":{\"num\":\"000\"},\"name\":\"leading zero trio\",\"out\":true},{\"in\":{\"num\":\"010\"},\"name\":\"leading zero invalid\",\"out\":false},{\"in\":{\"num\":\"1023\"},\"name\":\"leading zero in second number\",\"out\":false},{\"in\":{\"num\":\"101\"},\"name\":\"additive with zero\",\"out\":true},{\"in\":{\"num\":\"101202303\"},\"name\":\"additive with zeros\",\"out\":true},{\"in\":{\"num\":\"102030\"},\"name\":\"classic false\",\"out\":true},{\"in\":{\"num\":\"12122436\"},\"name\":\"classic true\",\"out\":true},{\"in\":{\"num\":\"1123581\"},\"name\":\"false with trailing mismatch\",\"out\":false},{\"in\":{\"num\":\"112359\"},\"name\":\"false due to wrong next sum\",\"out\":false},{\"in\":{\"num\":\"199100198\"},\"name\":\"false due to too many digits\",\"out\":false},{\"in\":{\"num\":\"112358132134558914423337761098715972584418167651094617711286\"},\"name\":\"large true fibonacci prefix\",\"out\":false},{\"in\":{\"num\":\"112358132134558914423337761098715972584418167651094617711287\"},\"name\":\"large false fibonacci prefix\",\"out\":false},{\"in\":{\"num\":\"0001\"},\"name\":\"many leading zeros invalid\",\"out\":false},{\"in\":{\"num\":\"011235\"},\"name\":\"zero and one\",\"out\":true},{\"in\":{\"num\":\"120122436\"},\"name\":\"hidden leading zero invalid\",\"out\":false},{\"in\":{\"num\":\"198019823962\"},\"name\":\"true mixed sizes\",\"out\":true},{\"in\":{\"num\":\"198019823961\"},\"name\":\"false mixed sizes\",\"out\":false},{\"in\":{\"num\":\"12122436601096516155\"},\"name\":\"true long carry chain\",\"out\":false},{\"in\":{\"num\":\"12122436601096516156\"},\"name\":\"false long carry chain\",\"out\":false},{\"in\":{\"num\":\"000000\"},\"name\":\"all zeros longer\",\"out\":true},{\"in\":{\"num\":\"0000001\"},\"name\":\"all zeros with extra digit\",\"out\":false},{\"in\":{\"num\":\"123581321\"},\"name\":\"true with larger first term\",\"out\":true},{\"in\":{\"num\":\"123581322\"},\"name\":\"false with larger first term\",\"out\":false},{\"in\":{\"num\":\"199100299399\"},\"name\":\"true from 1 99 100\",\"out\":true},{\"in\":{\"num\":\"199100299398\"},\"name\":\"false from 1 99 100\",\"out\":false},{\"in\":{\"num\":\"01\"},\"name\":\"false tiny zeroes\",\"out\":false},{\"in\":{\"num\":\"1122243360\"},\"name\":\"true nested carry\",\"out\":false},{\"in\":{\"num\":\"1122243361\"},\"name\":\"false nested carry\",\"out\":false},{\"in\":{\"num\":\"111122335588\"},\"name\":\"true alternating small\",\"out\":true},{\"in\":{\"num\":\"1234565790\"},\"name\":\"false with non additive middle\",\"out\":false}]}")
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
        
        extracted_expected = [None]
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
            if exp_val is None or judge_type == 'custom':
                if oracle_call:
                    try:
                        def trace_calls(frame, event, arg):
                            if event == "call":
                                if "expected" in frame.f_code.co_varnames:
                                    if hasattr(frame, 'f_trace_lines'):
                                        frame.f_trace_lines = False
                                    return trace_calls_local
                                return None
                            return None
                        def trace_calls_local(frame, event, arg):
                            if event == "return":
                                locals_dict = frame.f_locals
                                if "expected" in locals_dict:
                                    extracted_expected[0] = locals_dict["expected"]
                            return trace_calls_local
                        sys.settrace(trace_calls)
                        oracle_call_replaced = oracle_call.replace("{result}", "res")
                        is_correct = eval(oracle_call_replaced, globals(), {**local_vars, "res": res_val})
                        sys.settrace(None)
                        if not is_correct:
                            status = "Wrong Answer"
                    except Exception as e:
                        sys.settrace(None)
                        status = "System Error"
                        actual_str = f"Checker error: {str(e)}"
                else:
                    if res_val != exp_val:
                        status = "Wrong Answer"
            elif judge_type == 'unordered':
                if not compare_ignore_order(res_val, exp_val):
                    status = "Wrong Answer"
            else:
                exp_json = to_json(exp_val)
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
            "expected": to_json(extracted_expected[0]) if extracted_expected[0] is not None else (to_json(tc.get('out')) if tc.get('out') is not None else ""),
            "elapsedMs": elapsed_ms,
            "stdout": captured_stdout.getvalue() + captured_stderr.getvalue()
        })
        
    print("SF_JSON_SUMMARY_START")
    print(json.dumps(results))
    print("SF_JSON_SUMMARY_END")
run_all_tests()
