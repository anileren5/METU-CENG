`timescale 1ns / 1ps

module lab4ROM (input [3:0] romAddr, output reg[4:0] romOutput);
	reg [4:0] mem [0:15]; 

	initial begin
		mem[0] = 5'b00000;
		mem[1] = 5'b00001;
		mem[2] = 5'b00110;
		mem[3] = 5'b00111;
		mem[4] = 5'b01011;
		mem[5] = 5'b01100;
		mem[6] = 5'b01101;
		mem[7] = 5'b01110;
		mem[8] = 5'b11101;
		mem[9] = 5'b11110;
		mem[10] = 5'b11111;
		mem[11] = 5'b10000;
		mem[12] = 5'b10111;
		mem[13] = 5'b11000;
		mem[14] = 5'b11001;
		mem[15] = 5'b11010;

	end

	always @(romAddr) begin
		romOutput = mem[romAddr];
	end


endmodule

module lab4RAM (
	input ramMode, //0:read, 1:write
	input [3:0] ramAddr, 
	input [4:0] ramInput,
	input  ramOp, //0:polynomial, 1:derivative
	input [1:0] ramArg,  //00:+1, 01:+2, 10:-1, 11:-2
	input CLK, 
	output reg [8:0] ramOutput);

	reg [8:0] mem [0:15];
	integer i;
	integer n;
	reg [7:0] forconversion;
	initial begin
		mem[0] = 9'b000000000;  
		mem[1] = 9'b000000000;
		mem[2] = 9'b000000000;
		mem[3] = 9'b000000000;
		mem[4] = 9'b000000000;
		mem[5] = 9'b000000000;
		mem[6] = 9'b000000000;
		mem[7] = 9'b000000000;
		mem[8] = 9'b000000000;
		mem[9] = 9'b000000000;
		mem[10] = 9'b000000000;
		mem[11] = 9'b000000000;
		mem[12] = 9'b000000000;
		mem[13] = 9'b000000000;
		mem[14] = 9'b000000000;
		mem[15] = 9'b000000000;		
		n = 0;
		ramOutput = 9'b000000000;			
	end

	always @(posedge CLK) begin 
		if (ramMode == 1'b1) begin // Write
			if (ramOp == 1'b0) begin // Polynomial
				for (i=0;i<5;i=i+1) begin
					if (ramArg == 2'b00) begin 
						if (ramInput[i] == 0) n = n + (1**i);
						else n = n - (1**i);
					end
					else if (ramArg == 2'b01) begin
						if (ramInput[i] == 0) n = n + (2**i);
						else n = n - (2**i);
					end
					else if (ramArg == 2'b10) begin
						if (ramInput[i] == 0) n = n + ((-1)**i);
						else n = n - ((-1)**i);
					end
					else begin
						if (ramInput[i] == 0) n = n + ((-2)**i);
						else n = n - ((-2)**i);
					end
				end
				if (n>=0) forconversion = n;
				else forconversion = -n;
				if (n >= 0) mem[ramAddr] = {1'b0,forconversion};
				else mem[ramAddr] = {1'b1,forconversion};
				n = 0;
			end
			else begin // Derivative
				for (i=0;i<5;i=i+1) begin
					if (ramArg == 2'b00) begin 
						if ((ramInput[i] == 0) && (i == 0)) n = n + 0;
						else if ((ramInput[i] == 1) && (i == 0)) n = n - 0;

						if ((ramInput[i] == 0) && (i == 1)) n = n + 1;
						else if ((ramInput[i] == 1) && (i == 1)) n = n - 1;

						if ((ramInput[i] == 0) && (i == 2)) n = n + (2*1);
						else if ((ramInput[i] == 1) && (i == 2)) n = n - (2*1);
						
						if ((ramInput[i] == 0) && (i == 3)) n = n + (3*(1**2));
						else if ((ramInput[i] == 1) && (i == 3)) n = n - (3*(1**2));

						if ((ramInput[i] == 0) && (i == 4)) n = n + (4*(1**3));
						else if ((ramInput[i] == 1) && (i == 4)) n = n - (4*(1**3));
					end
					else if (ramArg == 2'b01) begin 
						if ((ramInput[i] == 0) && (i == 0)) n = n + 0;
						else if ((ramInput[i] == 1) && (i == 0)) n = n - 0;
						
						if ((ramInput[i] == 0) && (i == 1)) n = n + 1;
						else if ((ramInput[i] == 1) && (i == 1)) n = n - 1;
						
						if ((ramInput[i] == 0) && (i == 2)) n = n + (2*2);
						else if ((ramInput[i] == 1) && (i == 2)) n = n - (2*2);

						if ((ramInput[i] == 0) && (i == 3)) n = n + (3*(2**2));
						else if ((ramInput[i] == 1) && (i == 3)) n = n - (3*(2**2));

						if ((ramInput[i] == 0) && (i == 4)) n = n + (4*(2**3));
						else if ((ramInput[i] == 1) && (i == 4)) n = n - (4*(2**3));
					end
					else if (ramArg == 2'b10) begin 
						if ((ramInput[i] == 0) && (i == 0)) n = n + 0;
						else if ((ramInput[i] == 1) && (i == 0)) n = n - 0;
						
						if ((ramInput[i] == 0) && (i == 1)) n = n + 1;
						else if ((ramInput[i] == 1) && (i == 1)) n = n - 1;
						
						if ((ramInput[i] == 0) && (i == 2)) n = n + (2*(-1));
						else if ((ramInput[i] == 1) && (i == 2)) n = n - (2*(-1));
						
						if ((ramInput[i] == 0) && (i == 3)) n = n + (3*((-1)**2));
						else if ((ramInput[i] == 1) && (i == 3)) n = n - (3*((-1)**2));
						
						if ((ramInput[i] == 0) && (i == 4)) n = n + (4*((-1)**3));
						else if ((ramInput[i] == 1) && (i == 4)) n = n - (4*((-1)**3));
					end
					else begin 
						if ((ramInput[i] == 0) && (i == 0)) n = n + 0;
						else if ((ramInput[i] == 1) && (i == 0)) n = n - 0;
						
						if ((ramInput[i] == 0) && (i == 1)) n = n + 1;
						else if ((ramInput[i] == 1) && (i == 1)) n = n - 1;
						
						if ((ramInput[i] == 0) && (i == 2)) n = n + (2*(-2));
						else if ((ramInput[i] == 1) && (i == 2)) n = n - (2*(-2));
						
						if ((ramInput[i] == 0) && (i == 3)) n = n + (3*((-2)**2));
						else if ((ramInput[i] == 1) && (i == 3)) n = n - (3*((-2)**2));
						
						if ((ramInput[i] == 0) && (i == 4)) n = n + (4*((-2)**3));
						else if ((ramInput[i] == 1) && (i == 4)) n = n - (4*((-2)**3));
					end
				end
				if (n>=0) forconversion = n;
				else forconversion = -n;
				if (n >= 0) mem[ramAddr] = {1'b0,forconversion};
				else mem[ramAddr] = {1'b1,forconversion};
				n = 0;
			end
		end
	end

	always @(ramMode or ramAddr) begin // Read
		if (ramMode == 1'b0) begin
			ramOutput = mem[ramAddr];
		end
	end
endmodule

module polMEM(input mode, input [3:0] memAddr, input op, input [1:0] arg, input CLK, output wire [8:0] memOutput);

	/*Don't edit this module*/
	wire [4:0]  romOutput;

	lab4ROM RO(memAddr, romOutput);
	lab4RAM RA(mode, memAddr, romOutput, op, arg, CLK, memOutput);



endmodule
