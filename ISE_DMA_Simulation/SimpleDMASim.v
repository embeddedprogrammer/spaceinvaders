`timescale 1ns / 1ps

module simpleDMASim;

	parameter C_MST_AWIDTH                   = 32;
	parameter C_MST_DWIDTH                   = 32;
	parameter C_NUM_REG                      = 12;
	parameter C_SLV_DWIDTH                   = 32;

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
	
//----------------------------------------------------------------------------
// Simulation
//----------------------------------------------------------------------------
	always
		#5 Bus2IP_Clk = ~Bus2IP_Clk; //Clock at 100 MHz
		
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
		bus2ip_mstrd_src_rdy_n = 1;
		bus2ip_mstwr_dst_rdy_n = 1;

		// Wait 10 ns for global reset to finish
		#30;
		Bus2IP_Resetn = 1;
		#10
		writeReg(0, 32'b1111_0000_0000_0000); //Src
		writeReg(1, 32'b1001_0000_0000_0000); //Dst
		writeReg(2, 32'd128); //Length
		#10;
		writeReg(3, 32'hCC); //GO
		#10;
		
		while(1)
		begin
			if(ip2bus_mstrd_req)
			begin
				#20
				bus2ip_mst_cmdack = 1;
				#10
				bus2ip_mst_cmdack = 0;
				#20
				bus2ip_mst_cmplt = 1;
				bus2ip_mstrd_src_rdy_n = 0;
				#10
				bus2ip_mst_cmplt = 0;
				bus2ip_mstrd_src_rdy_n = 1;
			end
			else if(ip2bus_mstwr_req)
			begin
				#20
				bus2ip_mst_cmdack = 1;
				#10
				bus2ip_mst_cmdack = 0;
				#20
				bus2ip_mst_cmplt = 1;
				bus2ip_mstwr_dst_rdy_n = 0;
				#10
				bus2ip_mst_cmplt = 0;
				bus2ip_mstwr_dst_rdy_n = 1;
			end
			else
				#10;
		end
	end
	task writeReg;
		input [31:0] regNum;
		input [31:0] writeVal;
		begin
			Bus2IP_Data = writeVal;
			Bus2IP_WrCE = 1 << (8 - 1 - regNum);
			#10;
			Bus2IP_WrCE = 0;
		end
	endtask
	task readReg;
		input [31:0] regNum;
		begin
			Bus2IP_RdCE = 1 << (8 - 1 - regNum);
			#10
			Bus2IP_RdCE = 0;
		end
	endtask

//----------------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------------

  // --USER nets declarations added here, as needed for user logic

  // Nets for user logic slave model s/w accessible register example
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg0;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg1;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg2;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg3;
  wire       [C_SLV_DWIDTH-1 : 0]           slv_reg4;
  wire       [C_SLV_DWIDTH-1 : 0]           slv_reg5;
  wire       [C_SLV_DWIDTH-1 : 0]           slv_reg6;
  wire       [C_SLV_DWIDTH-1 : 0]           slv_reg7;
  wire       [7 : 0]                        slv_reg_write_sel;
  wire       [7 : 0]                        slv_reg_read_sel;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_ip2bus_data;
  wire                                      slv_read_ack;
  wire                                      slv_write_ack;

  // Nets for user logic master model example
  // signals for master model control/status registers write/read
  wire                                      mst_reg_write_req;
  wire                                      mst_reg_read_req;
  wire       [3 : 0]                        mst_reg_write_sel;
  wire       [3 : 0]                        mst_reg_read_sel;
  wire                                      mst_write_ack;
  wire                                      mst_read_ack;
  // signals for master model control/status registers
  wire                                      mst_cntl_rd_req;
  wire                                      mst_cntl_wr_req;
  wire                                      mst_cntl_bus_lock;
  wire                                      mst_cntl_burst;
  wire       [C_MST_AWIDTH-1 : 0]           mst_ip2bus_addr;
  wire       [15 : 0]                       mst_ip2bus_be;
  wire                                      mst_go;
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

	parameter [1:0]
		IDLE  = 2'b00,
		READ  = 2'b01,
		WRITE = 2'b10;
  reg [1:0] slv_state;
	
	wire [31:0] bytesLeft;
	reg [7:0] slv_cntl_reg;
  reg [31:0] slv_addr_reg;
  wire [16:0] slv_be_reg;
	reg slv_cmd_mst_go;
	reg slv_interrupt;
	
	wire FIFO_Empty;
	wire FIFO_Full;
	wire [31:0] FIFO_Addr;

  assign
    slv_reg_write_sel = Bus2IP_WrCE[7:0],
    slv_reg_read_sel  = Bus2IP_RdCE[7:0],
    slv_write_ack     = Bus2IP_WrCE[0] || Bus2IP_WrCE[1] || Bus2IP_WrCE[2] || Bus2IP_WrCE[3] || Bus2IP_WrCE[4] || Bus2IP_WrCE[5] || Bus2IP_WrCE[6] || Bus2IP_WrCE[7],
    slv_read_ack      = Bus2IP_RdCE[0] || Bus2IP_RdCE[1] || Bus2IP_RdCE[2] || Bus2IP_RdCE[3] || Bus2IP_RdCE[4] || Bus2IP_RdCE[5] || Bus2IP_RdCE[6] || Bus2IP_RdCE[7],
		slv_go            = (slv_reg3 == 32'hCC),
		bytesLeft         = slv_reg2,
		slv_be_reg        = 16'b1111_1111_1111_1111,
		slv_reg4          = slv_state,
		slv_reg5          = mst_cmd_sm_state,
		slv_reg6          = {mst_cmd_sm_set_timeout, mst_cmd_sm_set_error, mst_cmd_sm_busy},
		slv_reg7          = {FIFO_Empty, FIFO_Full};
		
  // implement top level state machine
	always @(posedge Bus2IP_Clk)
	begin
		if (!Bus2IP_Resetn)
		begin
			slv_state <= IDLE;
			slv_cntl_reg <= 4'b0;
			slv_addr_reg <= 32'b0;
			slv_cmd_mst_go <= 0;
			slv_interrupt <= 0;
		end
		else
		begin
			if(mst_cmd_sm_clr_go)
				slv_cmd_mst_go <= 0;
			case(slv_state)
				IDLE:
				begin
					slv_interrupt <= 0;
					if(slv_go)
					begin
						slv_state <= READ;
						slv_reg3 <= 0; //Clear GO register.
					end
				end
				READ:
					if(FIFO_Full || bytesLeft == 0)
					begin
						slv_state <= WRITE;
					end
					else if(!mst_cmd_sm_busy) // If not busy, start a read transaction
					begin
						slv_cntl_reg <= 4'b0001;  // Set control register to read
						slv_addr_reg <= slv_reg0; // Set address to source address
						slv_cmd_mst_go <= 1;      // Go!
					end
					else if(bus2ip_mst_cmplt)
					begin
						slv_reg2 <= slv_reg2 - 4; // Decrement bytes left
						slv_reg0 <= slv_reg0 + 4; // Increment source address
					end
				WRITE:
					if(FIFO_Empty)
						if(bytesLeft == 0)
						begin
							slv_state <= IDLE;
							slv_interrupt <= 1;
						end
						else
							slv_state <= READ;
					else if(!mst_cmd_sm_busy) // If not busy, start a write transaction
					begin
						slv_cntl_reg <= 4'b0010;  // Set control register to write
						slv_addr_reg <= slv_reg1; // Set address to destination address
						slv_cmd_mst_go <= 1;      // Go!
					end
					else if(bus2ip_mst_cmplt)
					begin
						slv_cmd_mst_go <= 0;
						slv_reg1 <= slv_reg1 + 4; // Increment destination address
					end
					else
						slv_cmd_mst_go <= 0;
			endcase
		end
	end

  // implement slave registers
  always @(posedge Bus2IP_Clk)
	begin
		if (!Bus2IP_Resetn)
		begin
			slv_reg0 <= 0;
			slv_reg1 <= 0;
			slv_reg2 <= 0;
			slv_reg3 <= 0;
//			slv_reg4 <= 0; //Read only
//			slv_reg5 <= 0;
//			slv_reg6 <= 0;
//			slv_reg7 <= 0;
		end
		else
			case(slv_reg_write_sel)
				8'b10000000: slv_reg0 <= Bus2IP_Data;
				8'b01000000: slv_reg1 <= Bus2IP_Data;
				8'b00100000: slv_reg2 <= Bus2IP_Data;
				8'b00010000: slv_reg3 <= Bus2IP_Data;
//				8'b00001000: slv_reg4 <= Bus2IP_Data;  Read only
//				8'b00000100: slv_reg5 <= Bus2IP_Data;
//				8'b00000010: slv_reg6 <= Bus2IP_Data;
//				8'b00000001: slv_reg7 <= Bus2IP_Data;
			endcase
	end // SLAVE_REG_WRITE_PROC	

  // implement slave model register read mux
  always @(slv_reg_read_sel or slv_reg0 or slv_reg1 or slv_reg2 or slv_reg3 or slv_reg4 or slv_reg5 or slv_reg6 or slv_reg7)
    begin
      case (slv_reg_read_sel)
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

  // rip control bits from slv control registers
  assign mst_cntl_rd_req   = slv_cntl_reg[0];
  assign mst_cntl_wr_req   = slv_cntl_reg[1];
  assign mst_cntl_bus_lock = slv_cntl_reg[2];
  assign mst_cntl_burst    = slv_cntl_reg[3];
  assign mst_ip2bus_addr   = slv_addr_reg;
  assign mst_ip2bus_be     = slv_be_reg;
	assign mst_go            = slv_cmd_mst_go;

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
    .FIFO_Full(FIFO_Full),
    .FIFO_Empty(FIFO_Empty),
    .Addr(FIFO_Addr)); //DATA_CAPTURE_FIFO_I
  // ------------------------------------------------------------
  // Example code to drive IP to Bus signals
  // ------------------------------------------------------------

assign IP2Bus_Data = (slv_read_ack == 1'b1) ? slv_ip2bus_data : 0;
  assign IP2Bus_WrAck = slv_write_ack || mst_write_ack;
  assign IP2Bus_RdAck = slv_read_ack || mst_read_ack;
  assign IP2Bus_Error = 0;

endmodule

