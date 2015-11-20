module Receiver(CLK, Resetn, Interrupt, ReadVal, Din, bitsReceived);
	input         CLK;
	input         Resetn;
	output        Interrupt;
	output [7:0]  ReadVal;
	input         Din;
	output [10:0] bitsReceived;
	
	reg  [10:0] bitsReceived;

  always @(negedge CLK or negedge Resetn)
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
