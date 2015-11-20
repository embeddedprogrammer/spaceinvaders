module Mouse(D, CLK);
	inout D;
	inout CLK;
	
	PULLUP pullup0 (.O(CLK));
	PULLUP pullup1 (.O(D));
	
	reg CLK_OUT;
	reg CLK_T;
	wire CLK_IN;
	
	assign CLK = CLK_T ? 1'bz : CLK_OUT;
	assign CLK_IN = CLK;
	
	reg D_OUT;
	reg D_T;
	wire D_IN;
	
	assign D = D_T ? 1'bz : D_OUT;
	assign D_IN = D;
	
	initial begin
		CLK_OUT = 0;
		CLK_T = 1;
		D_T = 1;
		D_OUT = 1;
		#10;
		
		while (CLK_IN) #1; //Wait till host pulls clock low.
		while (!CLK_IN) #1; //Wait till host releases clock.
		#100_000; //Time before mouse starts generating clock is about 100us
		CLK_T = 0;
		CLK_OUT = 0; #38_500; CLK_OUT = 1; #38_500;
		CLK_OUT = 0; #38_500; CLK_OUT = 1; #38_500;
		CLK_OUT = 0; #38_500; CLK_OUT = 1; #38_500;
		CLK_OUT = 0; #38_500; CLK_OUT = 1; #38_500;
		CLK_OUT = 0; #38_500; CLK_OUT = 1; #38_500;
		CLK_OUT = 0; #38_500; CLK_OUT = 1; #38_500;
		CLK_OUT = 0; #38_500; CLK_OUT = 1; #38_500;
		CLK_OUT = 0; #38_500; CLK_OUT = 1; #38_500;
		CLK_OUT = 0; #38_500; CLK_OUT = 1; #38_500;
		CLK_OUT = 0; #38_500; CLK_OUT = 1; #28_500; D_T = 0; D_OUT = 0; #10_000 //ACK
		CLK_OUT = 0; #38_500; CLK_T   = 1; #20_000; D_T = 1; D_OUT = 1;
		
		#500_000; //Send first byte in packet (0b'0001 1000)
		CLK_OUT = 0;
		D_T = 0;              D_OUT = 0; #10_000; CLK_T   = 0; #38_500;  //Start Bit
		CLK_OUT = 1; #11_500; D_OUT = 0; #27_000; CLK_OUT = 0; #38_500;  //b0
		CLK_OUT = 1; #11_500; D_OUT = 0; #27_000; CLK_OUT = 0; #38_500;  //b1
		CLK_OUT = 1; #11_500; D_OUT = 0; #27_000; CLK_OUT = 0; #38_500;  //b2
		CLK_OUT = 1; #11_500; D_OUT = 1; #27_000; CLK_OUT = 0; #38_500;  //b3
		CLK_OUT = 1; #11_500; D_OUT = 1; #27_000; CLK_OUT = 0; #38_500;  //b4
		CLK_OUT = 1; #11_500; D_OUT = 0; #27_000; CLK_OUT = 0; #38_500;  //b5
		CLK_OUT = 1; #11_500; D_OUT = 0; #27_000; CLK_OUT = 0; #38_500;  //b6
		CLK_OUT = 1; #11_500; D_OUT = 0; #27_000; CLK_OUT = 0; #38_500;  //b7
		CLK_OUT = 1; #11_500; D_OUT = 1; #27_000; CLK_OUT = 0; #38_500;  //Parity
		CLK_OUT = 1; #11_500; D_T   = 1; #27_000; CLK_OUT = 0; #38_500;  //Stop bit
		CLK_T = 1;
		
		#250_000; //Send second byte in packet (0b'1111 1101)
		CLK_OUT = 0;
		D_T = 0;              D_OUT = 0; #10_000; CLK_T   = 0; #38_500;  //Start Bit
		CLK_OUT = 1; #11_500; D_OUT = 1; #27_000; CLK_OUT = 0; #38_500;  //b0
		CLK_OUT = 1; #11_500; D_OUT = 0; #27_000; CLK_OUT = 0; #38_500;  //b1
		CLK_OUT = 1; #11_500; D_OUT = 1; #27_000; CLK_OUT = 0; #38_500;  //b2
		CLK_OUT = 1; #11_500; D_OUT = 1; #27_000; CLK_OUT = 0; #38_500;  //b3
		CLK_OUT = 1; #11_500; D_OUT = 1; #27_000; CLK_OUT = 0; #38_500;  //b4
		CLK_OUT = 1; #11_500; D_OUT = 1; #27_000; CLK_OUT = 0; #38_500;  //b5
		CLK_OUT = 1; #11_500; D_OUT = 1; #27_000; CLK_OUT = 0; #38_500;  //b6
		CLK_OUT = 1; #11_500; D_OUT = 1; #27_000; CLK_OUT = 0; #38_500;  //b7
		CLK_OUT = 1; #11_500; D_OUT = 0; #27_000; CLK_OUT = 0; #38_500;  //Parity
		CLK_OUT = 1; #11_500; D_T   = 1; #27_000; CLK_OUT = 0; #38_500;  //Stop bit
		CLK_T = 1;

		#250_000; //Send third byte in packet (0b'0000 0000)
		CLK_OUT = 0;
		D_T = 0;              D_OUT = 0; #10_000; CLK_T   = 0; #38_500;  //Start Bit
		CLK_OUT = 1; #11_500; D_OUT = 0; #27_000; CLK_OUT = 0; #38_500;  //b0
		CLK_OUT = 1; #11_500; D_OUT = 0; #27_000; CLK_OUT = 0; #38_500;  //b1
		CLK_OUT = 1; #11_500; D_OUT = 0; #27_000; CLK_OUT = 0; #38_500;  //b2
		CLK_OUT = 1; #11_500; D_OUT = 0; #27_000; CLK_OUT = 0; #38_500;  //b3
		CLK_OUT = 1; #11_500; D_OUT = 0; #27_000; CLK_OUT = 0; #38_500;  //b4
		CLK_OUT = 1; #11_500; D_OUT = 0; #27_000; CLK_OUT = 0; #38_500;  //b5
		CLK_OUT = 1; #11_500; D_OUT = 0; #27_000; CLK_OUT = 0; #38_500;  //b6
		CLK_OUT = 1; #11_500; D_OUT = 0; #27_000; CLK_OUT = 0; #38_500;  //b7
		CLK_OUT = 1; #11_500; D_OUT = 1; #27_000; CLK_OUT = 0; #38_500;  //Parity
		CLK_OUT = 1; #11_500; D_T   = 1; #27_000; CLK_OUT = 0; #38_500;  //Stop bit
		CLK_T = 1;		
	end
endmodule
