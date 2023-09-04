// Prepared by Anıl Eren Göçer

`timescale 1ns / 1ps

module testbench_ROM(
    );
	reg [3:0] romAddr; 
	wire[4:0] romOutput;
	integer result = 0;
	//instance
	lab4ROM ROM(romAddr,romOutput);

	initial begin
		$display("--------------------");
		$display("--------------------");
		$display("--------------------");
		$display("--------------------");
		$display("--------------------");
		$display("Starting Testbench");
		

		#1; 
		romAddr=4'b0000; //address 0
		#7; 
		if (romOutput==5'b00000) result = result + 1; 	
		else $display("time:",$time,":For romAddr:%b Error in romOutput:%b",romAddr,romOutput);

		#1; 
		romAddr=4'b0001; //address 1
		#7; 
		if (romOutput==5'b00001) result = result + 1; 	
		else $display("time:",$time,":For romAddr:%b Error in romOutput:%b",romAddr,romOutput);

		#1; 
		romAddr=4'b0010; //address 2
		#7; 
		if (romOutput==5'b00110) result = result + 1; 	
		else $display("time:",$time,":For romAddr:%b Error in romOutput:%b",romAddr,romOutput);
		#1;

		#1; 
		romAddr=4'b0011; //address 3
		#7; 
		if (romOutput==5'b00111) result = result + 1; 	
		else $display("time:",$time,":For romAddr:%b Error in romOutput:%b",romAddr,romOutput);
		#1;

		#1; 
		romAddr=4'b0100; //address 4
		#7; 
		if (romOutput==5'b01011) result = result + 1; 	
		else $display("time:",$time,":For romAddr:%b Error in romOutput:%b",romAddr,romOutput);

		#1; 
		romAddr=4'b0101; //address 5
		#7; 
		if (romOutput==5'b01100) result = result + 1; 	
		else $display("time:",$time,":For romAddr:%b Error in romOutput:%b",romAddr,romOutput);
		#1;

		#1; 
		romAddr=4'b0110; //address 6
		#7; 
		if (romOutput==5'b01101) result = result + 1; 	
		else $display("time:",$time,":For romAddr:%b Error in romOutput:%b",romAddr,romOutput);
		#1;

		#1; 
		romAddr=4'b0111; //address 7
		#7; 
		if (romOutput==5'b01110) result = result + 1; 	
		else $display("time:",$time,":For romAddr:%b Error in romOutput:%b",romAddr,romOutput);

		#1; 
		romAddr=4'b1000; //address 8
		#7; 
		if (romOutput==5'b11101) result = result + 1; 	
		else $display("time:",$time,":For romAddr:%b Error in romOutput:%b",romAddr,romOutput);
		#1;

		#1; 
		romAddr=4'b1001; //address 9
		#7; 
		if (romOutput==5'b11110) result = result + 1; 	
		else $display("time:",$time,":For romAddr:%b Error in romOutput:%b",romAddr,romOutput);
		#1;

		#1; 
		romAddr=4'b1010; //address 10
		#7; 
		if (romOutput==5'b11111) result = result + 1; 	
		else $display("time:",$time,":For romAddr:%b Error in romOutput:%b",romAddr,romOutput);

		#1; 
		romAddr=4'b1011; //address 11
		#7; 
		if (romOutput==5'b10000) result = result + 1; 	
		else $display("time:",$time,":For romAddr:%b Error in romOutput:%b",romAddr,romOutput);
		#1;

		#1; 
		romAddr=4'b1100; //address 12
		#7; 
		if (romOutput==5'b10111) result = result + 1; 	
		else $display("time:",$time,":For romAddr:%b Error in romOutput:%b",romAddr,romOutput);
		#1;

		#1; 
		romAddr=4'b1101; //address 13
		#7; 
		if (romOutput==5'b11000) result = result + 1; 	
		else $display("time:",$time,":For romAddr:%b Error in romOutput:%b",romAddr,romOutput);
		#1;

		#1; 
		romAddr=4'b1110; //address 14
		#7; 
		if (romOutput==5'b11001) result = result + 1; 	
		else $display("time:",$time,":For romAddr:%b Error in romOutput:%b",romAddr,romOutput);
		#1;

		romAddr=4'b1111;  //address 15
		#7; 
		if (romOutput==5'b11010) result = result + 1; 	
		else $display("time:",$time,":For romAddr:%b Error in romOutput:%b",romAddr,romOutput);	

		#1; 

		$display("Result %d",result);	
		$display("Testbench was finished");	
		$display("--------------------");
		$display("--------------------");
		$display("--------------------");
		$display("--------------------");
		$display("--------------------");
		$finish;
	end
	
	
endmodule

