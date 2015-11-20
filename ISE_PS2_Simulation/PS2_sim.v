`timescale 1ns / 1ps

module PS2_sim;
	// Inputs
	reg Bus2IP_Clk;
	reg Bus2IP_Resetn;
	reg [31:0] Bus2IP_Data;
	reg [3:0] Bus2IP_BE;
	reg [4:0] Bus2IP_RdCE;
	reg [4:0] Bus2IP_WrCE;

	// Outputs
	wire IP_Interupt;
	wire [31:0] IP2Bus_Data;
	wire IP2Bus_RdAck;
	wire IP2Bus_WrAck;
	wire IP2Bus_Error;
	
	// Inouts
	wire PS2_CLK;
	wire PS2_D;
	
	// Debug outputs
	wire [10:0] bitsReceived;
	wire [11:0] bitsToSend;
	wire [2:0] state;
	wire [14:0] counter;
	wire Load;
	wire [7:0] LoadVal;
	wire [7:0] ReadVal;
	
	
	// Instantiate the Units Under Test (UUT)
	PS2 uut1 (IP_Interupt, C_T, C_I, C_O, D_T, D_I, D_O, bitsReceived, bitsToSend, state, counter, Load, LoadVal, ReadVal,
		Bus2IP_Clk, Bus2IP_Resetn, Bus2IP_Data, Bus2IP_BE, Bus2IP_RdCE, Bus2IP_WrCE, IP2Bus_Data, IP2Bus_RdAck, IP2Bus_WrAck, IP2Bus_Error);
		
	Mouse uut2 (PS2_D, PS2_CLK);
		
	// Tri-state IOBUF drivers	
	assign PS2_CLK = C_T ? 1'bz : C_O;
	assign C_I = PS2_CLK;
	assign PS2_D = D_T ? 1'bz : D_O;
	assign D_I = PS2_D;

	always
		#5 Bus2IP_Clk = ~Bus2IP_Clk; //Clock at 100 MHz
		
	initial
	begin
		// Initialize Inputs
		Bus2IP_Clk = 1;
		Bus2IP_Resetn = 0; //Reset
		Bus2IP_Data = 0;
		Bus2IP_BE = 0;
		Bus2IP_RdCE = 0;
		Bus2IP_WrCE = 0;
		#10;
		Bus2IP_Resetn = 1;
		#10;
		Bus2IP_Resetn = 0;
		#10;
		Bus2IP_Resetn = 1; // End Reset.
		#10;
		writeReg(0, 8'b01001011); //Write a value to be sent to the mouse.
		
		while (!IP_Interupt) #1;  //Wait for first byte in packet
		readReg(1);
		while (IP_Interupt) #1;
		
		while (!IP_Interupt) #1;  //Wait for second byte in packet
		readReg(1);
		while (IP_Interupt) #1;
		
		while (!IP_Interupt) #1;  //Wait for third byte in packet
		readReg(1);
		while (IP_Interupt) #1;
		
	end
	task writeReg;
		input [31:0] regNum;
		input [31:0] writeVal;
		begin
			Bus2IP_Data = writeVal;
			Bus2IP_WrCE = 1 << (4 - regNum);
			#10;
			Bus2IP_WrCE = 0;
		end
	endtask
	task readReg;
		input [31:0] regNum;
		begin
			Bus2IP_RdCE = 1 << (4 - regNum);
			#10
			Bus2IP_RdCE = 0;
		end
	endtask
endmodule
