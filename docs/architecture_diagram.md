Here’s a clean **`docs/architecture_diagram.md`** using Mermaid that matches the architecture doc you just locked in.

---

# **ISO‑16 Architecture Diagram**
### High‑Level Component and Data Flow Overview  
*(Informative)*

The following Mermaid diagram illustrates the core ISO‑16 components, their relationships, and the execution flow from plugins through the True Delivery Loop to the Tetra‑Seal.

```mermaid
flowchart LR
    subgraph PL[Plugin Layer]
        P1["Plugin 0<br/>warp_vector, error, metadata"]
        P2["Plugin 1"]
        Pn["Plugin N"]
    end

    subgraph ACC[Accumulator Layer]
        WS["warp_sum (x,y,z)"]
        ES["error_sum"]
    end

    subgraph LAT[Phase Lattice]
        L0["canonical_lattice"]
        L1["phase_warped<br/>4x4 Q16.16"]
    end

    subgraph CHECK[Check Layer]
        SYM["symmetry_ok"]
        ERR["error_ok"]
        TD["true_delivery"]
    end

    subgraph SEAL[Seal Engine]
        SER["canonical_serialization"]
        H["SHA3-256"]
        S["seal[255:0]<br/>Tetra-Seal"]
    end

    P1 --> ACC
    P2 --> ACC
    Pn --> ACC

    ACC --> WS
    ACC --> ES

    L0 --> L1
    WS --> L1

    L1 --> SYM
    ES --> ERR

    SYM --> TD
    ERR --> TD

    TD --> SER
    WS --> SER
    ES --> SER
    SYM --> SER
    ERR --> SER
    L1 --> SER

    SER --> H --> S

```

---

## **State Machine Overview**

```mermaid
stateDiagram-v2
    [*] --> IDLE
    IDLE --> COLLECT
    COLLECT --> ACCUMULATE
    ACCUMULATE --> APPLY
    APPLY --> CHECK
    CHECK --> SEAL
    SEAL --> DONE
    DONE --> [*]
```

These diagrams correspond directly to the components and flow described in `architecture.md`, `true_delivery_loop.md`, and the waveform documentation.