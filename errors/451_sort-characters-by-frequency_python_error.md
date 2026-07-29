# Error for Problem 451 (python)

## Stdout
```
TEST_RESULT|0|Accepted|0
TEST_RESULT|1|Accepted|0
TEST_RESULT|2|Accepted|0
TEST_RESULT|3|Accepted|0
TEST_RESULT|4|Accepted|0
TEST_RESULT|5|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 187, in frequencySort
    return "".join(char * freq for char, freq in Counter(s).most_common())
  File "lib/python3.6/collections/__init__.py", line 535, in __init__
    from collections import OrderedDict
  File "lib/python3.6/collections/__init__.py", line 622, in update
    seen.add(name)
TypeError: 'int' object is not iterable

EXPECTED|
TEST_RESULT|6|Accepted|0
TEST_RESULT|7|Accepted|0
TEST_RESULT|8|Accepted|0
TEST_RESULT|9|Accepted|0
TEST_RESULT|10|Accepted|0
TEST_RESULT|11|Accepted|0
TEST_RESULT|12|Accepted|0
TEST_RESULT|13|Accepted|0
TEST_RESULT|14|Accepted|0
TEST_RESULT|15|Accepted|0
TEST_RESULT|16|Accepted|0
TEST_RESULT|17|Accepted|0
TEST_RESULT|18|Accepted|0
TEST_RESULT|19|Accepted|0
TEST_RESULT|20|Accepted|0
TEST_RESULT|21|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 187, in frequencySort
    return "".join(char * freq for char, freq in Counter(s).most_common())
  File "lib/python3.6/collections/__init__.py", line 535, in __init__
    from collections import OrderedDict
  File "lib/python3.6/collections/__init__.py", line 622, in update
    seen.add(name)
TypeError: 'int' object is not iterable

EXPECTED|
TEST_RESULT|22|Accepted|0
TEST_RESULT|23|Accepted|0
TEST_RESULT|24|Accepted|0
TEST_RESULT|25|Runtime Error|0
OUTPUT|TrFile "/src/user_code.py", line 200ne 382ne 564ne 746ne 928 1110, in run_all_tests
    res.append(node.val)
  File "<string>", line 1, in <module>
  File "/src/user_code.py", line 187, in frequencySort
    return "".join(char * freq for char, freq in Counter(s).most_common())
  File "lib/python3.6/collections/__init__.py", line 535, in __init__
    from collections import OrderedDict
  File "lib/python3.6/collections/__init__.py", line 622, in update
    seen.add(name)
TypeError: 'float' object is not iterable

EXPECTED|

```
## Stderr
```
Loading test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/medium/sort_characters_by_frequency.json"
Loaded 26 test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/medium/sort_characters_by_frequency.json"
Loaded 26 test cases
PythonRunner: starting wasmer... args: QList("run", "--mapdir", "/src:.", "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/tools/wasmer/packages/python", "--", "/src/user_code.py")
PythonRunner: waiting for finish...
PythonRunner: finished. exitCode: 0
PythonRunner: returning result

```
