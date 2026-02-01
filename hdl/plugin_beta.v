`timescale 1ns/1ps
// -----------------------------------------------------------------------------
// ISO‑16 Plugin BETA (informative reference plugin)
// -----------------------------------------------------------------------------
// This plugin produces a deterministic "frame‑drag" style warp vector and a
// small error contribution. It is intended for testing, tutorials, and
// waveform generation.
//
// Behavior:
//   - On start, it latches its outputs and asserts plugin_valid.
//   - Outputs remain stable for the entire COLLECT window.
//   - Warp vector is a simple rotational pattern based on PLUGIN_ID.
//   - Error is a small constant distinct from plugin_alpha.
// -----------------------------------------------------------------------------

module plugin_beta #(
    parameter integer WARP_WIDTH  = 16,
    parameter integer ERROR_WIDTH = 32,
    parameter integer PLUGIN_ID   = 1
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
    // Deterministic rotational warp pattern (informative)
    // -------------------------------------------------------------------------
    // BETA produces a "rotational" warp:
    //   X = +k
    //   Y = -k/2
    //   Z = +k/4
    //
    // This makes BETA visually distinct from ALPHA in waveforms.
    // -------------------------------------------------------------------------

    wire [WARP_WIDTH-1:0] k = PLUGIN_ID * 16'h0020;

    wire [WARP_WIDTH-1:0] warp_x_const =  k;
    wire [WARP_WIDTH-1:0] warp_y_const = -k >>> 1;   // arithmetic shift for sign
    wire [WARP_WIDTH-1:0] warp_z_const =  k >>> 2;

    // Slightly larger error than ALPHA
    wire [ERROR_WIDTH-1:0] error_const = 32'd2;

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
                plugin_valid  <= 1'b1;
                plugin_warp_x <= warp_x_const;
                plugin_warp_y <= warp_y_const;
                plugin_warp_z <= warp_z_const;
                plugin_error  <= error_const;
            end
        end
    end

endmodule