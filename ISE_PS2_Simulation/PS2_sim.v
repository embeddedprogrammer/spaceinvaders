`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   11:30:28 11/12/2015
// Design Name:   PS2
// Module Name:   C:/Users/superman/Documents/HinckWhite/ISE_PS2_Simulation/PS2_sim.v
// Project Name:  ISE_PS2_Simulation
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: PS2
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module PS2_sim;
	// Inputs
	reg Bus2IP_Clk;
	reg Bus2IP_Resetn;
	reg [31:0] Bus2IP_Data;
	reg [3:0] Bus2IP_BE;
	reg [1:0] Bus2IP_RdCE;
	reg [1:0] Bus2IP_WrCE;
	wire Din;

	// Outputs
	wire IP_Interupt;
	wire [10:0] bitsReceived;
	wire [10:0] bitsToSend;
	wire Dout;
	wire [31:0] IP2Bus_Data;
	wire IP2Bus_RdAck;
	wire IP2Bus_WrAck;
	wire IP2Bus_Error;
	
	// Instantiate the Unit Under Test (UUT)
	PS2 uut (
		.IP_Interupt(IP_Interupt), 
		.bitsReceived(bitsReceived),
		.bitsToSend(bitsToSend),
		.Dout(Dout),
		.Din(Din),
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
	
	assign Din = Dout;

	always
		#5 Bus2IP_Clk = ~Bus2IP_Clk; //Clock at 100 MHz

	initial begin
		// Initialize Inputs
		Bus2IP_Clk = 1;
		Bus2IP_Resetn = 0;
		Bus2IP_Data = 0;
		Bus2IP_BE = 0;
		Bus2IP_RdCE = 0;
		Bus2IP_WrCE = 0;

		// Reset
		#10;
		Bus2IP_Resetn = 1;
		
// Test writing a character
		writeReg(1, 8'b01001011);
		
// Read character
		while (!IP_Interupt)
			#1;
		readReg(0);	
		
	end
	task writeReg;
		input [31:0] regNum;
		input [31:0] writeVal;
		begin
			Bus2IP_Data = writeVal;
			Bus2IP_WrCE = 1 << (1 - regNum);
			#10;
			Bus2IP_WrCE = 0;
		end
	endtask
	task readReg;
		input [31:0] regNum;
		begin
			Bus2IP_RdCE = 1 << (1 - regNum);
			#10
			Bus2IP_RdCE = 0;
		end
	endtask
endmodule
