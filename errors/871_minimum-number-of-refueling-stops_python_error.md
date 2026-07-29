# Error for Problem 871 (python)

## Stdout
```
COMPILATION_ERROR|TFile "/src/user_code.py", line 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1, in <module>
    run_all_tests()
  File "/src/user_code.py", line 222, in run_all_tests
    exec(oracle_code, globals())
  File "<string>", line 4, in <module>
  File "<string>", line 5, in Checker
TypeError: 'type' object is not subscriptable


```
## Stderr
```
Loading test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/hard/minimum_number_of_refueling_stops.json"
Loaded 37 test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/hard/minimum_number_of_refueling_stops.json"
Loaded 37 test cases
PythonRunner: starting wasmer... args: QList("run", "--mapdir", "/src:.", "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/tools/wasmer/packages/python", "--", "/src/user_code.py")
PythonRunner: waiting for finish...
PythonRunner: finished. exitCode: 1
PythonRunner: returning result

```
