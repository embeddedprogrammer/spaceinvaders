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
		#200_000; //Time before mouse starts generating clock must be no greater than 15ms.
		CLK_T = 0;
		CLK_OUT = 0; #50_000; CLK_OUT = 1; #50_000;
		CLK_OUT = 0; #50_000; CLK_OUT = 1; #50_000;
		CLK_OUT = 0; #50_000; CLK_OUT = 1; #50_000;
		CLK_OUT = 0; #50_000; CLK_OUT = 1; #50_000;
		CLK_OUT = 0; #50_000; CLK_OUT = 1; #50_000;
		CLK_OUT = 0; #50_000; CLK_OUT = 1; #50_000;
		CLK_OUT = 0; #50_000; CLK_OUT = 1; #50_000;
		CLK_OUT = 0; #50_000; CLK_OUT = 1; #50_000;
		CLK_OUT = 0; #50_000; CLK_OUT = 1; #50_000;
		CLK_OUT = 0; #50_000; CLK_OUT = 1; #25_000; D_T = 0; D_OUT = 0; #25_000 //ACK
		CLK_OUT = 0; #50_000; CLK_OUT = 1; #25_000; D_T = 1; D_OUT = 1; #25_000
		CLK_T = 1;
		
		#500_000; //Send a character
		CLK_T = 0;
		D_T = 0;
		CLK_OUT = 1; #25_000; D_OUT = 0; #25_000; CLK_OUT = 0; #50_000;  //Start Bit
		CLK_OUT = 1; #25_000; D_OUT = 1; #25_000; CLK_OUT = 0; #50_000;  //b0
		CLK_OUT = 1; #25_000; D_OUT = 1; #25_000; CLK_OUT = 0; #50_000;  //b1
		CLK_OUT = 1; #25_000; D_OUT = 0; #25_000; CLK_OUT = 0; #50_000;  //b2
		CLK_OUT = 1; #25_000; D_OUT = 1; #25_000; CLK_OUT = 0; #50_000;  //b3
		CLK_OUT = 1; #25_000; D_OUT = 0; #25_000; CLK_OUT = 0; #50_000;  //b4
		CLK_OUT = 1; #25_000; D_OUT = 0; #25_000; CLK_OUT = 0; #50_000;  //b5
		CLK_OUT = 1; #25_000; D_OUT = 1; #25_000; CLK_OUT = 0; #50_000;  //b6
		CLK_OUT = 1; #25_000; D_OUT = 0; #25_000; CLK_OUT = 0; #50_000;  //b7
		CLK_OUT = 1; #25_000; D_OUT = 1; #25_000; CLK_OUT = 0; #50_000;  //Parity
		CLK_OUT = 1; #25_000; D_OUT = 1; #25_000; CLK_OUT = 0; #50_000;  //Stop bit
		CLK_OUT = 1;
		CLK_T = 1;
		D_T = 1;
	end
endmodule
