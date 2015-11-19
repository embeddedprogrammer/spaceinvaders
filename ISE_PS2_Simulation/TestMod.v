`timescale 1ns / 100ps

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
		Resetn = 1;  //End reset
		#400_000;    //Wait to ensure mouse doesn't randomly do stuff.
		CLK_OUT = 0; //Pull clock line low
		CLK_T = 0;
		#100_000; 
		CLK_T = 1;   //Release clock
		Load = 1;    //Load to start transmitting bits.
		D_T = 0;     //Pull data line low
		#10;
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
		#100_000;
		CLK_OUT = 0;
		CLK_T = 0;
		#100_000;  //Pull clock low for 100us
		D_T = 0;   //Pull data line low and release clock
		CLK_T = 1;
		D_OUT = 0; //Start Bit
		#10;
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
		#50_000 CLK = ~CLK; //Clock at 10 KHz

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
		#10;
		Load = 0;
	end
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
		CLK = 1; #25_000; Din = 0; #25_000; CLK = 0; #50_000;  //Start Bit
		CLK = 1; #25_000; Din = 1; #25_000; CLK = 0; #50_000;  //b0
		CLK = 1; #25_000; Din = 1; #25_000; CLK = 0; #50_000;  //b1
		CLK = 1; #25_000; Din = 0; #25_000; CLK = 0; #50_000;  //b2
		CLK = 1; #25_000; Din = 1; #25_000; CLK = 0; #50_000;  //b3
		CLK = 1; #25_000; Din = 0; #25_000; CLK = 0; #50_000;  //b4
		CLK = 1; #25_000; Din = 0; #25_000; CLK = 0; #50_000;  //b5
		CLK = 1; #25_000; Din = 1; #25_000; CLK = 0; #50_000;  //b6
		CLK = 1; #25_000; Din = 0; #25_000; CLK = 0; #50_000;  //b7
		CLK = 1; #25_000; Din = 1; #25_000; CLK = 0; #50_000;  //Parity
		CLK = 1; #25_000; Din = 1; #25_000; CLK = 0; #50_000;  //Stop bit
		CLK = 1; //END
	end
endmodule
