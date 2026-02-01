`timescale 1ns/1ps
// -----------------------------------------------------------------------------
// ISO‑16 Plugin DELTA (informative reference plugin)
// -----------------------------------------------------------------------------
// DELTA models a slow drift or bias accumulation. It produces a small,
// directional warp vector and a modest error contribution.
//
// Behavior:
//   - On start, it latches its outputs and asserts plugin_valid.
//   - Outputs remain stable for the entire COLLECT window.
//   - Warp vector is a small directional drift based on PLUGIN_ID.
//   - Error is distinct from ALPHA/BETA/GAMMA.
// -----------------------------------------------------------------------------

module plugin_delta #(
    parameter integer WARP_WIDTH  = 16,
    parameter integer ERROR_WIDTH = 32,
    parameter integer PLUGIN_ID   = 3
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
    // Deterministic drift/bias pattern (informative)
    // -------------------------------------------------------------------------
    // DELTA produces a small directional drift:
    //   X = +k
    //   Y = +k/3
    //   Z = -k/5
    //
    // This gives a gentle, asymmetric bias that is easy to see in waveforms.
    // -------------------------------------------------------------------------

    wire [WARP_WIDTH-1:0] k = PLUGIN_ID * 16'h0006;

    wire [WARP_WIDTH-1:0] warp_x_const =  k;
    wire [WARP_WIDTH-1:0] warp_y_const =  k / 3;
    wire [WARP_WIDTH-1:0] warp_z_const = -(k / 5);

    // Distinct error contribution
    wire [ERROR_WIDTH-1:0] error_const = 32'd4;

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
