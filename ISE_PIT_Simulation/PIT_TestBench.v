`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   20:22:08 11/03/2015
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
	reg Bus2IP_Clk;
	reg Bus2IP_Resetn;
	reg [31:0] Bus2IP_Data;
	reg [3:0] Bus2IP_BE;
	reg [1:0] Bus2IP_RdCE;
	reg [1:0] Bus2IP_WrCE;

	// Outputs
	wire IP_Interupt;
	wire [31:0] counter;
	wire [31:0] slv_reg0;
	wire [31:0] slv_reg1;
	wire [31:0] IP2Bus_Data;
	wire IP2Bus_RdAck;
	wire IP2Bus_WrAck;
	wire IP2Bus_Error;
	
	// Instantiate the Unit Under Test (UUT)
	PIT_TopLevel uut (
		.IP_Interupt(IP_Interupt), 
		.counter(counter),
		.slv_reg0(slv_reg0),
		.slv_reg1(slv_reg1),
		.Bus2IP_Clk(Bus2IP_Clk), 
		.Bus2IP_Resetn(Bus2IP_Resetn), 
		.Bus2IP_Data(Bus2IP_Data), 
		.Bus2IP_BE(Bus2IP_BE), 
		.Bus2IP_RdCE(Bus2IP_RdCE), 
		.Bus2IP_WrCE(Bus2IP_WrCE), 
		.IP2Bus_Data(IP2Bus_Data), 
		.IP2Bus_RdAck(IP2Bus_RdAck), 
		.IP2Bus_WrAck(IP2Bus_WrAck), 
		.IP2Bus_Error(IP2Bus_Error)
	);
	
	always
			#10 Bus2IP_Clk = ~Bus2IP_Clk;

	initial begin
		// Initialize Inputs
		Bus2IP_Clk = 0;
		Bus2IP_Resetn = 0;
		Bus2IP_Data = 0;
		Bus2IP_BE = 0;
		Bus2IP_RdCE = 0;
		Bus2IP_WrCE = 0;

		// Reset
		#20;
		Bus2IP_Resetn = 1;
		
// Test decrement functionality
		// Write to register 1		
		Bus2IP_RdCE = 2'b01;
		Bus2IP_Data = 30;
		#20;
		Bus2IP_WrCE = 2'b01;
		#20;
		Bus2IP_WrCE = 2'b00;
		Bus2IP_RdCE = 2'b00;
		
		// Write to register 0
		Bus2IP_RdCE = 2'b10;
		Bus2IP_Data = 32'b001; //Decrement
		#20;
		Bus2IP_WrCE = 2'b10;
		#20;
		Bus2IP_WrCE = 2'b00;
		Bus2IP_RdCE = 2'b11;
		#20;
		
		// Wait for counter to expire
		#1000;
		
// Test reload functionality
		// Write to register 0
		Bus2IP_RdCE = 2'b10;
		Bus2IP_Data = 32'b101; // Reload
		#20;
		Bus2IP_WrCE = 2'b10;
		#20;
		Bus2IP_WrCE = 2'b00;
		Bus2IP_RdCE = 2'b11;
		#20;
		
		// Wait for counter to expire
		#2000;
		
// Test interupt functionality
		// Write to register 0
		Bus2IP_RdCE = 2'b10;
		Bus2IP_Data = 32'b111; // Enable interrupts
		#20;
		Bus2IP_WrCE = 2'b10;
		#20;
		Bus2IP_WrCE = 2'b00;
		Bus2IP_RdCE = 2'b11;
		#20;
		
		// Wait for counter to expire
		#2000;
		
// Test interupt functionality without reload enable
		// Write to register 0
		Bus2IP_RdCE = 2'b10;
		Bus2IP_Data = 32'b011; // Enable interrupts
		#20;
		Bus2IP_WrCE = 2'b10;
		#20;
		Bus2IP_WrCE = 2'b00;
		Bus2IP_RdCE = 2'b11;
		#20;
		
		// Wait for counter to expire
		#2000;		
	
	end
endmodule

