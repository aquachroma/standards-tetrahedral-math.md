`timescale 1ns/1ps
// -----------------------------------------------------------------------------
// ISO‑16 Plugin ALPHA (informative reference plugin)
// -----------------------------------------------------------------------------
// This plugin produces a deterministic warp vector and error value. It is
// intended for testing, tutorials, and waveform generation.
//
// Behavior:
//   - On start, it latches its outputs and asserts plugin_valid.
//   - Outputs remain stable for the entire COLLECT window.
//   - Warp vector is a simple deterministic function of plugin ID.
//   - Error is a small constant.
// -----------------------------------------------------------------------------

module plugin_alpha #(
    parameter integer WARP_WIDTH  = 16,
    parameter integer ERROR_WIDTH = 32,
    parameter integer PLUGIN_ID   = 0
)(
    input  wire                     clk,
    input  wire                     rst_n,
    input  wire                     start,

    // Plugin outputs
    output reg                      plugin_valid,
    output reg [WARP_WIDTH-1:0]     plugin_warp_x,
    output reg [WARP_WIDTH-1:0]     plugin_warp_y,
    output reg [WARP_WIDTH-1:0]     plugin_warp_z,
    output reg [ERROR_WIDTH-1:0]    plugin_error
);

    // -------------------------------------------------------------------------
    // Deterministic warp/error generation (informative)
    // -------------------------------------------------------------------------
    // These values are arbitrary but stable and deterministic. They allow
    // auditors to see plugin behavior clearly in waveforms.
    // -------------------------------------------------------------------------

    wire [WARP_WIDTH-1:0] warp_x_const = PLUGIN_ID * 16'h0010;
    wire [WARP_WIDTH-1:0] warp_y_const = PLUGIN_ID * 16'h0008;
    wire [WARP_WIDTH-1:0] warp_z_const = PLUGIN_ID * 16'h0004;

    wire [ERROR_WIDTH-1:0] error_const = 32'd1;  // small fixed error

    // -------------------------------------------------------------------------
    // Sequential logic: latch outputs on start, hold stable
    // -------------------------------------------------------------------------
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            plugin_valid  <= 1'b0;
            plugin_warp_x <= {WARP_WIDTH{1'b0}};
            plugin_warp_y <= {WARP_WIDTH{1'b0}};
            plugin_warp_z <= {WARP_WIDTH{1'b0}};
            plugin_error  <= {ERROR_WIDTH{1'b0}};
        end else begin
            if (start) begin
                // Latch deterministic values
                plugin_valid  <= 1'b1;
                plugin_warp_x <= warp_x_const;
                plugin_warp_y <= warp_y_const;
                plugin_warp_z <= warp_z_const;
                plugin_error  <= error_const;
            end
        end
    end

endmodule
