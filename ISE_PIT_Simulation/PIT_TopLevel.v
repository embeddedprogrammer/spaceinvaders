`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company:  Ecen 427
// Engineer: Jacob White and Troy Hinckley
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
	reg        [C_SLV_DWIDTH-1 : 0]           slv_reg0_control;
	reg        [C_SLV_DWIDTH-1 : 0]           slv_reg1_delay;
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
		decrement         = slv_reg0_control[0],
		enableInterrupts  = slv_reg0_control[1],
		enableReload      = slv_reg0_control[2];

  // Counter and PIT logic
  always @(posedge Bus2IP_Clk)
    begin
      if(Bus2IP_Resetn == 1'b0)
        begin
          slv_reg0_control <= 0;
          slv_reg1_delay <= 0;
					counter <= 32'hFFFF_FFFF;
        end
      else
				begin
					case(slv_reg_write_sel)
						2'b10: 
							slv_reg0_control <= Bus2IP_Data;
						2'b01:
							slv_reg1_delay <= Bus2IP_Data;
						default:
							begin
								slv_reg0_control <= slv_reg0_control;
								slv_reg1_delay <= slv_reg1_delay;
							end
					endcase
					if(decrement)
						if(counter == 32'hFFFF_FFFF)
							counter = slv_reg1_delay;
						else if(counter > 0)
							counter <= counter - 1;
						else if(enableReload)
							counter <= slv_reg1_delay;
					IP_Interupt <= (counter == 1) && enableInterrupts;
				end
    end

  // Slave register read mux
  always @(slv_reg_read_sel or slv_reg0_control or slv_reg1_delay)
    begin 
      case(slv_reg_read_sel)
        2'b10:
					slv_ip2bus_data <= slv_reg0_control;
        2'b01:
					slv_ip2bus_data <= slv_reg1_delay;
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
