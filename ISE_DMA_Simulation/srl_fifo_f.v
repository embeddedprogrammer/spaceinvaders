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
	output wire [31:0] Data_Out;
	input wire FIFO_Read;
	output wire FIFO_Full;
	output wire FIFO_Empty;
	output reg [31:0] Addr;

	assign FIFO_Full = (Addr == C_DEPTH);
	assign FIFO_Empty = (Addr == 0);
	assign Data_Out = (FIFO_Read && !FIFO_Empty) ? 32'b01010101 : 32'b0;

	always @(posedge Clk)
	begin
		if(Reset)
			Addr <= 0;
		else if(FIFO_Write && !FIFO_Full)
			Addr <= Addr + 1;
		else if(FIFO_Read && !FIFO_Empty)
			Addr <= Addr - 1;
	end
endmodule
