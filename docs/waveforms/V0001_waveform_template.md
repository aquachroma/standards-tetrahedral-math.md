# **V0001 Golden Waveform Template**  
### Perfect symmetry, zero warp, zero error — TRUE case  
*(Informative)*

## **1. Expected Behavior Summary**

| Signal | Expected Value |
|--------|----------------|
| warp_sum | (0,0,0) |
| error_sum | 0 |
| symmetry_ok | 1 |
| error_ok | 1 |
| true_delivery | 1 |
| seal_start | 1 cycle pulse |
| seal_ready | asserts after hashing completes |

## **2. Expected State Progression**

```
IDLE → COLLECT → ACCUMULATE → APPLY → CHECK → SEAL → DONE
```

## **3. Expected Signal Transitions**

- `plugin_valid[id]` asserted only in COLLECT  
- `warp_sum` remains zero throughout  
- `error_sum` remains zero throughout  
- `symmetry_ok` transitions 0 → 1 in CHECK  
- `error_ok` transitions 0 → 1 in CHECK  
- `true_delivery` transitions 0 → 1 in CHECK  
- `seal_start` pulses high on first SEAL cycle  
- `seal_ready` asserts when hashing completes  

## **4. ASCII Waveform Scaffold**

```
clk:        ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐
           ─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─

state:     IDLE   COLLECT   ACCUM   APPLY   CHECK   SEAL    DONE

plugin_valid:      1         0       0       0       0       0

warp_sum:   0      0         0       0       0       0       0
error_sum:  0      0         0       0       0       0       0

symmetry_ok:                        0 → 1
error_ok:                           0 → 1
true_delivery:                      0 → 1

seal_start:                                   |──|
seal_ready:                                       |────────|
```

---
