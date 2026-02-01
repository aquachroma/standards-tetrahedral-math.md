# **V0003 Golden Waveform Template**  
### Symmetric lattice, error_sum > epsilon — FALSE case  
*(Informative)*

## **1. Expected Behavior Summary**

| Signal | Expected Value |
|--------|----------------|
| warp_sum | (0,0,0) |
| error_sum | 2 |
| symmetry_ok | 1 |
| error_ok | 0 (2 > epsilon) |
| true_delivery | 0 |

## **2. Expected State Progression**

Same as V0001.

## **3. Expected Signal Transitions**

- `error_sum` increments to 2 in ACCUMULATE  
- `symmetry_ok` transitions 0 → 1 in CHECK  
- `error_ok` remains 0  
- `true_delivery` remains 0  
- Seal boundary still occurs, but seal encodes a FALSE result  

## **4. ASCII Waveform Scaffold**

```
error_sum:  0      2         2       2       2       2       2

symmetry_ok:                        0 → 1
error_ok:                           0
true_delivery:                      0

seal_start:                                   |──|
seal_ready:                                       |────────|
```

---
