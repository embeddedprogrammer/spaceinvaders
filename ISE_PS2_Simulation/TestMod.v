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
