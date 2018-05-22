# Grade Information

## Test 1: Compilation

No dead/leftover/unnecessary code should be highlighted here. [1 pts]

```
+ gcc -Wall -g -pthread -std=c99 mine.c -o mine
+ test_end
+ return=0
+ [Output Lines: 0]
+ [Time elapsed: 0.11632]
```

## Test 2: Invalid number of threads

```
+ timeout 5 ./mine 0 1 'this is a test'
Difficulty Mask: 01111111111111111111111111111111
Number of threads must be greater than 0+ test_end
+ return=1
+ [Output Lines: 2]
+ [Time elapsed: 0.0202498]
```

## Test 3: Invalid difficulty I

```
+ timeout 5 ./mine 1 -5 'this is a test'
Difficulty Mask: 11111111111111111111111111111111
Difficulty mask must be in between 1 and 32+ test_end
+ return=1
+ [Output Lines: 2]
+ [Time elapsed: 0.0199399]
```

## Test 4: Invalid difficulty II

```
+ timeout 5 ./mine 1 50 'this is a test'
Difficulty Mask: 00000000000000000000000000000000
Difficulty mask must be in between 1 and 32+ test_end
+ return=1
+ [Output Lines: 2]
+ [Time elapsed: 0.0189998]
```

## Test 5: Difficulty Calculation

```
+ timeout 5 ./mine 1 1 'this is a test'
+ timeout 5 ./mine 1 2 'this is a test'
+ timeout 5 ./mine 1 4 'this is a test'
+ timeout 5 ./mine 1 10 'this is a test'
+ timeout 5 ./mine 1 17 'this is a test'
+ timeout 5 ./mine 1 32 'this is a test'
Difficulty Mask: 00000000000000000000000000000000
Number of threads: 1
....+ test_end
+ return=124
+ [Output Lines: 3]
+ [Time elapsed: 30.072]
```

## Test 6: Single thread run

With one thread, the number of hashes should (roughly) equal the nonce.

```
+ timeout 10 ./mine 1 21 'testing testing 1 2 3'
Difficulty Mask: 00000000000000000000011111111111
Number of threads: 1
.+ test_end
+ return=124
+ [Output Lines: 3]
+ [Time elapsed: 10.0263]
```

## Test 7: Input String: testing testing 1 2 3

```
+ timeout 60 ./mine 8 24 'testing testing 1 2 3'
/Users/matthew/Documents/classroom-utils/220-p3: line 64: 36478 Segmentation fault: 11  timeout 60 ./mine 8 24 "${str}"
+ test_end
+ return=139
+ [Output Lines: 1]
+ [Time elapsed: 0.18226]
```

## Test 8: Input String: iewoijeoiwfjwiojpqwjj

```
+ timeout 5 ./mine 1 22 iewoijeoiwfjwiojpqwjj
Difficulty Mask: 00000000000000000000001111111111
Number of threads: 1
...+ test_end
+ return=124
+ [Output Lines: 3]
+ [Time elapsed: 5.0292]
```

## Test 9: Input String: what what what what what what what

```
+ timeout 5 ./mine 4 10 'what what what what what what what'
/Users/matthew/Documents/classroom-utils/220-p3: line 86: 36530 Segmentation fault: 11  timeout 5 ./mine 4 10 "${str}"
+ test_end
+ return=139
+ [Output Lines: 1]
+ [Time elapsed: 0.0202801]
```

## Test 10: Input String: hellooooooooooo world!

```
+ timeout 5 ./mine 8 3 'hellooooooooooo world!'
+ test_end
+ return=124
+ [Output Lines: 0]
+ [Time elapsed: 5.02923]
```

## Test 11: Input String: long computation

```
+ timeout 5 ./mine 8 22 'long computation'
+ test_end
+ return=124
+ [Output Lines: 0]
+ [Time elapsed: 5.02851]
```

## Test 12: Baseline (single thread) vs four threads

```
+ timeout 10 ./mine 1 22 'Hello CS220!'
Difficulty Mask: 00000000000000000000001111111111
Number of threads: 1
.+ timeout 10 ./mine 4 22 'Hello CS220!'
Difficulty Mask: 00000000000000000000001111111111
Number of threads: 4
.Solution found by thread 0:
Nonce: 1079495
Hash: 000002FEE38B31856A2F75732B0297F98AC6FE6B
1080000 hashes in 0.32s (3385463.48 hashes/sec)
+ test_end
+ return=0
+ [Output Lines: 9]
+ [Time elapsed: 10.3629]
```


## Deductions

* (Test 5): Incorrect difficulty calculation [-1]
* (Test 6): Incorrect hash count [-1]
* (Test 7): Segfault [-1]
* (Test 8): Invalid nonce/hash or timeout [-1]
* (Test 9): Invalid nonce/hash or timeout [-0]
* (Test 10): Invalid nonce/hash or timeout [-0]
* (Test 11): Invalid nonce/hash or timeout [-0]
* Final results occasionally do not print [-1]
