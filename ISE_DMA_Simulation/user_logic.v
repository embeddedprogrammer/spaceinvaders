//----------------------------------------------------------------------------
// user_logic.v - module
//----------------------------------------------------------------------------
//
// ***************************************************************************
// ** Copyright (c) 1995-2011 Xilinx, Inc.  All rights reserved.            **
// **                                                                       **
// ** Xilinx, Inc.                                                          **
// ** XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"         **
// ** AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND       **
// ** SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,        **
// ** OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,        **
// ** APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION           **
// ** THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,     **
// ** AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE      **
// ** FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY              **
// ** WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE               **
// ** IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR        **
// ** REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF       **
// ** INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS       **
// ** FOR A PARTICULAR PURPOSE.                                             **
// **                                                                       **
// ***************************************************************************
//
//----------------------------------------------------------------------------
// Filename:          user_logic.v
// Version:           1.00.a
// Description:       User logic module.
// Date:              Tue Dec 01 13:12:03 2015 (by Create and Import Peripheral Wizard)
// Verilog Standard:  Verilog-2001
//----------------------------------------------------------------------------
// Naming Conventions:
//   active low signals:                    "*_n"
//   clock signals:                         "clk", "clk_div#", "clk_#x"
//   reset signals:                         "rst", "rst_n"
//   generics:                              "C_*"
//   user defined types:                    "*_TYPE"
//   state machine next state:              "*_ns"
//   state machine current state:           "*_cs"
//   combinatorial signals:                 "*_com"
//   pipelined or register delay signals:   "*_d#"
//   counter signals:                       "*cnt*"
//   clock enable signals:                  "*_ce"
//   internal version of output port:       "*_i"
//   device pins:                           "*_pin"
//   ports:                                 "- Names begin with Uppercase"
//   processes:                             "*_PROCESS"
//   component instantiations:              "<ENTITY_>I_<#|FUNC>"
//----------------------------------------------------------------------------

`uselib lib=unisims_ver
`uselib lib=proc_common_v3_00_a

module user_logic
(
  // -- ADD USER PORTS BELOW THIS LINE ---------------
  // --USER ports added here 
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
  IP2Bus_Error,                   // IP to Bus error response
  ip2bus_mstrd_req,               // IP to Bus master read request
  ip2bus_mstwr_req,               // IP to Bus master write request
  ip2bus_mst_addr,                // IP to Bus master read/write address
  ip2bus_mst_be,                  // IP to Bus byte enable
  ip2bus_mst_lock,                // Ip to Bus bus lock
  ip2bus_mst_reset,               // Ip to Bus master reset
  bus2ip_mst_cmdack,              // Bus to Ip master command ack
  bus2ip_mst_cmplt,               // Bus to Ip master trans complete
  bus2ip_mst_error,               // Bus to Ip master error
  bus2ip_mst_rearbitrate,         // Bus to Ip master re-arbitrate for bus ownership
  bus2ip_mst_cmd_timeout,         // Bus to Ip master command time out
  bus2ip_mstrd_d,                 // Bus to Ip master read data
  bus2ip_mstrd_src_rdy_n,         // Bus to Ip master read source ready
  ip2bus_mstwr_d,                 // Ip to Bus master write data
  bus2ip_mstwr_dst_rdy_n          // Bus to Ip master write dest. ready
  // -- DO NOT EDIT ABOVE THIS LINE ------------------
); // user_logic

// -- ADD USER PARAMETERS BELOW THIS LINE ------------
// --USER parameters added here 
// -- ADD USER PARAMETERS ABOVE THIS LINE ------------

// -- DO NOT EDIT BELOW THIS LINE --------------------
// -- Bus protocol parameters, do not add to or delete
parameter C_MST_AWIDTH                   = 32;
parameter C_MST_DWIDTH                   = 32;
parameter C_NUM_REG                      = 12;
parameter C_SLV_DWIDTH                   = 32;
// -- DO NOT EDIT ABOVE THIS LINE --------------------

// -- ADD USER PORTS BELOW THIS LINE -----------------
// --USER ports added here 
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
output                                    ip2bus_mstrd_req;
output                                    ip2bus_mstwr_req;
output     [C_MST_AWIDTH-1 : 0]           ip2bus_mst_addr;
output     [(C_MST_DWIDTH/8)-1 : 0]       ip2bus_mst_be;
output                                    ip2bus_mst_lock;
output                                    ip2bus_mst_reset;
input                                     bus2ip_mst_cmdack;
input                                     bus2ip_mst_cmplt;
input                                     bus2ip_mst_error;
input                                     bus2ip_mst_rearbitrate;
input                                     bus2ip_mst_cmd_timeout;
input      [C_MST_DWIDTH-1 : 0]           bus2ip_mstrd_d;
input                                     bus2ip_mstrd_src_rdy_n;
output     [C_MST_DWIDTH-1 : 0]           ip2bus_mstwr_d;
input                                     bus2ip_mstwr_dst_rdy_n;
// -- DO NOT EDIT ABOVE THIS LINE --------------------

//----------------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------------

  // --USER nets declarations added here, as needed for user logic

  // Nets for user logic slave model s/w accessible register example
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg0;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg1;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg2;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg3;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg4;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg5;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg6;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg7;
  wire       [7 : 0]                        slv_reg_write_sel;
  wire       [7 : 0]                        slv_reg_read_sel;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_ip2bus_data;
  wire                                      slv_read_ack;
  wire                                      slv_write_ack;
  integer                                   byte_index, bit_index;

  // Nets for user logic master model example
  // signals for master model control/status registers write/read
  reg        [C_SLV_DWIDTH-1 : 0]           mst_ip2bus_data;
  wire                                      mst_reg_write_req;
  wire                                      mst_reg_read_req;
  wire       [3 : 0]                        mst_reg_write_sel;
  wire       [3 : 0]                        mst_reg_read_sel;
  wire                                      mst_write_ack;
  wire                                      mst_read_ack;
  // signals for master model control/status registers
  reg        [7 : 0]                        mst_reg [0 : 15];
  reg        [15 : 0]                       mst_byte_we;
  wire                                      mst_cntl_rd_req;
  wire                                      mst_cntl_wr_req;
  wire                                      mst_cntl_bus_lock;
  wire                                      mst_cntl_burst;
  wire       [C_MST_AWIDTH-1 : 0]           mst_ip2bus_addr;
  wire       [11 : 0]                       mst_xfer_length;
  wire       [15 : 0]                       mst_ip2bus_be;
  reg                                       mst_go;
  // signals for master model command interface state machine
    parameter  [1 : 0]                         CMD_IDLE = 2'b00,
                                             CMD_RUN  = 2'b01,
                                             CMD_WAIT_FOR_DATA  = 2'b10,
                                             CMD_DONE  = 2'b11;
  reg        [1 : 0]                        mst_cmd_sm_state;
  reg                                       mst_cmd_sm_set_done;
  reg                                       mst_cmd_sm_set_error;
  reg                                       mst_cmd_sm_set_timeout;
  reg                                       mst_cmd_sm_busy;
  reg                                       mst_cmd_sm_clr_go;
  reg                                       mst_cmd_sm_rd_req;
  reg                                       mst_cmd_sm_wr_req;
  reg                                       mst_cmd_sm_reset;
  reg                                       mst_cmd_sm_bus_lock;
  reg        [C_MST_AWIDTH-1 : 0]           mst_cmd_sm_ip2bus_addr;
  reg        [C_MST_DWIDTH/8-1 : 0]         mst_cmd_sm_ip2bus_be;
  wire                                      mst_fifo_valid_write_xfer;
  wire                                      mst_fifo_valid_read_xfer;
  wire                                      bus2ip_Reset;
  parameter                                  BE_WIDTH = C_SLV_DWIDTH/8;
  parameter  [7:0]                           GO_DATA_KEY = 8'ha;
  parameter                                  GO_BYTE_LANE = 15;

  // USER logic implementation added here

  // ------------------------------------------------------
  // Example code to read/write user logic slave model s/w accessible registers
  // 
  // Note:
  // The example code presented here is to show you one way of reading/writing
  // software accessible registers implemented in the user logic slave model.
  // Each bit of the Bus2IP_WrCE/Bus2IP_RdCE signals is configured to correspond
  // to one software accessible register by the top level template. For example,
  // if you have four 32 bit software accessible registers in the user logic,
  // you are basically operating on the following memory mapped registers:
  // 
  //    Bus2IP_WrCE/Bus2IP_RdCE   Memory Mapped Register
  //                     "1000"   C_BASEADDR + 0x0
  //                     "0100"   C_BASEADDR + 0x4
  //                     "0010"   C_BASEADDR + 0x8
  //                     "0001"   C_BASEADDR + 0xC
  // 
  // ------------------------------------------------------

  assign
    slv_reg_write_sel = Bus2IP_WrCE[7:0],
    slv_reg_read_sel  = Bus2IP_RdCE[7:0],
    slv_write_ack     = Bus2IP_WrCE[0] || Bus2IP_WrCE[1] || Bus2IP_WrCE[2] || Bus2IP_WrCE[3] || Bus2IP_WrCE[4] || Bus2IP_WrCE[5] || Bus2IP_WrCE[6] || Bus2IP_WrCE[7],
    slv_read_ack      = Bus2IP_RdCE[0] || Bus2IP_RdCE[1] || Bus2IP_RdCE[2] || Bus2IP_RdCE[3] || Bus2IP_RdCE[4] || Bus2IP_RdCE[5] || Bus2IP_RdCE[6] || Bus2IP_RdCE[7];

  // implement slave model register(s)
  always @( posedge Bus2IP_Clk )
    begin

      if ( Bus2IP_Resetn == 1'b0 )
        begin
          slv_reg0 <= 0;
          slv_reg1 <= 0;
          slv_reg2 <= 0;
          slv_reg3 <= 0;
          slv_reg4 <= 0;
          slv_reg5 <= 0;
          slv_reg6 <= 0;
          slv_reg7 <= 0;
        end
      else
        case ( slv_reg_write_sel )
          8'b10000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg0[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          8'b01000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg1[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          8'b00100000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg2[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          8'b00010000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg3[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          8'b00001000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg4[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          8'b00000100 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg5[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          8'b00000010 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg6[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          8'b00000001 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg7[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          default : begin
            slv_reg0 <= slv_reg0;
            slv_reg1 <= slv_reg1;
            slv_reg2 <= slv_reg2;
            slv_reg3 <= slv_reg3;
            slv_reg4 <= slv_reg4;
            slv_reg5 <= slv_reg5;
            slv_reg6 <= slv_reg6;
            slv_reg7 <= slv_reg7;
                    end
        endcase

    end // SLAVE_REG_WRITE_PROC

  // implement slave model register read mux
  always @( slv_reg_read_sel or slv_reg0 or slv_reg1 or slv_reg2 or slv_reg3 or slv_reg4 or slv_reg5 or slv_reg6 or slv_reg7 )
    begin 

      case ( slv_reg_read_sel )
        8'b10000000 : slv_ip2bus_data <= slv_reg0;
        8'b01000000 : slv_ip2bus_data <= slv_reg1;
        8'b00100000 : slv_ip2bus_data <= slv_reg2;
        8'b00010000 : slv_ip2bus_data <= slv_reg3;
        8'b00001000 : slv_ip2bus_data <= slv_reg4;
        8'b00000100 : slv_ip2bus_data <= slv_reg5;
        8'b00000010 : slv_ip2bus_data <= slv_reg6;
        8'b00000001 : slv_ip2bus_data <= slv_reg7;
        default : slv_ip2bus_data <= 0;
      endcase

    end // SLAVE_REG_READ_PROC

  //----------------------------------------------------------------------------
  // Example code to demonstrate user logic master model functionality
  // 
  // Note:
  // The example code presented here is to show you one way of stimulating
  // the AXI4LITE master interface under user control. It is provided for
  // demonstration purposes only and allows the user to exercise the AXI4LITE
  // master interface during test and evaluation of the template.
  // This user logic master model contains a 16-byte flattened register and
  // the user is required to initialize the value to desire and then write to
  // the model's 'Go' port to initiate the user logic master operation.
  // 
  //    Control Register     (C_BASEADDR + OFFSET + 0x0):
  //       bit 0    - Rd     (Read Request Control)
  //       bit 1    - Wr     (Write Request Control)
  //       bit 2    - BL     (Bus Lock Control)
  //       bit 3    - Brst   (Burst Assertion Control)
  //       bit 4-7  - Spare  (Spare Control Bits)
  //    Status Register      (C_BASEADDR + OFFSET + 0x1):
  //       bit 0    - Done   (Transfer Done Status)
  //       bit 1    - Busy   (User Logic Master is Busy)
  //       bit 2    - Error  (User Logic Master request got error response)
  //       bit 3    - Tmout  (User Logic Master request is timeout)
  //       bit 2-7  - Spare  (Spare Status Bits)
  //    Addrress Register    (C_BASEADDR + OFFSET + 0x4):
  //       bit 0-31 - Target Address (This 32-bit value is used to populate the
  //                  ip2bus_Mst_Addr(0:31) address bus during a Read or Write
  //                  user logic master operation)
  //    Byte Enable Register (C_BASEADDR + OFFSET + 0x8):
  //       bit 0-15 - Master BE (This 16-bit value is used to populate the
  //                  ip2bus_Mst_BE byte enable bus during a Read or Write user
  //                  logic master operation for single data beat transfer)
  //    Length Register      (C_BASEADDR + OFFSET + 0xC):
  //       bit 0-3  - Reserved
  //       bit 4-15 - Transfer Length (This 12-bit value is used to populate the
  //                  ip2bus_Mst_Length(0:11) transfer length bus which specifies
  //                  the number of bytes (1 to 4096) to transfer during user logic
  //                  master Read or Write fixed length burst operations)
  //    Go Register          (C_BASEADDR + OFFSET + 0xF):
  //       bit 0-7  - Go Port (Write to this byte address initiates the user
  //                  logic master transfer, data key value of 0x0A must be used)
  // 
  //    Note: OFFSET may be different depending on your address space configuration,
  //          by default it's either 0x0 or 0x100. Refer to IPIF address range array
  //          for actual value.
  // 
  // Here's an example procedure in your software application to initiate
  // read operation of a single word:
  //    1. write 0x01 to the control register to perform write operation.
  //    2. write the target address to the address register .
  //    3. write valid byte lane value to the be register
  //      - note: this value must be aligned with ip2bus address  
  //    4. write 0x040 to the length register
  //    5. write 0x0a to the go register, this will start the master write operation
  //
  // Here's an example procedure in your software application to initiate
  // write operation of a single data word of this master model:
  //   1. write 0x02 to the control register to perform write operation.
  //   2. write the target address to the address register
  //   3. write valid byte lane value to the be register
  //      - note: this value must be aligned with ip2bus address
  //   4. write 0x0040 to the length register
  //   5. write 0x0a to the go register, this will start the master write operation
  // 
  //----------------------------------------------------------------------------

  // master register control signals generation
  assign mst_reg_write_req = Bus2IP_WrCE[8] || Bus2IP_WrCE[9] || Bus2IP_WrCE[10] || Bus2IP_WrCE[11];
  assign mst_reg_read_req  = Bus2IP_RdCE[8] || Bus2IP_RdCE[9] || Bus2IP_RdCE[10] || Bus2IP_RdCE[11];
  assign mst_reg_write_sel = Bus2IP_WrCE[11 : 8];
  assign mst_reg_read_sel  = Bus2IP_RdCE[11 : 8];
  assign mst_write_ack     = mst_reg_write_req;
  assign mst_read_ack      = mst_reg_read_req;

  // rip control bits from master model registers
  assign mst_cntl_rd_req   = mst_reg[0][0];
  assign mst_cntl_wr_req   = mst_reg[0][1];
  assign mst_cntl_bus_lock = mst_reg[0][2];
  assign mst_cntl_burst    = mst_reg[0][3];
  assign mst_ip2bus_addr   = {mst_reg[7], mst_reg[6], mst_reg[5], mst_reg[4]};
  assign mst_ip2bus_be     = {mst_reg[9], mst_reg[8]};
  assign mst_xfer_length   = {mst_reg[13][3 : 0] , mst_reg[12]};

  // implement byte write enable for each byte slice of the master model registers
  always @ (Bus2IP_BE or mst_reg_write_req or mst_reg_write_sel )
  begin

    for ( byte_index = 0; byte_index <= 15; byte_index = byte_index+1)
      mst_byte_we[byte_index] <= mst_reg_write_req & mst_reg_write_sel[3 - (byte_index/BE_WIDTH) ] &
                                 Bus2IP_BE[byte_index- ((byte_index/BE_WIDTH)*BE_WIDTH)];
    end // MASTER_REG_BYTE_WR_EN 

  // implement master model registers
  always @ ( posedge Bus2IP_Clk )
  begin
    if (Bus2IP_Resetn == 1'b0 )
      begin
        for ( byte_index = 0; byte_index <= 14; byte_index = byte_index+1 ) 
          mst_reg[byte_index] <= 0;
      end
    else 
      begin
        if ( mst_byte_we[0] == 1'b1 )
          begin
            mst_reg[0][7:0] <= Bus2IP_Data[7 : 0];
          end

        mst_reg[1][1] <= mst_cmd_sm_busy;  

        if (mst_byte_we[1] == 1'b1 )
        // allows a clear of the 'Done'/'error'/'timeout'
          begin
            mst_reg[1][0] <= Bus2IP_Data[(1-(1/BE_WIDTH)*BE_WIDTH)*8];
            mst_reg[1][2] <= Bus2IP_Data[(1-(1/BE_WIDTH)*BE_WIDTH)*8+2];
            mst_reg[1][3] <= Bus2IP_Data[(1-(1/BE_WIDTH)*BE_WIDTH)*8+3];
          end
        else
          // 'Done'/'error'/'timeout' from master control state machine
          begin
            mst_reg[1][0]  <= mst_cmd_sm_set_done | mst_reg[1][0];
            mst_reg[1][2]  <= mst_cmd_sm_set_error | mst_reg[1][2];
            mst_reg[1][3]  <= mst_cmd_sm_set_timeout | mst_reg[1][3];
          end 	 
        // byte 2 and 3 are reserved
        // address register (byte 4 to 7)
        // be register (byte 8 to 9)
        // length register (byte 12 to 13)
        // byte 10, 11 and 14 are reserved
        for ( byte_index = 4; byte_index <= 14; byte_index = byte_index+1 )
          if ( mst_byte_we[byte_index] == 1'b1 )
            begin
              mst_reg[byte_index] <= Bus2IP_Data[(byte_index-(byte_index/BE_WIDTH)*BE_WIDTH)*8 +: 8];
          end
    end
  end // MASTER_REG_WRITE_PROC

  // implement master model write only 'go' port
  always @ ( posedge Bus2IP_Clk)
    begin
      if (Bus2IP_Resetn == 1'b0 || mst_cmd_sm_clr_go == 1'b1 )
        begin
          mst_go <= 1'b0;
        end
      else
      if ( mst_cmd_sm_busy == 1'b0 && mst_byte_we[GO_BYTE_LANE] == 1'b1 && Bus2IP_Data[(GO_BYTE_LANE-(GO_BYTE_LANE/BE_WIDTH)*BE_WIDTH)*8 +: 8] == GO_DATA_KEY)
        begin
          mst_go   <= 1'b1;
        end
    end
  // implement master model register read mux
  always @ (mst_reg_read_sel, mst_reg)
    begin 
      case (mst_reg_read_sel)
      4'b1000:
          for ( byte_index = 0; byte_index <= BE_WIDTH-1; byte_index = byte_index+1 )
            mst_ip2bus_data[byte_index*8 +: 8] <= mst_reg[byte_index];
      4'b0100:
          for ( byte_index = 0; byte_index <= BE_WIDTH-1; byte_index = byte_index+1 )
            mst_ip2bus_data[byte_index*8 +: 8] <= mst_reg[BE_WIDTH+byte_index];
      4'b0010:
          for ( byte_index = 0; byte_index <= BE_WIDTH-1; byte_index = byte_index+1 )
            mst_ip2bus_data[byte_index*8 +: 8] <= mst_reg[BE_WIDTH*2+byte_index];
      4'b0001:
          for ( byte_index = 0; byte_index <= BE_WIDTH-1; byte_index = byte_index+1 )
            if ( byte_index == (BE_WIDTH-1) )
              begin
                // go port is not readable
                mst_ip2bus_data[byte_index*8 +: 8] <= 8'b0;
              end
          else
            begin
              mst_ip2bus_data[byte_index*8 +: 8] <= mst_reg[BE_WIDTH*3+byte_index];
            end
        default : 
          begin
            mst_ip2bus_data <= 0;
          end
      endcase
  end //MASTER_REG_READ_PROC	

  // user logic master command interface assignments
  assign ip2bus_mstrd_req  = mst_cmd_sm_rd_req;
  assign ip2bus_mstwr_req  = mst_cmd_sm_wr_req;
  assign ip2bus_mst_addr   = mst_cmd_sm_ip2bus_addr;
  assign ip2bus_mst_be     = mst_cmd_sm_ip2bus_be;
  assign ip2bus_mst_lock   = mst_cmd_sm_bus_lock;
  assign ip2bus_mst_reset  = mst_cmd_sm_reset;

  //implement master command interface state machine
  always @ ( posedge Bus2IP_Clk ) 
  begin

      if ( Bus2IP_Resetn == 1'b0 ) 
        begin
        // reset condition
        mst_cmd_sm_state          <= CMD_IDLE;
        mst_cmd_sm_clr_go         <= 0;
        mst_cmd_sm_rd_req         <= 0;
        mst_cmd_sm_wr_req         <= 0;
        mst_cmd_sm_bus_lock       <= 0;
        mst_cmd_sm_reset          <= 0;
        mst_cmd_sm_ip2bus_addr    <= 0;
        mst_cmd_sm_ip2bus_be      <= 0;
        mst_cmd_sm_set_done       <= 0;
        mst_cmd_sm_set_error      <= 0;
        mst_cmd_sm_set_timeout    <= 0;
        mst_cmd_sm_busy           <= 0;
        end
      else

      begin
        // default condition
        mst_cmd_sm_clr_go         <= 0;
        mst_cmd_sm_rd_req         <= 0;
        mst_cmd_sm_wr_req         <= 0;
        mst_cmd_sm_bus_lock       <= 0;
        mst_cmd_sm_reset          <= 0;
        mst_cmd_sm_ip2bus_addr    <= 0;
        mst_cmd_sm_ip2bus_be      <= 0;
        mst_cmd_sm_set_done       <= 0;
        mst_cmd_sm_set_error      <= 0;
        mst_cmd_sm_set_timeout    <= 0;
        mst_cmd_sm_busy           <= 1;
        // state transition
        case (mst_cmd_sm_state)
          CMD_IDLE:
            if ( mst_go == 1'b1 )
              begin 
                mst_cmd_sm_state  <= CMD_RUN;
                mst_cmd_sm_clr_go <= 1'b1;
                mst_cmd_sm_busy   <= 1'b1;
              end
            else
              begin
                mst_cmd_sm_state  <= CMD_IDLE;
                mst_cmd_sm_busy   <= 1'b0;
              end
          CMD_RUN:
            if ( bus2ip_mst_cmdack == 1'b1 && bus2ip_mst_cmplt == 1'b0 ) 
              begin
                mst_cmd_sm_state <= CMD_WAIT_FOR_DATA;
              end
            else if ( bus2ip_mst_cmplt == 1'b1 )
              begin
                mst_cmd_sm_state <= CMD_DONE;

                if ( bus2ip_mst_cmd_timeout == 1'b1)
                  begin 
                   // AXI4LITE address phase timeout
                   mst_cmd_sm_set_error   <= 1'b1;
                   mst_cmd_sm_set_timeout <= 1'b1;
                  end

                else if ( bus2ip_mst_error == 1'b1)
                  begin
                   mst_cmd_sm_set_error   <= 1'b1;
                  end
              end
            else 
              begin
                mst_cmd_sm_state       <= CMD_RUN;
                mst_cmd_sm_rd_req      <= mst_cntl_rd_req;
                mst_cmd_sm_wr_req      <= mst_cntl_wr_req;
                mst_cmd_sm_ip2bus_addr <= mst_ip2bus_addr;
                mst_cmd_sm_ip2bus_be   <= mst_ip2bus_be[15 : 16-C_MST_DWIDTH/8 ];
                mst_cmd_sm_bus_lock    <= mst_cntl_bus_lock;
              end

          CMD_WAIT_FOR_DATA:
            if ( bus2ip_mst_cmplt == 1'b1 )
              begin
                mst_cmd_sm_state <= CMD_DONE;
                if ( bus2ip_mst_cmd_timeout == 1'b1 )
                  begin
                   // AXI4LITE address phase timeout
                   mst_cmd_sm_set_error   <= 1'b1;
                   mst_cmd_sm_set_timeout <= 1'b1;	
                  end
                else if ( bus2ip_mst_error == 1'b1 )
                  begin
                   // AXI4LITE data transfer error
                   mst_cmd_sm_set_error   <= 1'b1;
                  end
              end
            else
              begin
                mst_cmd_sm_state <= CMD_WAIT_FOR_DATA;
              end
          CMD_DONE:
            begin
              mst_cmd_sm_state    <= CMD_IDLE;
              mst_cmd_sm_set_done <= 1'b1;
              mst_cmd_sm_busy     <= 1'b0;
            end
          default : 
            begin
              mst_cmd_sm_state    <= CMD_IDLE;
              mst_cmd_sm_busy     <= 1'b0;
            end
        endcase
      end
    end

  // local srl fifo for data storage
  assign mst_fifo_valid_write_xfer = !(bus2ip_mstrd_src_rdy_n);
  assign mst_fifo_valid_read_xfer  = !(bus2ip_mstwr_dst_rdy_n);
  assign bus2ip_Reset   = !(Bus2IP_Resetn);

  srl_fifo_f #(
    .C_DWIDTH(C_MST_DWIDTH),
    .C_DEPTH(16))
  DATA_CAPTURE_FIFO_I (
    .Clk(Bus2IP_Clk),
    .Reset(bus2ip_Reset),
    .FIFO_Write(mst_fifo_valid_write_xfer),
    .Data_In(bus2ip_mstrd_d),
    .FIFO_Read(mst_fifo_valid_read_xfer),
    .Data_Out(ip2bus_mstwr_d),
    .FIFO_Full(),
    .FIFO_Empty(),
    .Addr()); //DATA_CAPTURE_FIFO_I
  // ------------------------------------------------------------
  // Example code to drive IP to Bus signals
  // ------------------------------------------------------------

assign IP2Bus_Data = (slv_read_ack == 1'b1) ? slv_ip2bus_data :  (mst_read_ack == 1'b1) ? mst_ip2bus_data :  0 ;
  assign IP2Bus_WrAck = slv_write_ack || mst_write_ack;
  assign IP2Bus_RdAck = slv_read_ack || mst_read_ack;
  assign IP2Bus_Error = 0;

endmodule

