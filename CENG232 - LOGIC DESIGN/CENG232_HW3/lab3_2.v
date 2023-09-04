`timescale 1ns / 1ps 
module lab3_2(
			input[4:0] smartCode,
			input CLK, 
			input lab, //0:Digital, 1:Mera
			input [1:0] mode, //00:exit, 01:enter, 1x: idle 
			output reg [5:0] numOfStuInMera,
			output reg [5:0] numOfStuInDigital,
			output reg restrictionWarnMera,//1:show warning, 0:do not show warning
			output reg isFullMera, //1:full, 0:not full
			output reg isEmptyMera, //1: empty, 0:not empty
			output reg unlockMera,	//1:door is open, 0:closed
			output reg restrictionWarnDigital,//1:show warning, 0:do not show warning
			output reg isFullDigital, //1:full, 0:not full
			output reg isEmptyDigital, //1: empty, 0:not empty
			output reg unlockDigital //1:door is open, 0:closed
	);
	 
	// You may declare your variables below	
	
	initial begin
			numOfStuInMera=0;
			numOfStuInDigital=0;
			restrictionWarnMera=0;
			isFullMera=0;
			isEmptyMera=1'b1;
			unlockMera=0;		
			restrictionWarnDigital=0;
			isFullDigital=0;
			isEmptyDigital=1'b1;
			unlockDigital=0;
	end
	
	
	//Modify the lines below to implement your design
	
	always @(numOfStuInDigital) begin 
		if (numOfStuInDigital == 6'b000000) isEmptyDigital = 1'b1;
		else isEmptyDigital = 1'b0; 
		if (numOfStuInDigital == 6'b011110) isFullDigital = 1'b1;
		else isFullDigital = 1'b0;
	end

	always @(numOfStuInMera) begin 
		if (numOfStuInMera == 6'b000000) isEmptyMera = 1'b1;
		else isEmptyMera = 1'b0; 
		if (numOfStuInMera == 6'b011110) isFullMera = 1'b1;
		else isFullMera = 1'b0;
	end

	always @(posedge CLK) begin
		if ((mode == 2'b01) && (lab == 1'b0)) begin // ENTER DIGITAL
			if (numOfStuInDigital < 6'b001111) begin
				restrictionWarnDigital = 1'b0;
				unlockDigital = 1'b1;
				numOfStuInDigital = numOfStuInDigital + 6'b000001;
				unlockMera = 1'b0;
				restrictionWarnMera = 1'b0;
			end
			else if ((numOfStuInDigital >= 6'b001111) && (numOfStuInDigital < 6'b011110)) begin
				if (((smartCode[0] + smartCode[1] + smartCode[2] + smartCode[3] + smartCode[4]) % 2) != 0) begin
					restrictionWarnDigital = 1'b0;
					unlockDigital = 1'b1;
					numOfStuInDigital = numOfStuInDigital + 6'b000001;
					unlockMera = 1'b0;
					restrictionWarnMera = 1'b0;
				end
				else begin
					restrictionWarnDigital = 1'b1;
					unlockDigital = 1'b0;
					unlockMera = 1'b0;
					restrictionWarnMera = 1'b0;
				end
			end
			else begin
				unlockDigital = 1'b0;
				unlockMera = 1'b0;
				restrictionWarnDigital = 1'b0;
				restrictionWarnMera = 1'b0;
			end
		end
		else if ((mode == 2'b01) && (lab == 1'b1)) begin // ENTER MERA
			if (numOfStuInMera < 6'b001111) begin
				restrictionWarnMera = 1'b0;
				unlockMera = 1'b1;
				numOfStuInMera = numOfStuInMera + 6'b000001;
				unlockDigital = 1'b0;
				restrictionWarnDigital = 1'b0;
			end
			else if ((numOfStuInMera >= 6'b001111) && (numOfStuInMera < 6'b011110)) begin
				if (((smartCode[0] + smartCode[1] + smartCode[2] + smartCode[3] + smartCode[4]) % 2) == 0) begin
					restrictionWarnMera = 1'b0;
					unlockMera = 1'b1;
					numOfStuInMera = numOfStuInMera + 6'b000001;
					unlockDigital = 1'b0;
					restrictionWarnDigital = 1'b0;
				end
				else begin
					restrictionWarnMera = 1'b1;
					unlockMera = 1'b0;
					unlockDigital = 1'b0;
					restrictionWarnDigital = 1'b0;
				end
			end
			else begin
				unlockDigital = 1'b0;
				unlockMera = 1'b0;
				restrictionWarnDigital = 1'b0;
				restrictionWarnMera = 1'b0;
			end
		end
		else if ((mode == 2'b00) && (lab == 1'b0)) begin // EXIT DIGITAL
			if (numOfStuInDigital > 6'b000000) begin
				unlockDigital = 1'b1;
				numOfStuInDigital = numOfStuInDigital - 6'b000001;
				unlockMera = 1'b0;
				restrictionWarnDigital = 1'b0;
				restrictionWarnMera = 1'b0;
			end
			else begin 
				unlockDigital = 1'b0;
				unlockMera = 1'b0;
				restrictionWarnDigital = 1'b0;
				restrictionWarnMera = 1'b0;
			end
		end
		else if ((mode == 2'b00) && (lab == 1'b1)) begin // EXIT MERA
			if (numOfStuInMera > 6'b000000) begin
				unlockMera = 1'b1;
				numOfStuInMera = numOfStuInMera - 6'b000001;
				unlockDigital = 1'b0;
				restrictionWarnMera = 1'b0;
				restrictionWarnDigital = 1'b0;
			end
			else begin 
				unlockMera = 1'b0;
				unlockDigital = 1'b0;
				restrictionWarnMera = 1'b0;
				restrictionWarnDigital = 1'b0;
			end
		end
		else begin // IDLE
			unlockDigital = 1'b0;
			unlockMera = 1'b0;
			restrictionWarnDigital = 1'b0;
			restrictionWarnMera = 1'b0;
		end
	end
endmodule