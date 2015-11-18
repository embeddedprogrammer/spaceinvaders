`timescale 1ns / 1ps

module TestReceiverTransmitter;
	// Inputs
	reg CLK_OUT;
	reg CLK_T;
	reg D_T;
	reg Resetn;
	reg Load;
	reg [7:0] LoadVal;
	
	// Bidirectional inouts
	wire PS2_CLK;
	wire D;
	
	// Outputs
	wire D_OUT;
	wire CLK_IN;
	wire D_IN;
	wire [7:0] ReadVal;
	wire Interrupt;
	wire [10:0] bitsReceived;
	wire [11:0] bitsToSend;
	wire Done;
	
	// Tri-state IOBUFF drivers	
	assign PS2_CLK = CLK_T ? 1'bz : CLK_OUT;
	assign CLK_IN = PS2_CLK;
	assign D = D_T ? 1'bz : D_OUT;
	assign D_IN = D_T ? D : 1;
	
	// Instantiate the Units Under Test (UUT)
	Mouse       uut1 (D, PS2_CLK);
	Transmitter uut2 (PS2_CLK, Resetn, Load, LoadVal, D_OUT, Done, bitsToSend);
	Receiver    uut3 (PS2_CLK, Resetn, Interrupt, ReadVal, D_IN, bitsReceived);

	initial begin
		// Initialize Inputs
		CLK_T = 1;
		CLK_OUT = 1;
		Resetn = 0; //Reset
		D_T = 1;
		LoadVal = 8'b01001011;
		Load = 0;
		#10;
		Resetn = 1; //End reset
		#40; //Wait to ensure mouse doesn't randomly do stuff.
		CLK_OUT = 0; //Pull clock line low
		CLK_T = 0;
		#100; 
		CLK_T = 1;//Release clock
		Load = 1; // Load to start transmitting bits.
		D_T = 0;  //Pull data line low
		#1;
		Load = 0; 
		while (!Done) #1; //Wait till transmitter is done.
		D_T = 1;
	end
endmodule
module TestMouse;
	reg CLK_OUT;
	reg CLK_T;
	wire CLK_IN;
	
	assign CLK = CLK_T ? 1'bz : CLK_OUT;
	assign CLK_IN = CLK;

	reg D_OUT;
	reg D_T;
	wire D_IN;
	
	assign D = D_T ? 1'bz : D_OUT;
	assign D_IN = D;
	
	// Instantiate the Unit Under Test (UUT)
	Mouse uut(D, CLK);
	
	initial begin
		// Initialize Inputs
		CLK_T = 1;
		CLK_OUT = 1;
		D_T = 1;
		D_OUT = 1;
		#40;
		CLK_OUT = 0;
		CLK_T = 0;
		#100; //Wait to ensure mouse doesn't randomly do stuff.
		D_T = 0;
		D_OUT = 0;
		#10;
		CLK_T = 1;
		#1;
		                   D_OUT = 0;                      //Start Bit
		while (CLK_IN) #1; D_OUT = 1; while (!CLK_IN) #1;  //b0
		while (CLK_IN) #1; D_OUT = 1; while (!CLK_IN) #1;  //b1
		while (CLK_IN) #1; D_OUT = 0; while (!CLK_IN) #1;  //b2
		while (CLK_IN) #1; D_OUT = 1; while (!CLK_IN) #1;  //b3
		while (CLK_IN) #1; D_OUT = 0; while (!CLK_IN) #1;  //b4
		while (CLK_IN) #1; D_OUT = 0; while (!CLK_IN) #1;  //b5
		while (CLK_IN) #1; D_OUT = 1; while (!CLK_IN) #1;  //b6
		while (CLK_IN) #1; D_OUT = 0; while (!CLK_IN) #1;  //b7
		while (CLK_IN) #1; D_OUT = 1; while (!CLK_IN) #1;  //Parity
		while (CLK_IN) #1; D_OUT = 1; while (!CLK_IN) #1;  //Stop bit
		D_T = 1;
	end
endmodule
module Mouse(D, CLK);
	inout D;
	inout CLK;
	
	PULLUP pullup0 (.O(CLK));
	PULLUP pullup1 (.O(D));
	
	reg CLK_OUT;
	reg CLK_T;
	wire CLK_IN;
	
	assign CLK = CLK_T ? 1'bz : CLK_OUT;
	assign CLK_IN = CLK;
	
	reg D_OUT;
	reg D_T;
	wire D_IN;
	
	assign D = D_T ? 1'bz : D_OUT;
	assign D_IN = D;	
	
	initial begin
		CLK_OUT = 0;
		CLK_T = 1;
		D_T = 1;
		D_OUT = 1;
		#10;
		
		while (CLK_IN) #1; //Wait till host pulls clock low.
		while (!CLK_IN) #1; //Wait till host releases clock.
		#15;
		CLK_T = 0;
		CLK_OUT = 0; #5; CLK_OUT = 1; #5;
		CLK_OUT = 0; #5; CLK_OUT = 1; #5;
		CLK_OUT = 0; #5; CLK_OUT = 1; #5;
		CLK_OUT = 0; #5; CLK_OUT = 1; #5;
		CLK_OUT = 0; #5; CLK_OUT = 1; #5;
		CLK_OUT = 0; #5; CLK_OUT = 1; #5;
		CLK_OUT = 0; #5; CLK_OUT = 1; #5;
		CLK_OUT = 0; #5; CLK_OUT = 1; #5;
		CLK_OUT = 0; #5; CLK_OUT = 1; #5;
		CLK_OUT = 0; #5; CLK_OUT = 1; #2.5; D_T = 0; D_OUT = 0; #2.5 //ACK
		CLK_OUT = 0; #5; CLK_OUT = 1; #2.5; D_T = 1; D_OUT = 1; #2.5
		CLK_T = 1;
		
		#100; //Send a character
		CLK_T = 0;
		D_T = 0;
		CLK_OUT = 1; #2.5; D_OUT = 0; #2.5; CLK_OUT = 0; #5;  //Start Bit
		CLK_OUT = 1; #2.5; D_OUT = 1; #2.5; CLK_OUT = 0; #5;  //b0
		CLK_OUT = 1; #2.5; D_OUT = 1; #2.5; CLK_OUT = 0; #5;  //b1
		CLK_OUT = 1; #2.5; D_OUT = 0; #2.5; CLK_OUT = 0; #5;  //b2
		CLK_OUT = 1; #2.5; D_OUT = 1; #2.5; CLK_OUT = 0; #5;  //b3
		CLK_OUT = 1; #2.5; D_OUT = 0; #2.5; CLK_OUT = 0; #5;  //b4
		CLK_OUT = 1; #2.5; D_OUT = 0; #2.5; CLK_OUT = 0; #5;  //b5
		CLK_OUT = 1; #2.5; D_OUT = 1; #2.5; CLK_OUT = 0; #5;  //b6
		CLK_OUT = 1; #2.5; D_OUT = 0; #2.5; CLK_OUT = 0; #5;  //b7
		CLK_OUT = 1; #2.5; D_OUT = 1; #2.5; CLK_OUT = 0; #5;  //Parity
		CLK_OUT = 1; #2.5; D_OUT = 1; #2.5; CLK_OUT = 0; #5;  //Stop bit
		CLK_OUT = 1;
		CLK_T = 1;
		D_T = 1;
	end
endmodule
module Transmitter_Test;
	// Inputs
	reg CLK;
	reg Resetn;
	reg Load;
	reg [7:0] LoadVal;
	
	// Outputs
	wire Dout;
	wire [11:0] bitsToSend;
	wire Done;
	
	// Instantiate the Unit Under Test (UUT)
	Transmitter uut (CLK, Resetn, Load, LoadVal, Dout, Done, bitsToSend);
	
	always
		#5 CLK = ~CLK; //Clock at 100 MHz

	initial begin
		// Initialize Inputs
		CLK = 1;
		Resetn = 0;
		Load = 0;
		LoadVal = 8'b01001011;

		// Reset
		#10;
		Resetn = 1;
		#10;
		Load = 1;
		#1;
		Load = 0;
		#9;
	end
endmodule
module Transmitter(CLK, Resetn, Load, LoadVal, Dout, Done, bitsToSend);
	input       CLK;
	input       Resetn;
	input       Load;
	input [7:0] LoadVal;
	output      Dout;
	output      Done;
	output [11:0] bitsToSend;
	
	reg  [11:0] bitsToSend;

  always @(negedge CLK or ~Resetn or posedge Load)
	begin
		if(!Resetn)
		begin
			bitsToSend <= 12'b1111_1111_1111;
		end
		else
		begin
			if(Load)
			begin
				bitsToSend[0] <= 0; // Start bit
				bitsToSend[8:1] <= LoadVal[7:0]; // Data
				bitsToSend[9] <= ~^LoadVal[7:0]; // Bitwise reduction XNOR for odd parity bit.
				bitsToSend[10] <= 1; // Stop bit
				bitsToSend[11] <= 0; // End of transmission (this bit won't actually be sent, but is needed for the SM to stop)
			end
			else if(Done)
			begin
				bitsToSend <= 12'b1111_1111_1111;
			end
			else
			begin
				bitsToSend[10:0] <= bitsToSend[11:1]; // Shift bits to continue sending data.
				bitsToSend[11] <= 1;
			end
		end
	end
	assign Dout = bitsToSend[0];
	assign Done = (bitsToSend == 12'b1111_1111_1101);
endmodule

module Receiver_Test;
	// Inputs
	reg CLK;
	reg Resetn;
	reg Din;
	
	// Outputs
	wire [7:0] ReadVal;
	wire Interrupt;
	wire [10:0] bitsReceived;
	
	// Instantiate the Unit Under Test (UUT)
	Receiver    uut2 (CLK, Resetn, Interrupt, ReadVal, Din, bitsReceived);

	initial begin
		// Initialize Inputs
		CLK = 1;
		Resetn = 0;
		Din = 1;
		#10;
		Resetn = 1;
		#10;
		//Send character
		CLK = 1; #1; Din = 0; #4; CLK = 0; #5;  //Start Bit
		CLK = 1; #1; Din = 1; #4; CLK = 0; #5;  //b0
		CLK = 1; #1; Din = 1; #4; CLK = 0; #5;  //b1
		CLK = 1; #1; Din = 0; #4; CLK = 0; #5;  //b2
		CLK = 1; #1; Din = 1; #4; CLK = 0; #5;  //b3
		CLK = 1; #1; Din = 0; #4; CLK = 0; #5;  //b4
		CLK = 1; #1; Din = 0; #4; CLK = 0; #5;  //b5
		CLK = 1; #1; Din = 1; #4; CLK = 0; #5;  //b6
		CLK = 1; #1; Din = 0; #4; CLK = 0; #5;  //b7
		CLK = 1; #1; Din = 1; #4; CLK = 0; #5;  //Parity
		CLK = 1; #1; Din = 1; #4; CLK = 0; #5;  //Stop bit
		CLK = 1; //END
	end
endmodule
module Receiver(CLK, Resetn, Interrupt, ReadVal, Din, bitsReceived);
	input         CLK;
	input         Resetn;
	output        Interrupt;
	output [7:0]  ReadVal;
	input         Din;
	output [10:0] bitsReceived;
	
	reg  [10:0] bitsReceived;

  always @(negedge CLK or ~Resetn)
	begin
		if(!Resetn || Interrupt)
		begin
			bitsReceived <= 11'b11111111111;
		end
		else
		begin
			bitsReceived[9:0] <= bitsReceived[10:1]; // Shift bits to continue receiving data.
			bitsReceived[10] <= Din;
		end
	end
  assign Interrupt = !bitsReceived[0];
  assign ReadVal = bitsReceived[8:1];
endmodule
