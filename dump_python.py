
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
    def complexNumberMultiply(self, num1: str, num2: str) -> str:
        # Strip trailing 'i' and split by '+'
        a_str, b_str = num1[:-1].split('+')
        c_str, d_str = num2[:-1].split('+')
        
        a, b = int(a_str), int(b_str)
        c, d = int(c_str), int(d_str)
        
        # Calculate real and imaginary components
        real_part = a * c - b * d
        imag_part = a * d + b * c
        
        return f"{real_part}+{imag_part}i"

# --- RUNNER ---
def run_all_tests():
    manifest = json.loads("{\"entry\":{\"call\":{\"cpp\":\"Solution().complexNumberMultiply({num1}, {num2})\",\"csharp\":\"new Solution().ComplexNumberMultiply({num1}, {num2})\",\"dart\":\"Solution().complexNumberMultiply({num1}, {num2})\",\"go\":\"complexNumberMultiply({num1}, {num2})\",\"java\":\"new Solution().complexNumberMultiply({num1}, {num2})\",\"kotlin\":\"Solution().complexNumberMultiply({num1}, {num2})\",\"python2\":\"Solution().complexNumberMultiply({num1}, {num2})\",\"python3\":\"Solution().complexNumberMultiply({num1}, {num2})\",\"ruby\":\"complex_number_multiply({num1}, {num2})\",\"rust\":\"Solution::complex_number_multiply({num1}, {num2})\",\"swift\":\"Solution().complexNumberMultiply({num1}, {num2})\",\"typescript\":\"complexNumberMultiply({num1}, {num2})\"},\"id\":537,\"params\":{\"num1\":{\"type\":\"string\"},\"num2\":{\"type\":\"string\"}},\"title\":\"complex-number-multiplication\"},\"judge\":{\"type\":\"exact\"},\"limits\":{\"memory_mb\":256,\"time_ms\":1000},\"oracle\":{\"python3\":{\"call\":\"Checker().check(num1, num2, {result})\",\"checker\":\"from typing import Any, Tuple\\n\\nclass Checker:\\n    def parse(self, s: str) -> Tuple[int, int]:\\n        if not isinstance(s, str) or not s.endswith(\\\"i\\\") or \\\"+\\\" not in s:\\n            raise ValueError(\\\"invalid complex string\\\")\\n        real_str, imag_str = s[:-1].split(\\\"+\\\", 1)\\n        real = int(real_str)\\n        imag = int(imag_str)\\n        if real < -100 or real > 100 or imag < -100 or imag > 100:\\n            raise ValueError(\\\"value out of range\\\")\\n        return real, imag\\n\\n    def check(self, num1: str, num2: str, result: Any) -> bool:\\n        if not isinstance(result, str):\\n            return False\\n        try:\\n            a, b = self.parse(num1)\\n            c, d = self.parse(num2)\\n            real, imag = self.parse(result)\\n        except Exception:\\n            return False\\n        expected_real = a * c - b * d\\n        expected_imag = a * d + b * c\\n        return real == expected_real and imag == expected_imag\\n\"}},\"seed\":537537,\"tests\":[{\"in\":{\"num1\":\"1+1i\",\"num2\":\"1+1i\"},\"name\":\"example_1\",\"out\":\"0+2i\"},{\"in\":{\"num1\":\"1+-1i\",\"num2\":\"1+-1i\"},\"name\":\"example_2\",\"out\":\"0+-2i\"},{\"in\":{\"num1\":\"0+0i\",\"num2\":\"0+0i\"},\"name\":\"zero_zero\",\"out\":\"0+0i\"},{\"in\":{\"num1\":\"1+0i\",\"num2\":\"0+0i\"},\"name\":\"one_times_zero\",\"out\":\"0+0i\"},{\"in\":{\"num1\":\"0+0i\",\"num2\":\"1+1i\"},\"name\":\"zero_times_complex\",\"out\":\"0+0i\"},{\"in\":{\"num1\":\"5+0i\",\"num2\":\"-7+0i\"},\"name\":\"real_only_negative_product\",\"out\":\"-35+0i\"},{\"in\":{\"num1\":\"1+1i\",\"num2\":\"1+-1i\"},\"name\":\"conjugates_cancel_imaginary\",\"out\":\"2+0i\"},{\"in\":{\"num1\":\"0+2i\",\"num2\":\"0+3i\"},\"name\":\"pure_imaginary_positive\",\"out\":\"-6+0i\"},{\"in\":{\"num1\":\"0+-2i\",\"num2\":\"0+3i\"},\"name\":\"pure_imaginary_mixed_sign\",\"out\":\"6+0i\"},{\"in\":{\"num1\":\"100+100i\",\"num2\":\"100+100i\"},\"name\":\"boundary_same_positive\",\"out\":\"0+20000i\"},{\"in\":{\"num1\":\"100+-100i\",\"num2\":\"100+-100i\"},\"name\":\"boundary_same_negative_imag\",\"out\":\"0+-20000i\"},{\"in\":{\"num1\":\"100+100i\",\"num2\":\"100+-100i\"},\"name\":\"boundary_conjugate\",\"out\":\"20000+0i\"},{\"in\":{\"num1\":\"-100+100i\",\"num2\":\"100+100i\"},\"name\":\"boundary_opposite_real\",\"out\":\"-20000+0i\"},{\"in\":{\"num1\":\"-100+-100i\",\"num2\":\"-100+-100i\"},\"name\":\"double_negative_boundary\",\"out\":\"0+20000i\"},{\"in\":{\"num1\":\"-100+0i\",\"num2\":\"0+100i\"},\"name\":\"negative_real_pure_imaginary\",\"out\":\"0+-10000i\"},{\"in\":{\"num1\":\"3+2i\",\"num2\":\"1+7i\"},\"name\":\"mixed_small_positive\",\"out\":\"-11+23i\"},{\"in\":{\"num1\":\"3+-2i\",\"num2\":\"1+7i\"},\"name\":\"mixed_small_negative_imag\",\"out\":\"17+19i\"},{\"in\":{\"num1\":\"-3+2i\",\"num2\":\"1+7i\"},\"name\":\"negative_real_positive_imag\",\"out\":\"-17+-19i\"},{\"in\":{\"num1\":\"-3+-2i\",\"num2\":\"1+-7i\"},\"name\":\"double_negative_mixed\",\"out\":\"-17+19i\"},{\"in\":{\"num1\":\"99+1i\",\"num2\":\"1+99i\"},\"name\":\"high_cross_imaginary\",\"out\":\"0+9802i\"},{\"in\":{\"num1\":\"2+3i\",\"num2\":\"4+5i\"},\"name\":\"classic_2_3_4_5\",\"out\":\"-7+22i\"},{\"in\":{\"num1\":\"-2+3i\",\"num2\":\"4+-5i\"},\"name\":\"mixed_signs_cross\",\"out\":\"7+22i\"},{\"in\":{\"num1\":\"-8+0i\",\"num2\":\"-3+0i\"},\"name\":\"real_times_real_positive\",\"out\":\"24+0i\"},{\"in\":{\"num1\":\"0+5i\",\"num2\":\"-7+0i\"},\"name\":\"imaginary_times_real_negative\",\"out\":\"0+-35i\"},{\"in\":{\"num1\":\"0+-5i\",\"num2\":\"0+-7i\"},\"name\":\"negative_pure_imaginary_square\",\"out\":\"-35+0i\"},{\"in\":{\"num1\":\"7+8i\",\"num2\":\"0+1i\"},\"name\":\"multiply_by_i\",\"out\":\"-8+7i\"},{\"in\":{\"num1\":\"7+8i\",\"num2\":\"0+-1i\"},\"name\":\"multiply_by_negative_i\",\"out\":\"8+-7i\"},{\"in\":{\"num1\":\"42+17i\",\"num2\":\"1+0i\"},\"name\":\"identity_complex\",\"out\":\"42+17i\"},{\"in\":{\"num1\":\"42+17i\",\"num2\":\"-1+0i\"},\"name\":\"negate_complex\",\"out\":\"-42+-17i\"},{\"in\":{\"num1\":\"11+-13i\",\"num2\":\"11+13i\"},\"name\":\"conjugate_product_11_13\",\"out\":\"290+0i\"},{\"in\":{\"num1\":\"6+9i\",\"num2\":\"6+-9i\"},\"name\":\"conjugate_product_6_9\",\"out\":\"117+0i\"},{\"in\":{\"num1\":\"10+-1i\",\"num2\":\"-2+-3i\"},\"name\":\"mixed_signs_large\",\"out\":\"-23+-28i\"},{\"in\":{\"num1\":\"-8+6i\",\"num2\":\"-3+-4i\"},\"name\":\"double_negative_with_negative_imag\",\"out\":\"48+14i\"},{\"in\":{\"num1\":\"100+0i\",\"num2\":\"0+-100i\"},\"name\":\"boundary_zero_imaginary_factor\",\"out\":\"0+-10000i\"},{\"in\":{\"num1\":\"-1+100i\",\"num2\":\"100+-1i\"},\"name\":\"mixed_sparse_boundary\",\"out\":\"0+10001i\"}]}")
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
