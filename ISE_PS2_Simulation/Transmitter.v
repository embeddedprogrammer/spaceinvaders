module Transmitter(CLK, Resetn, Load, LoadVal, Dout, Done, bitsToSend);
	input       CLK;
	input       Resetn;
	input       Load;
	input [7:0] LoadVal;
	output      Dout;
	output      Done;
	output [11:0] bitsToSend;
	
	reg  [11:0] bitsToSend;

  always @(negedge CLK or negedge Resetn or posedge Load)
	begin
		if(!Resetn)
		begin
			bitsToSend <= 12'b1111_1111_1111;
		end
		else
		begin
			if(Load)
			begin
				bitsToSend[0] <= 0; // Start bit
				bitsToSend[8:1] <= LoadVal[7:0]; // Data
				bitsToSend[9] <= ~^LoadVal[7:0]; // Bitwise reduction XNOR for odd parity bit.
				bitsToSend[10] <= 1; // Stop bit
				bitsToSend[11] <= 0; // End of transmission (this bit won't actually be sent, but is needed for the SM to stop)
			end
			else if(Done)
			begin
				bitsToSend <= 12'b1111_1111_1111;
			end
			else
			begin
				bitsToSend[10:0] <= bitsToSend[11:1]; // Shift bits to continue sending data.
				bitsToSend[11] <= 1;
			end
		end
	end
	assign Dout = bitsToSend[0];
	assign Done = (bitsToSend == 12'b1111_1111_1101);
endmodule
