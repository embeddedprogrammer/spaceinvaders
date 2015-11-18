`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    11:30:12 11/12/2015 
// Design Name: 
// Module Name:    PS2 
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
module PS2
(
	IP_Interrupt,
	C_T,
	C_I,
	C_O,
	D_T,
	D_I,
	D_O,
	bitsReceived,
	bitsToSend,
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
output                                    IP_Interrupt;
output                                    C_T;
input                                     C_I;
output                                    C_O;
output                                    D_T;
input                                     D_I;
output                                    D_O;
output
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
	wire       [10:0]                         bitsReceived;
	wire       [11:0]                         bitsToSend;
	wire                                      IP_Interrupt;
	
	wire       [1 : 0]                        slv_reg_write_sel;
	wire       [1 : 0]                        slv_reg_read_sel;
	reg        [C_SLV_DWIDTH-1 : 0]           slv_ip2bus_data;
	wire                                      slv_read_ack;
	wire                                      slv_write_ack;
	integer                                   byte_index, bit_index;
	reg                                       sendingData;
	reg        [14]                           counter; //To pull clock line low
	reg        [2:0]                          state;
	wire                                      Load;
	reg        [7:0]                          LoadVal;

	localparam [2:0]
		IDLE_RECEIVE = 3'd0,
		CLOCK_LOW    = 3'd1,
		SEND         = 3'd2;
		WAIT         = 3'd3;
		ACK          = 3'd4;

	localparam [13:0]
		MAXCOUNT = 14'd10_000; //At 100MHz this will give us 100 micro seconds.

	assign PS2_CLK = C_T;
	assign Resetn = Bus2IP_Resetn;
	assign Interrupt = IP_Interrupt;
	assign Load = (state == IDLE) && (slv_reg_write_sel == 2'b10); //Load on transition from Idle to Clock Low
	assign LoadVal = 
	
	Transmitter transmitter1 (PS2_CLK, Resetn, Load, LoadVal, D_OUT, Done, bitsToSend);
	Receiver    receiver1    (PS2_CLK, Resetn, Interrupt, ReadVal, D_IN, bitsReceived);
	
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
			state <= IDLE;
		end
		else
		begin	
			case(state)
				IDLE:
					if(slv_reg_write_sel == 2'b10)
						state <= CLOCK_LOW;
				CLOCK_LOW:
					if(counter == MAXCOUNT) //TODO: Make counter count
						state <= SEND;
				SEND:
					if(Done == 0'b1)
						state <= WAIT;
				WAIT:
					if(D_I == 0'b1)
						state <= ACK;
				ACK:
					if(D_I == 1'b1)
						state <= IDLE;
				default:
					state <= IDLE;
			endcase
		end
	end
	
  // Slave register read mux
  always @(slv_reg_read_sel or bitsReceived or bitsToSend)
    begin
      case(slv_reg_read_sel)
        2'b10:
				begin
					slv_ip2bus_data <= bitsToSend;
				end
        2'b01:
					slv_ip2bus_data <= ReadVal;
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
  assign IP_Interrupt = !bitsReceived[0];
endmodule
