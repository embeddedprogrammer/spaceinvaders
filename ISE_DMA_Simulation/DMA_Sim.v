`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   09:04:29 12/03/2015
// Design Name:   user_logic
// Module Name:   C:/Users/superman/Documents/HinckWhite/ISE_DRAM_Simulation/DMA_Sim.v
// Project Name:  ISE_DRAM_Simulation
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: user_logic
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module DMA_Sim;

	// Inputs
	reg Bus2IP_Clk;
	reg Bus2IP_Resetn;
	reg [31:0] Bus2IP_Data;
	reg [3:0] Bus2IP_BE;
	reg [11:0] Bus2IP_RdCE;
	reg [11:0] Bus2IP_WrCE;
	reg bus2ip_mst_cmdack;
	reg bus2ip_mst_cmplt;
	reg bus2ip_mst_error;
	reg bus2ip_mst_rearbitrate;
	reg bus2ip_mst_cmd_timeout;
	reg [31:0] bus2ip_mstrd_d;
	reg bus2ip_mstrd_src_rdy_n;
	reg bus2ip_mstwr_dst_rdy_n;

	// Outputs
	wire [31:0] IP2Bus_Data;
	wire IP2Bus_RdAck;
	wire IP2Bus_WrAck;
	wire IP2Bus_Error;
	wire ip2bus_mstrd_req;
	wire ip2bus_mstwr_req;
	wire [31:0] ip2bus_mst_addr;
	wire [3:0] ip2bus_mst_be;
	wire ip2bus_mst_lock;
	wire ip2bus_mst_reset;
	wire [31:0] ip2bus_mstwr_d;

	// Instantiate the Unit Under Test (UUT)
	user_logic uut (
		.Bus2IP_Clk(Bus2IP_Clk), 
		.Bus2IP_Resetn(Bus2IP_Resetn), 
		.Bus2IP_Data(Bus2IP_Data), 
		.Bus2IP_BE(Bus2IP_BE), 
		.Bus2IP_RdCE(Bus2IP_RdCE), 
		.Bus2IP_WrCE(Bus2IP_WrCE), 
		.IP2Bus_Data(IP2Bus_Data), 
		.IP2Bus_RdAck(IP2Bus_RdAck), 
		.IP2Bus_WrAck(IP2Bus_WrAck), 
		.IP2Bus_Error(IP2Bus_Error), 
		.ip2bus_mstrd_req(ip2bus_mstrd_req), 
		.ip2bus_mstwr_req(ip2bus_mstwr_req), 
		.ip2bus_mst_addr(ip2bus_mst_addr), 
		.ip2bus_mst_be(ip2bus_mst_be), 
		.ip2bus_mst_lock(ip2bus_mst_lock), 
		.ip2bus_mst_reset(ip2bus_mst_reset), 
		.bus2ip_mst_cmdack(bus2ip_mst_cmdack), 
		.bus2ip_mst_cmplt(bus2ip_mst_cmplt), 
		.bus2ip_mst_error(bus2ip_mst_error), 
		.bus2ip_mst_rearbitrate(bus2ip_mst_rearbitrate), 
		.bus2ip_mst_cmd_timeout(bus2ip_mst_cmd_timeout), 
		.bus2ip_mstrd_d(bus2ip_mstrd_d), 
		.bus2ip_mstrd_src_rdy_n(bus2ip_mstrd_src_rdy_n), 
		.ip2bus_mstwr_d(ip2bus_mstwr_d), 
		.bus2ip_mstwr_dst_rdy_n(bus2ip_mstwr_dst_rdy_n)
	);

	initial begin
		// Initialize Inputs
		Bus2IP_Clk = 0;
		Bus2IP_Resetn = 0;
		Bus2IP_Data = 0;
		Bus2IP_BE = 0;
		Bus2IP_RdCE = 0;
		Bus2IP_WrCE = 0;
		bus2ip_mst_cmdack = 0;
		bus2ip_mst_cmplt = 0;
		bus2ip_mst_error = 0;
		bus2ip_mst_rearbitrate = 0;
		bus2ip_mst_cmd_timeout = 0;
		bus2ip_mstrd_d = 0;
		bus2ip_mstrd_src_rdy_n = 0;
		bus2ip_mstwr_dst_rdy_n = 0;

		// Wait 100 ns for global reset to finish
		#100;
        
		// Add stimulus here

	end
      
endmodule

