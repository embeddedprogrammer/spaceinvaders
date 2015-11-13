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
	IP_Interupt,
	bitpipe,
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
inout                                     PS2_D;
inout                                     PS2_CLK;
output                                    IP_Interupt;
output                                    bitpipe;      // TODO: REMOVE
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
	reg                                       sendingData;
	reg        [14]                           counter; //To pull clock line low

	reg usingPS2_CLK;
	reg usingPS2_D;
	assign PS2_D = (usingPS2_D && !Dout) ? 0 : 'bz;
	assign PS2_CLK = (usingPS2_CLK && !CLKout) ? 0 : 'bz;

	reg last_PS2_CLK;

	if !sendingData
		
	else if receivingData
	else if Din
		sendingData <= 1;
	else if 
	end

localparam [2:0]
	IDLE = 3'd0,
	DRIVE_CLOCK_LOW = 3'd1,
	SENDING_DATA    = 3'd2;
	ACKNOWLEDGE     = 3'd3;
	RECEIVING_DATA  = 3'd4;
localparam [13:0]
	MAXCOUNT = 14'd10_000; //At 100MHz this will give us 100 micro seconds.
	
reg [2:0] state;

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
			//reset
			state = IDLE;
		end
		else
		begin
			if (state == IDLE)
			begin
				if(requestToSend) //Should we allow overriding the signal when the mouse is already talking to us?
				begin
					usingPS2_CLK <= 1;
					usingPS2_D <= 1;
					CLKout <= 0;
					Dout <= 0;
					counter <= 0;
					state <= DRIVE_CLOCK_LOW;
					requestToSend <= 0;
				end
				else if(Din)
					state <= RECEIVING_DATA;
			end
			else if (state == DRIVE_CLOCK_LOW)
			begin
					if(counter < MAXCOUNT)
						counter <= counter + 1;
					else
					begin
						counter <= 0;
						state <= SENDING_DATA;
						last_PS2_CLK <= PS2_CLK;
						usingPS2_CLK <= 0;
						usingPS2_D <= 1;
						Dout <= 0; //Start bit
					end				
			end
			else if (state == SENDING_DATA)
			begin
				if(last_PS2_CLK && !PS2_CLK) //On falling edge
				begin
					counter++;
					//Need some sort of way to select bit
					// Idea 1: shift bits using registers?
					// Idea 2: bit selector (counter, like below?)
					
					//if counter == 0 
					//	Dout = 0; //Start bit
					//else 
					if(counter == 1) //Should this go outside of always block?
						Dout <= D0;
					else if(counter == 2)
						Dout <= D1;
						... //Fill in
					else if(counter == 8)
						Dout <= D7;
					else if(counter == 8)
						Dout <= parity;
					else if(counter == 9)
						usingPS2_D <= 0;
						Dout <= 1; //stop bit
						counter <= 0;
					else if(counter == 10)
						state <= ACKNOWLEDGE;
					end
				end
				last_PS2_CLK <= PS2_CLK;
			end
			else if (state == ACKNOWLEDGE)
				last_PS2_CLK <= PS2_CLK;
				if(!last_PS2_CLK && PS2_CLK) //On rising edge
				begin
					//Store acknowledge bit in control register
				
				end
			else if (state == RECEIVING_DATA)
			begin
				//counter to store bits in register, or shift bits.
			end	
		end //reset_n	
	end //always	
		
				
		// Old code below:
		
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
