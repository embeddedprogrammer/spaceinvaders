`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    19:44:53 11/03/2015 
// Design Name: 
// Module Name:    PIT_TopLevel 
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
module PIT_TopLevel(
    input CLOCK,
		input DIRECTION,
		input RST,
    output reg [3:0] COUNT,
    output LED
    );

	always @(posedge CLOCK or RST)
		if(RST)
			COUNT <= 0;
		else
			if(DIRECTION)
				COUNT <= COUNT + 1;
			else
				COUNT <= COUNT - 1;

endmodule
