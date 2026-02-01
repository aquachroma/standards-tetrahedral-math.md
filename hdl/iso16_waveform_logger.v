/*
 * ISO‑16 Waveform Logger (Informative)
 * ------------------------------------
 * This module provides a canonical tap point for observing the
 * True Delivery Loop state machine and associated accumulators,
 * checks, and seal boundary signals.
 *
 * It is intended for:
 *   • simulation waveform capture (VCD/FSDB)
 *   • FPGA ILA / logic analyzer probing
 *   • cross‑domain verification against the Python VCD logger
 *
 * The logger is INFORMATIVE.
 * The signals it observes are NORMATIVE.
 */

module iso16_waveform_logger (
    input  wire         clk,
    input  wire         rst_n,

    // Core state and cycle counter
    input  wire [3:0]   state_i,
    input  wire [31:0]  cycle_i,

    // Accumulation and check signals
    input  wire [31:0]  warp_sum_x_i,
    input  wire [31:0]  error_sum_i,
    input  wire         symmetry_ok_i,
    input  wire         error_ok_i,
    input  wire         true_delivery_i,

    // Seal boundary signals
    input  wire         seal_start_i,
    input  wire         seal_ready_i,
    input  wire [255:0] seal_out_i,

    // Latched check results at SEAL boundary (informative)
    output wire         symmetry_latched_o,
    output wire         error_latched_o
);

    // --------------------------------------------------------------------
    // Internal anchor registers
    // --------------------------------------------------------------------
    // These registers capture the check results at the SEAL state boundary.
    // This provides a stable reference for waveform inspection and ILA
    // triggering without altering functional behavior.
    //
    // SEAL state encoding (4'h5) must match iso16_true_delivery.v.
    // --------------------------------------------------------------------
    reg symmetry_latched_q;
    reg error_latched_q;

    assign symmetry_latched_o = symmetry_latched_q;
    assign error_latched_o    = error_latched_q;

	parameter [3:0] ADDR_STATE_SEAL = 4'h5;
	
	...
	
	always @(posedge clk or negedge rst_n) begin
	    if (!rst_n) begin
	        symmetry_latched_q <= 1'b0;
	        error_latched_q    <= 1'b0;
	    end else begin
	        if (state_i == ADDR_STATE_SEAL) begin
	            symmetry_latched_q <= symmetry_ok_i;
	            error_latched_q    <= error_ok_i;
	        end
	    end
	end


    // --------------------------------------------------------------------
    // Simulation‑only annotation hook
    // --------------------------------------------------------------------
    // This block is ignored by synthesis tools and is provided solely
    // for simulator logs and human‑readable traces.
    // --------------------------------------------------------------------
    // synthesis translate_off
    initial begin
        $display("[ISO‑16] Waveform logger active.");
        $display("[ISO‑16] Observing: state, cycle, warp_sum_x, error_sum,");
        $display("[ISO‑16]            symmetry_ok, error_ok, true_delivery,");
        $display("[ISO‑16]            seal_start, seal_ready, seal_out.");
    end
    // synthesis translate_on

endmodule
