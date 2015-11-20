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
  // -- ADD USER PORTS BELOW THIS LINE ---------------
	IP_Interrupt,
	C_T,
	C_I,
	C_O,
	D_T,
	D_I,
	D_O,
	bitsReceived,
	bitsToSend,
	state,
	counter,
	Load,
	LoadVal,
	ReadVal,
  // -- ADD USER PORTS ABOVE THIS LINE ---------------

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
); // user_logic

// -- ADD USER PARAMETERS BELOW THIS LINE ------------
localparam [2:0]
	IDLE_RECEIVE = 3'd0,
	CLOCK_LOW    = 3'd1,
	SEND         = 3'd2,
	WAIT         = 3'd3,
	ACK          = 3'd4;

localparam [13:0]
	MAXCOUNT = 14'd10_000; //At 100MHz this will give us 100 micro seconds.
// -- ADD USER PARAMETERS ABOVE THIS LINE ------------

// -- DO NOT EDIT BELOW THIS LINE --------------------
// -- Bus protocol parameters, do not add to or delete
parameter C_NUM_REG                      = 5;
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
output                                    bitsReceived;
output                                    bitsToSend;
output                                    state;
output                                    counter;
output                                    Load;
output                                    LoadVal;
output                                    ReadVal;
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
	
  wire       [4 : 0]                        slv_reg_write_sel;
  wire       [4 : 0]                        slv_reg_read_sel;
	reg        [C_SLV_DWIDTH-1 : 0]           slv_ip2bus_data;
	wire                                      slv_read_ack;
	wire                                      slv_write_ack;
	reg        [14:0]                         counter; //To pull clock line low
	reg        [2:0]                          state;
	wire                                      Load;
	reg        [7:0]                          LoadVal;
	wire       [7:0]                          ReadVal;
	wire                                      Interrupt;

	assign Resetn = Bus2IP_Resetn;
	assign IP_Interrupt = Interrupt;
	assign Load = (state == CLOCK_LOW) && (counter == MAXCOUNT); //Load on transition from CLOCK_LOW to SEND
	
	assign PS2_CLK = C_I;
	assign C_O = (state == CLOCK_LOW) ? 0 : 1;
	assign C_T = (state == CLOCK_LOW) ? 0 : 1;
	
	assign D_IN = (state == IDLE_RECEIVE) ? D_I : 1;
	assign D_O = D_OUT;
	assign D_T = (state == SEND) ? 0 : 1;
	
	Transmitter transmitter1 (PS2_CLK, Resetn, Load, LoadVal, D_OUT, Done, bitsToSend);
	Receiver    receiver1    (PS2_CLK, Resetn, Interrupt, ReadVal, D_IN, bitsReceived);
	
  assign
    slv_reg_write_sel = Bus2IP_WrCE[4:0],
    slv_reg_read_sel  = Bus2IP_RdCE[4:0],
    slv_write_ack     = Bus2IP_WrCE[0] || Bus2IP_WrCE[1] || Bus2IP_WrCE[2] || Bus2IP_WrCE[3] || Bus2IP_WrCE[4],
    slv_read_ack      = Bus2IP_RdCE[0] || Bus2IP_RdCE[1] || Bus2IP_RdCE[2] || Bus2IP_RdCE[3] || Bus2IP_RdCE[4];		

  // State Machine
  always @(posedge Bus2IP_Clk)
	begin
		if(Bus2IP_Resetn == 1'b0)
		begin
			state <= IDLE_RECEIVE;
			counter <= 0;
			LoadVal <= 0;
		end
		else
		begin	
			case(state)
				IDLE_RECEIVE:
					if(slv_reg_write_sel == 5'b10000)
					begin
						state <= CLOCK_LOW;
						counter <= 0;
					end
				CLOCK_LOW:			
					if(counter == MAXCOUNT)
						begin
							state <= SEND;
							counter <= 0;
						end
					else
						counter <= counter + 1;
				SEND:
					if(Done == 1'b1)
						state <= WAIT;
				WAIT:
					if(D_I == 1'b0)
						state <= ACK;
				ACK:
					if(D_I == 1'b1)
						state <= IDLE_RECEIVE;
				default:
					state <= IDLE_RECEIVE;
			endcase
			if(slv_reg_write_sel == 5'b10000)
				LoadVal <= Bus2IP_Data;
		end
	end
	
  // Slave register read mux
  always @(slv_reg_read_sel or bitsToSend or ReadVal or state or bitsReceived or counter)
    begin
      case(slv_reg_read_sel)
        5'b10000:
					slv_ip2bus_data <= bitsToSend;
        5'b01000:
					slv_ip2bus_data <= ReadVal;
        5'b00100:
					slv_ip2bus_data <= state;
        5'b00010:
					slv_ip2bus_data <= bitsReceived;
        5'b00001:
					slv_ip2bus_data <= counter;
        default:
					slv_ip2bus_data <= 0;
      endcase
    end

  // Drive IP to Bus signals
	assign IP2Bus_Data = (slv_read_ack == 1'b1) ? slv_ip2bus_data : 0;
  assign IP2Bus_WrAck = slv_write_ack;
  assign IP2Bus_RdAck = slv_read_ack;
  assign IP2Bus_Error = 0;
endmodule
