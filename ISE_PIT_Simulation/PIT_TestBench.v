`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   19:56:22 11/03/2015
// Design Name:   PIT_TopLevel
// Module Name:   C:/Users/superman/Documents/HinckWhite/ISE_PIT_Simulation/PIT_TestBench.v
// Project Name:  ISE_PIT_Simulation
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: PIT_TopLevel
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module PIT_TestBench;

	// Inputs
	reg CLOCK;
	reg DIRECTION;

	// Outputs
	wire [3:0] COUNT;
	wire LED;
	reg RST;

	// Instantiate the Unit Under Test (UUT)
	PIT_TopLevel uut (
		.CLOCK(CLOCK), 
		.DIRECTION(DIRECTION), 
		.COUNT(COUNT), 
		.RST(RST), 
		.LED(LED)
	);
	
	always
		#10 CLOCK = ~CLOCK;

	initial begin
		// Initialize Inputs
		CLOCK = 0;
		DIRECTION = 1;
		RST = 1;

		// Wait 100 ns for global reset to finish
		#10;
		RST = 0;
		DIRECTION = 0;
		
        
		// Add stimulus here

	end
      
endmodule

