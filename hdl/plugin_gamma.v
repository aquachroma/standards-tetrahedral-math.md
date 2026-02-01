`timescale 1ns/1ps
// -----------------------------------------------------------------------------
// ISO‑16 Plugin GAMMA (informative reference plugin)
// -----------------------------------------------------------------------------
// This plugin produces a deterministic "jitter/noise" warp vector and a small
// error contribution. It is intended for testing accumulator sensitivity,
// symmetry robustness, and waveform visibility.
//
// Behavior:
//   - On start, it latches its outputs and asserts plugin_valid.
//   - Outputs remain stable for the entire COLLECT window.
//   - Warp vector uses a simple LFSR‑style deterministic pattern.
//   - Error is distinct from ALPHA and BETA.
// -----------------------------------------------------------------------------

module plugin_gamma #(
    parameter integer WARP_WIDTH  = 16,
    parameter integer ERROR_WIDTH = 32,
    parameter integer PLUGIN_ID   = 2
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
    // Deterministic jitter/noise pattern (informative)
    // -------------------------------------------------------------------------
    // GAMMA uses a simple LFSR‑like pattern derived from PLUGIN_ID.
    // This produces visually distinct jitter in waveforms while remaining
    // 100% deterministic and stable.
    // -------------------------------------------------------------------------

    wire [WARP_WIDTH-1:0] seed = (PLUGIN_ID * 16'h00A5) ^ 16'h5C3F;

    // Simple LFSR‑style mixing
    wire [WARP_WIDTH-1:0] warp_x_const = seed ^ (seed << 3);
    wire [WARP_WIDTH-1:0] warp_y_const = seed ^ (seed >> 2);
    wire [WARP_WIDTH-1:0] warp_z_const = (seed << 1) ^ (seed >> 1);

    // Distinct error contribution
    wire [ERROR_WIDTH-1:0] error_const = 32'd3;

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