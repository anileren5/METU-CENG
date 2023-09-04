// Prepared by Anıl Eren Göçer

`timescale 1ns / 1ps
module testbench_RAM(
    );

	reg ramMode; //0:read, 1:write
	reg [3:0] ramAddr; 
	reg [4:0] ramInput;
	reg ramOp; //0:polynomial, 1:derivative
	reg [1:0] ramArg; //00:+1, 01:+2, 10:-1, 11:-2
	reg CLK;
	wire [8:0] ramOutput;
	 
	//instance
	lab4RAM RAM(ramMode, ramAddr, ramInput, ramOp, ramArg, CLK, ramOutput);
	//Set initial values
	initial CLK = 0;	
	always #5 CLK = ~CLK;	 

	initial begin
		$display("--------------------");
		$display("--------------------");
		$display("--------------------");
		$display("--------------------");
		$display("--------------------");
		$display("Starting Testbench");
		ramAddr=4'b0100;
		
		ramMode=1'b1;//write
		ramInput=5'b00000;
		ramOp=1'b0; //derivative
		ramArg=2'b00; //-1
		#10;
		
		ramMode=1'b0;//read
		#1;
      	$display("%b %b %b %b",ramInput,ramOp,ramArg,ramOutput);


ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b00000;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b00000;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b00000;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b00000;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b00000;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b00000;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b00000;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b00000;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b00001;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b00001;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b00001;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b00001;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b00001;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b00001;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b00001;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b00001;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b00110;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b00110;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b00110;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b00110;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b00110;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b00110;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b00110;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b00110;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b00111;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b00111;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b00111;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b00111;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b00111;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b00111;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b00111;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b00111;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b01011;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b01011;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b01011;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b01011;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b01011;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b01011;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b01011;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b01011;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b01100;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b01100;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b01100;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b01100;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b01100;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b01100;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b01100;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b01100;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b01101;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b01101;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b01101;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b01101;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b01101;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b01101;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b01101;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b01101;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b01110;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b01110;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b01110;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b01110;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b01110;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b01110;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b01110;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b01110;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b11101;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b11101;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b11101;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b11101;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b11101;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b11101;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b11101;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b11101;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b11110;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b11110;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b11110;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b11110;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b11110;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b11110;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b11110;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b11110;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b11111;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b11111;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b11111;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b11111;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b11111;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b11111;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b11111;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b11111;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b10000;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b10000;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b10000;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b10000;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b10000;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b10000;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b10000;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b10000;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b10111;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b10111;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b10111;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b10111;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b10111;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b10111;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b10111;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b10111;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b11000;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b11000;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b11000;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b11000;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b11000;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b11000;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b11000;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b11000;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b11001;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b11001;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b11001;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b11001;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b11001;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b11001;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b11001;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b11001;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b11010;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b11010;
ramArg = 2'b00;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b11010;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b11010;
ramArg = 2'b01;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b11010;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b11010;
ramArg = 2'b10;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b0;
ramInput = 5'b11010;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);

ramMode = 1'b1;
ramOp = 1'b1;
ramInput = 5'b11010;
ramArg = 2'b11;
#10;

ramMode = 1'b0;
#1;
$display("\%b \%b \%b \%b",ramInput,ramOp,ramArg,ramOutput);



		
		$display("Testbench was finished");	
		$display("--------------------");
		$display("--------------------");
		$display("--------------------");
		$display("--------------------");
		$display("--------------------");
		$finish;
	end

endmodule
