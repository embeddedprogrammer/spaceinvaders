`timescale 1ns / 1ps

module PS2_sim_v2;
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
	PS2_v2 uut (
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

module PS2_v2
(
	IP_Interupt,
	bitsToSend,
	bitsReceived,
	Din,
	Dout,
  // -- DO NOT EDIT BELOW THIS LINE ------------------
  // -- Bus protocol ports, do not add to or delete 
  Bus2IP_Clk,                     // Bus to IP clock
  Bus2IP_Resetn,                  // Bus to IP reset
  Bus2IP_Data,                    // Bus to IP data bus
  Bus2IP_BE,                      // Bus to IP byte enables
  Bus2IP_RdCE,                    // Bus to IP read chip enable
  Bus2IP_WrCE,                    // Bus to IP write chip enable
  IP2Bus_Data,                    // IP to Bus data bus
  IP2Bus_RdAck,                   // IP to Bus read transfer acknowledgement
  IP2Bus_WrAck,                   // IP to Bus write transfer acknowledgement
  IP2Bus_Error                    // IP to Bus error response
  // -- DO NOT EDIT ABOVE THIS LINE ------------------
);

// -- ADD USER PARAMETERS BELOW THIS LINE ------------
// -- ADD USER PARAMETERS ABOVE THIS LINE ------------

// -- DO NOT EDIT BELOW THIS LINE --------------------
// -- Bus protocol parameters, do not add to or delete
parameter C_NUM_REG                      = 2;
parameter C_SLV_DWIDTH                   = 32;
// -- DO NOT EDIT ABOVE THIS LINE --------------------

// -- ADD USER PORTS BELOW THIS LINE -----------------
input                                     Din;
output                                    IP_Interupt;
output                                    bitsToSend;   // TODO: REMOVE
output                                    bitsReceived; // TODO: REMOVE
output                                    Dout;         
// -- ADD USER PORTS ABOVE THIS LINE -----------------

// -- DO NOT EDIT BELOW THIS LINE --------------------
// -- Bus protocol ports, do not add to or delete
input                                     Bus2IP_Clk;
input                                     Bus2IP_Resetn;
input      [C_SLV_DWIDTH-1 : 0]           Bus2IP_Data;
input      [C_SLV_DWIDTH/8-1 : 0]         Bus2IP_BE;
input      [C_NUM_REG-1 : 0]              Bus2IP_RdCE;
input      [C_NUM_REG-1 : 0]              Bus2IP_WrCE;
output     [C_SLV_DWIDTH-1 : 0]           IP2Bus_Data;
output                                    IP2Bus_RdAck;
output                                    IP2Bus_WrAck;
output                                    IP2Bus_Error;
// -- DO NOT EDIT ABOVE THIS LINE --------------------

//----------------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------------

  // Nets for user logic slave model s/w accessible register example
	reg        [10:0]                         bitsToSend;
	reg        [10:0]                         bitsReceived;
	wire                                      IP_Interupt;
	wire       [1 : 0]                        slv_reg_write_sel;
	wire       [1 : 0]                        slv_reg_read_sel;
	reg        [C_SLV_DWIDTH-1 : 0]           slv_ip2bus_data;
	wire                                      slv_read_ack;
	wire                                      slv_write_ack;
	integer                                   byte_index, bit_index;

  assign
    slv_reg_write_sel = Bus2IP_WrCE[1:0],
    slv_reg_read_sel  = Bus2IP_RdCE[1:0],
    slv_write_ack     = Bus2IP_WrCE[0] || Bus2IP_WrCE[1],
    slv_read_ack      = Bus2IP_RdCE[0] || Bus2IP_RdCE[1];

  // Counter and PIT logic
  always @(posedge Bus2IP_Clk)
	begin
		if(Bus2IP_Resetn == 1'b0)
		begin
			bitsToSend <= 11'b11111111111;
			bitsReceived <= 11'b11111111111;
		end
		else
		begin
			case(slv_reg_write_sel)
				2'b10: //reg 0 (read Din)
				begin
					bitsReceived <= Bus2IP_Data[11:0]; // TODO: Make this be a control bit.
				end
				2'b01: //reg 1 (write to Dout)
				begin
					bitsToSend[0] <= 0; // Start bit
					bitsToSend[8:1] <= Bus2IP_Data[7:0]; // Data
					bitsToSend[9] <= ~^Bus2IP_Data[7:0]; // Bitwise reduction XNOR for odd parity bit.
					bitsToSend[10] <= 1; // Stop bit
				end
				default:
				begin
					bitsToSend[9:0] <= bitsToSend[10:1]; // Shift bits to continue sending data.
					bitsToSend[10] <= 1;
					bitsReceived[9:0] <= bitsReceived[10:1]; // Shift bits to continue receiving data.
					bitsReceived[10] <= Din;
				end
			endcase
		end
	end

  // Slave register read mux
  always @(slv_reg_read_sel or bitsReceived or bitsToSend)
    begin
      case(slv_reg_read_sel)
        2'b10:
				begin
					slv_ip2bus_data <= bitsReceived[8:1];
				end
        2'b01:
					slv_ip2bus_data <= bitsToSend[8:1];
        default:
					slv_ip2bus_data <= 0;
      endcase
    end

  // Drive IP to Bus signals
	assign IP2Bus_Data = (slv_read_ack == 1'b1) ? slv_ip2bus_data : 0;
  assign IP2Bus_WrAck = slv_write_ack;
  assign IP2Bus_RdAck = slv_read_ack;
  assign IP2Bus_Error = 0;
  assign Dout = bitsToSend[0];
  assign IP_Interupt = !bitsReceived[0];
endmodule

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
	wire [10:0] bitsToSend;
	
	// Tri-state IOBUFF drivers	
	assign PS2_CLK = CLK_T ? 1'bz : CLK_OUT;
	assign CLK_IN = PS2_CLK;
	assign D = D_T ? 1'bz : D_OUT;
	assign D_IN = D_T ? D : 1;
	
	// Instantiate the Units Under Test (UUT)
	Mouse       uut1 (D, PS2_CLK);
	Transmitter uut2 (PS2_CLK, Resetn, Load, LoadVal, D_OUT, bitsToSend);
	Receiver    uut3 (PS2_CLK, Resetn, Interrupt, ReadVal, D_IN, bitsReceived);
	
//localparam [1:0]
//	IDLE = 2'd0,
//	DRIVE_CLOCK_LOW = 2'd1,
//	SENDING_DATA    = 2'd2;
//	
//reg [1:0] state;
//
//  always @(posedge CLK)
//	begin
//		case(slv_reg_write_sel)
//			2'b10: //reg 0 (read Din)
//			begin
//				bitsReceived <= Bus2IP_Data[11:0]; // TODO: Make this be a control bit.
//			end
//			2'b01: //reg 1 (write to Dout)
//			begin
//				bitsToSend[0] <= 0; // Start bit
//				bitsToSend[8:1] <= Bus2IP_Data[7:0]; // Data
//				bitsToSend[9] <= ~^Bus2IP_Data[7:0]; // Bitwise reduction XNOR for odd parity bit.
//				bitsToSend[10] <= 1; // Stop bit
//			end
//			default:
//			begin
//				bitsToSend[9:0] <= bitsToSend[10:1]; // Shift bits to continue sending data.
//				bitsToSend[10] <= 1;
//				bitsReceived[9:0] <= bitsReceived[10:1]; // Shift bits to continue receiving data.
//				bitsReceived[10] <= Din;
//			end
//		endcase
//	end

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
		Load = 0; //How do we know when the trasmitter is done?
		//D_T = 1;
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
		CLK_OUT = 0; #5; CLK_OUT = 1; #4; D_T = 0; D_OUT = 0; #1 //ACK
		CLK_OUT = 0; #5; CLK_OUT = 1; #1; D_T = 1; D_OUT = 1; #4
		CLK_T = 1;
		
		#100; //Send a character
		CLK_T = 0;
		D_T = 0;
		CLK_OUT = 1; #1; D_OUT = 0; #4; CLK_OUT = 0; #5;  //Start Bit
		CLK_OUT = 1; #1; D_OUT = 1; #4; CLK_OUT = 0; #5;  //b0
		CLK_OUT = 1; #1; D_OUT = 1; #4; CLK_OUT = 0; #5;  //b1
		CLK_OUT = 1; #1; D_OUT = 0; #4; CLK_OUT = 0; #5;  //b2
		CLK_OUT = 1; #1; D_OUT = 1; #4; CLK_OUT = 0; #5;  //b3
		CLK_OUT = 1; #1; D_OUT = 0; #4; CLK_OUT = 0; #5;  //b4
		CLK_OUT = 1; #1; D_OUT = 0; #4; CLK_OUT = 0; #5;  //b5
		CLK_OUT = 1; #1; D_OUT = 1; #4; CLK_OUT = 0; #5;  //b6
		CLK_OUT = 1; #1; D_OUT = 0; #4; CLK_OUT = 0; #5;  //b7
		CLK_OUT = 1; #1; D_OUT = 1; #4; CLK_OUT = 0; #5;  //Parity
		CLK_OUT = 1; #1; D_OUT = 1; #4; CLK_OUT = 0; #5;  //Stop bit
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

  always @(posedge CLK or ~Resetn)
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
