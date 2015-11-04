`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    19:44:53 11/03/2015 
// Design Name: 
// Module Name:    PIT_TopLevel 
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
module PIT_TopLevel
(
	IP_Interupt,
	counter, // TODO: REMOVE
	slv_reg0, // TODO: REMOVE
	slv_reg1, // TODO: REMOVE
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
output                                    IP_Interupt;
output                                    counter; // TODO: REMOVE
output                                    slv_reg0; // TODO: REMOVE
output                                    slv_reg1; // TODO: REMOVE
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
	reg        [C_SLV_DWIDTH-1 : 0]           slv_reg0;
	reg        [C_SLV_DWIDTH-1 : 0]           slv_reg1;
	reg        [C_SLV_DWIDTH-1 : 0]           counter;
	reg                                       IP_Interupt;
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
    slv_read_ack      = Bus2IP_RdCE[0] || Bus2IP_RdCE[1],
		decrement         = slv_reg0[0],
		enableInterrupts  = slv_reg0[1],
		enableReload      = slv_reg0[2];

  // implement slave model register(s)
  always @( posedge Bus2IP_Clk )
    begin
      if ( Bus2IP_Resetn == 1'b0 )
        begin
          slv_reg0 <= 0;
          slv_reg1 <= 0;
					counter <= 32'hFFFF_FFFF;
        end
      else
				begin
					case (slv_reg_write_sel)
						2'b10: 
							slv_reg0 <= Bus2IP_Data;
						2'b01:
							slv_reg1 <= Bus2IP_Data;
						default:
							begin
								slv_reg0 <= slv_reg0;
								slv_reg1 <= slv_reg1;
							end
					endcase
					if(decrement)
						if(counter == 32'hFFFF_FFFF) //Will this suffice? Or do I need to reload every time decrement is enabled?
							counter = slv_reg1;
						else if(counter > 0)
							counter <= counter - 1;
						else if(enableReload)
							counter <= slv_reg1;
					IP_Interupt <= (counter == 1) && enableInterrupts;
				end
    end

  // implement slave model register read mux
  always @( slv_reg_read_sel or slv_reg0 or slv_reg1 )
    begin 
      case ( slv_reg_read_sel )
        2'b10:
					slv_ip2bus_data <= slv_reg0;
        2'b01:
					slv_ip2bus_data <= slv_reg1;
        2'b11:
					slv_ip2bus_data <= counter; //TODO: This line is only for simulation; take out before importing to XPS.
        default:
					slv_ip2bus_data <= 0;
      endcase

    end // SLAVE_REG_READ_PROC

  // ------------------------------------------------------------
  // Example code to drive IP to Bus signals
  // ------------------------------------------------------------

assign IP2Bus_Data = (slv_read_ack == 1'b1) ? slv_ip2bus_data :  0 ;
  assign IP2Bus_WrAck = slv_write_ack;
  assign IP2Bus_RdAck = slv_read_ack;
  assign IP2Bus_Error = 0;

endmodule