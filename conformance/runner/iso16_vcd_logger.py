#!/usr/bin/env python3
"""
ISO‑16 VCD Waveform Logger (Informative)
----------------------------------------
This logger produces a canonical VCD (Value Change Dump) file that mirrors
the HDL signal set defined in iso16_true_delivery.v. It is intended for:

  • conformance debugging
  • cross‑checking HDL vs C++/Python behavior
  • GTKWave visualization
"""

import time
import datetime

class ISO16VCDLogger:
    def __init__(self, filename="iso16_trace.vcd"):
        self.filename = filename
        self.start_time = time.time()
        self.f = open(filename, "w")

        self._write_header()
        self._define_signals()

        # VCD identifier codes (short ASCII tokens)
        self.ids = {
            "state": "s",
            "cycle": "c",
            "warp_sum_x": "w",
            "error_sum": "e",
            "symmetry_ok": "y",
            "error_ok": "r",
            "true_delivery": "t",
            "seal_start": "a",
            "seal_ready": "b",
            "seal_out": "z"
        }

        self.timestamp = 0

    # ----------------------------------------------------------------------
    # VCD Header
    # ----------------------------------------------------------------------
    def _write_header(self):
        now = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        self.f.write("$date\n    " + now + "\n$end\n")
        self.f.write("$version\n    ISO‑16 Python VCD Logger\n$end\n")
        self.f.write("$timescale 1ns $end\n")

    # ----------------------------------------------------------------------
    # Declare signals
    # ----------------------------------------------------------------------
    def _define_signals(self):
        self.f.write("$scope module iso16 $end\n")

        self.f.write("$var wire 4 s state $end\n")
        self.f.write("$var wire 32 c cycle $end\n")
        self.f.write("$var wire 32 w warp_sum_x $end\n")
        self.f.write("$var wire 32 e error_sum $end\n")
        self.f.write("$var wire 1 y symmetry_ok $end\n")
        self.f.write("$var wire 1 r error_ok $end\n")
        self.f.write("$var wire 1 t true_delivery $end\n")
        self.f.write("$var wire 1 a seal_start $end\n")
        self.f.write("$var wire 1 b seal_ready $end\n")
        self.f.write("$var wire 256 z seal_out $end\n")

        self.f.write("$upscope $end\n")
        self.f.write("$enddefinitions $end\n\n")

    # ----------------------------------------------------------------------
    # Emit timestamp
    # ----------------------------------------------------------------------
    def _tick(self):
        self.timestamp += 1
        self.f.write(f"#{self.timestamp}\n")

    # ----------------------------------------------------------------------
    # Write a value change
    # ----------------------------------------------------------------------
    def write(self, signal, value):
        self._tick()

        sid = self.ids[signal]

        if isinstance(value, int):
            # Binary dump for multi‑bit signals
            if signal == "seal_out":
                self.f.write(f"b{value:0256b} {sid}\n")
            elif signal in ("warp_sum_x", "error_sum", "cycle"):
                self.f.write(f"b{value:032b} {sid}\n")
            elif signal == "state":
                self.f.write(f"b{value:04b} {sid}\n")
            else:
                self.f.write(f"{value}{sid}\n")
        else:
            # Strings (rare)
            self.f.write(f"{value}{sid}\n")

    # ----------------------------------------------------------------------
    # Close VCD file
    # ----------------------------------------------------------------------
    def close(self):
        self.f.write("\n$comment\nISO‑16 VCD Logger Closed\n$end\n")
        self.f.close()


# --------------------------------------------------------------------------
# Example usage (informative)
# --------------------------------------------------------------------------
if __name__ == "__main__":
    vcd = ISO16VCDLogger("example_iso16.vcd")

    vcd.write("state", 1)          # COLLECT
    vcd.write("warp_sum_x", 0x4B82)
    vcd.write("error_sum", 0)
    vcd.write("symmetry_ok", 1)
    vcd.write("error_ok", 1)

    vcd.write("seal_start", 1)
    vcd.write("seal_out", int("d8e4f0c4e9b6a5b8"*4, 16))
    vcd.write("seal_ready", 1)

    vcd.close()
