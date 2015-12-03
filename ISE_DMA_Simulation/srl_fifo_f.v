`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    09:47:38 12/03/2015 
// Design Name: 
// Module Name:    srl_fifo_f 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module srl_fifo_f(Clk, Reset, FIFO_Write, Data_In, Data_Out, FIFO_Read, FIFO_Full, FIFO_Empty, Addr);

parameter C_DWIDTH = 32;
parameter C_DEPTH = 16;

input wire Clk;
input wire Reset;
input wire FIFO_Write;
input wire [31:0] Data_In;
input wire [31:0] Data_Out;
input wire FIFO_Read;
output wire FIFO_Full;
output wire FIFO_Empty;
input wire [31:0] Addr;


assign FIFO_Full = 1;
assign FIFO_Empty = 1;
assign Data_Out = 32'b01010101;

endmodule
