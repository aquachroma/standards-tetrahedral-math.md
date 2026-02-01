`timescale 1ns/1ps
// -----------------------------------------------------------------------------
// ISO‑16 True Delivery Loop — Canonical Waveform Testbench (informative)
// -----------------------------------------------------------------------------
// This testbench instantiates:
//   - the reference RTL (iso16_true_delivery)
//   - five reference plugins (ALPHA/BETA/GAMMA/DELTA/EPSILON)
//   - the waveform logger (iso16_waveform_logger)
//
// It drives the canonical start pulses, waits for seal_ready, and dumps VCD
// waveforms that match the Golden Waveform Templates.
// -----------------------------------------------------------------------------

module tb_iso16_waveform;

    // -------------------------------------------------------------------------
    // Clock + Reset
    // -------------------------------------------------------------------------
    reg clk = 0;
    reg rst_n = 0;

    always #5 clk = ~clk;   // 100 MHz

    // -------------------------------------------------------------------------
    // DUT inputs
    // -------------------------------------------------------------------------
    reg         start = 0;
    reg [15:0]  vector_id = 16'h0001;
    reg [31:0]  epsilon    = 32'd10;

    // -------------------------------------------------------------------------
    // Plugin wiring (NUM_PLUGINS = 5)
    // -------------------------------------------------------------------------
    localparam integer NUM_PLUGINS = 5;
    localparam integer WARP_WIDTH  = 16;
    localparam integer ERROR_WIDTH = 32;

    wire [NUM_PLUGINS-1:0]                     plugin_valid;
    wire [NUM_PLUGINS*WARP_WIDTH-1:0]          plugin_warp_x;
    wire [NUM_PLUGINS*WARP_WIDTH-1:0]          plugin_warp_y;
    wire [NUM_PLUGINS*WARP_WIDTH-1:0]          plugin_warp_z;
    wire [NUM_PLUGINS*ERROR_WIDTH-1:0]         plugin_error;

    // -------------------------------------------------------------------------
    // Instantiate Plugins
    // -------------------------------------------------------------------------

    // ALPHA
    plugin_alpha #(.PLUGIN_ID(0)) P_ALPHA (
        .clk(clk), .rst_n(rst_n), .start(start),
        .plugin_valid(plugin_valid[0]),
        .plugin_warp_x(plugin_warp_x[0*WARP_WIDTH +: WARP_WIDTH]),
        .plugin_warp_y(plugin_warp_y[0*WARP_WIDTH +: WARP_WIDTH]),
        .plugin_warp_z(plugin_warp_z[0*WARP_WIDTH +: WARP_WIDTH]),
        .plugin_error(plugin_error[0*ERROR_WIDTH +: ERROR_WIDTH])
    );

    // BETA
    plugin_beta #(.PLUGIN_ID(1)) P_BETA (
        .clk(clk), .rst_n(rst_n), .start(start),
        .plugin_valid(plugin_valid[1]),
        .plugin_warp_x(plugin_warp_x[1*WARP_WIDTH +: WARP_WIDTH]),
        .plugin_warp_y(plugin_warp_y[1*WARP_WIDTH +: WARP_WIDTH]),
        .plugin_warp_z(plugin_warp_z[1*WARP_WIDTH +: WARP_WIDTH]),
        .plugin_error(plugin_error[1*ERROR_WIDTH +: ERROR_WIDTH])
    );

    // GAMMA
    plugin_gamma #(.PLUGIN_ID(2)) P_GAMMA (
        .clk(clk), .rst_n(rst_n), .start(start),
        .plugin_valid(plugin_valid[2]),
        .plugin_warp_x(plugin_warp_x[2*WARP_WIDTH +: WARP_WIDTH]),
        .plugin_warp_y(plugin_warp_y[2*WARP_WIDTH +: WARP_WIDTH]),
        .plugin_warp_z(plugin_warp_z[2*WARP_WIDTH +: WARP_WIDTH]),
        .plugin_error(plugin_error[2*ERROR_WIDTH +: ERROR_WIDTH])
    );

    // DELTA
    plugin_delta #(.PLUGIN_ID(3)) P_DELTA (
        .clk(clk), .rst_n(rst_n), .start(start),
        .plugin_valid(plugin_valid[3]),
        .plugin_warp_x(plugin_warp_x[3*WARP_WIDTH +: WARP_WIDTH]),
        .plugin_warp_y(plugin_warp_y[3*WARP_WIDTH +: WARP_WIDTH]),
        .plugin_warp_z(plugin_warp_z[3*WARP_WIDTH +: WARP_WIDTH]),
        .plugin_error(plugin_error[3*ERROR_WIDTH +: ERROR_WIDTH])
    );

    // EPSILON
    plugin_epsilon #(.PLUGIN_ID(4)) P_EPSILON (
        .clk(clk), .rst_n(rst_n), .start(start),
        .plugin_valid(plugin_valid[4]),
        .plugin_warp_x(plugin_warp_x[4*WARP_WIDTH +: WARP_WIDTH]),
        .plugin_warp_y(plugin_warp_y[4*WARP_WIDTH +: WARP_WIDTH]),
        .plugin_warp_z(plugin_warp_z[4*WARP_WIDTH +: WARP_WIDTH]),
        .plugin_error(plugin_error[4*ERROR_WIDTH +: ERROR_WIDTH])
    );

    // -------------------------------------------------------------------------
    // Instantiate DUT
    // -------------------------------------------------------------------------
    wire [2:0]               state;
    wire [WARP_WIDTH-1:0]    warp_sum_x, warp_sum_y, warp_sum_z;
    wire [ERROR_WIDTH-1:0]   error_sum;
    wire                     symmetry_ok, error_ok, true_delivery;
    wire                     seal_start, seal_ready;
    wire [255:0]             seal;

    iso16_true_delivery #(
        .NUM_PLUGINS(NUM_PLUGINS),
        .WARP_WIDTH(WARP_WIDTH),
        .ERROR_WIDTH(ERROR_WIDTH)
    ) dut (
        .clk(clk),
        .rst_n(rst_n),
        .start(start),
        .vector_id(vector_id),
        .epsilon(epsilon),
        .plugin_valid(plugin_valid),
        .plugin_warp_x(plugin_warp_x),
        .plugin_warp_y(plugin_warp_y),
        .plugin_warp_z(plugin_warp_z),
        .plugin_error(plugin_error),
        .state(state),
        .warp_sum_x(warp_sum_x),
        .warp_sum_y(warp_sum_y),
        .warp_sum_z(warp_sum_z),
        .error_sum(error_sum),
        .symmetry_ok(symmetry_ok),
        .error_ok(error_ok),
        .true_delivery(true_delivery),
        .seal_start(seal_start),
        .seal_ready(seal_ready),
        .seal(seal)
    );

    // -------------------------------------------------------------------------
    // Waveform Logger
    // -------------------------------------------------------------------------
    iso16_waveform_logger #(
        .WARP_WIDTH(WARP_WIDTH),
        .ERROR_WIDTH(ERROR_WIDTH)
    ) logger (
        .clk(clk),
        .rst_n(rst_n),
        .state(state),
        .warp_sum_x(warp_sum_x),
        .warp_sum_y(warp_sum_y),
        .warp_sum_z(warp_sum_z),
        .error_sum(error_sum),
        .symmetry_ok(symmetry_ok),
        .error_ok(error_ok),
        .true_delivery(true_delivery),
        .seal_start(seal_start),
        .seal_ready(seal_ready),
        .seal(seal)
    );

    // -------------------------------------------------------------------------
    // Test Sequence
    // -------------------------------------------------------------------------
    initial begin
        // VCD dump
        $dumpfile("iso16_waveform.vcd");
        $dumpvars(0, tb_iso16_waveform);

        // Reset
        rst_n = 0;
        #40;
        rst_n = 1;

        // First vector
        #20 start = 1;
        #10 start = 0;

        // Wait for seal_ready
        wait (seal_ready);
        #20;

        // Second vector (different ID)
        vector_id = 16'h0002;
        #20 start = 1;
        #10 start = 0;

        wait (seal_ready);
        #40;

        $display("ISO‑16 testbench complete.");
        $finish;
    end

endmodule
