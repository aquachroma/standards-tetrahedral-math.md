`timescale 1ns/1ps
// -----------------------------------------------------------------------------
// ISO‑16 Plugin EPSILON (informative reference plugin)
// -----------------------------------------------------------------------------
// EPSILON models saturation / clipping behavior. It produces a warp vector that
// is intentionally near the representational limits of WARP_WIDTH, allowing
// auditors to test accumulator overflow handling, symmetry sensitivity, and
// error threshold boundaries.
//
// Behavior:
//   - On start, outputs latch and remain stable for the COLLECT window.
//   - Warp vector is a clipped/saturated pattern derived from PLUGIN_ID.
//   - Error is the largest among ALPHA/BETA/GAMMA/DELTA to stress error_sum.
// -----------------------------------------------------------------------------

module plugin_epsilon #(
    parameter integer WARP_WIDTH  = 16,
    parameter integer ERROR_WIDTH = 32,
    parameter integer PLUGIN_ID   = 4
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
    // Saturation / clipping pattern (informative)
    // -------------------------------------------------------------------------
    // EPSILON produces values near the numeric limits:
    //   X = +max
    //   Y = -max
    //   Z = clipped(PLUGIN_ID * constant)
    //
    // This stresses accumulator overflow and symmetry edge cases.
    // -------------------------------------------------------------------------

    localparam [WARP_WIDTH-1:0] MAX_POS = {1'b0, {(WARP_WIDTH-1){1'b1}}};
    localparam [WARP_WIDTH-1:0] MAX_NEG = {1'b1, {(WARP_WIDTH-1){1'b0}}};

    // A deterministic but potentially large intermediate value
    wire signed [WARP_WIDTH:0] z_raw = $signed(PLUGIN_ID * 16'sh0800);

    // Clip Z to representable range
    wire [WARP_WIDTH-1:0] warp_z_const =
        (z_raw >  $signed(MAX_POS)) ? MAX_POS :
        (z_raw <  $signed(MAX_NEG)) ? MAX_NEG :
                                      z_raw[WARP_WIDTH-1:0];

    // X and Y saturate fully
    wire [WARP_WIDTH-1:0] warp_x_const = MAX_POS;
    wire [WARP_WIDTH-1:0] warp_y_const = MAX_NEG;

    // Largest error among the plugin family
    wire [ERROR_WIDTH-1:0] error_const = 32'd5;

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
