`timescale 1ns/1ps
// -----------------------------------------------------------------------------
// ISO‑16 Waveform Logger (informative reference module)
// -----------------------------------------------------------------------------
// This module is a passive, zero‑logic waveform capture wrapper. It exposes the
// canonical ISO‑16 True Delivery Loop signals with stable, readable names so
// that VCD/FSDB waveform dumps match the Golden Waveform Templates.
//
// This module:
//   - performs NO computation
//   - adds NO timing
//   - simply re‑exports DUT signals for waveform visibility
//   - emits optional $display audit markers
//
// Vendors wrap their DUT with this module during conformance testing.
// -----------------------------------------------------------------------------

module iso16_waveform_logger #(
    parameter integer WARP_WIDTH   = 16,
    parameter integer ERROR_WIDTH  = 32
)(
    input  wire                     clk,
    input  wire                     rst_n,

    // State machine
    input  wire [2:0]               state,

    // Accumulators
    input  wire [WARP_WIDTH-1:0]    warp_sum_x,
    input  wire [WARP_WIDTH-1:0]    warp_sum_y,
    input  wire [WARP_WIDTH-1:0]    warp_sum_z,
    input  wire [ERROR_WIDTH-1:0]   error_sum,

    // Check results
    input  wire                     symmetry_ok,
    input  wire                     error_ok,
    input  wire                     true_delivery,

    // Seal boundary
    input  wire                     seal_start,
    input  wire                     seal_ready,
    input  wire [255:0]             seal
);

    // -------------------------------------------------------------------------
    // Waveform aliases (clean names for VCD/FSDB)
    // -------------------------------------------------------------------------
    // These wires exist ONLY to give waveform viewers stable, readable names.
    // They do not change logic or timing.
    // -------------------------------------------------------------------------

    wire [2:0]               wf_state       = state;

    wire [WARP_WIDTH-1:0]    wf_warp_sum_x  = warp_sum_x;
    wire [WARP_WIDTH-1:0]    wf_warp_sum_y  = warp_sum_y;
    wire [WARP_WIDTH-1:0]    wf_warp_sum_z  = warp_sum_z;
    wire [ERROR_WIDTH-1:0]   wf_error_sum   = error_sum;

    wire                     wf_symmetry_ok = symmetry_ok;
    wire                     wf_error_ok    = error_ok;
    wire                     wf_true_deliv  = true_delivery;

    wire                     wf_seal_start  = seal_start;
    wire                     wf_seal_ready  = seal_ready;
    wire [255:0]             wf_seal        = seal;

    // -------------------------------------------------------------------------
    // Optional audit markers (informative)
    // These appear in the simulator console and help auditors correlate
    // waveform transitions with state machine events.
    // -------------------------------------------------------------------------

    always @(posedge clk) begin
        if (wf_seal_start)
            $display("[%0t] ISO16: seal_start asserted", $time);

        if (wf_seal_ready)
            $display("[%0t] ISO16: seal_ready asserted", $time);

        // CHECK state = 4 in the canonical state machine
        if (wf_state == 3'd4)
            $display("[%0t] ISO16: CHECK: sym=%0d err=%0d true=%0d",
                     $time, wf_symmetry_ok, wf_error_ok, wf_true_deliv);
    end

    // -------------------------------------------------------------------------
    // NOTE:
    // This module intentionally does NOT call $dumpvars or $dumpfile.
    // The testbench controls waveform dumping so vendors can choose VCD/FSDB.
    // -------------------------------------------------------------------------

endmodule
