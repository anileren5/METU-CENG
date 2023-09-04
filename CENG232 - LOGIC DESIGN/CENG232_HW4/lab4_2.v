`timescale 1ns / 1ps

module lab4_2(// INPUTS
              input wire      mode,
              input wire[2:0] opCode,
              input wire[3:0] value,
              input clk,
              input reset,
              // OUTPUTS
              output reg[9:0] result,
              output reg cacheFull,
              output reg invalidOp,
              output reg overflow);

    reg [6:0] instruction_buffer [0:31]; // ABCDEFG -> ABC: op-code  DEFG: value
    reg [9:0] p1; // The Second Last Result
    integer temp; // Temporary variable
    integer n_instruction;
    integer start_offset; // Index where the new loop will start
    integer i; // Index of the instruction to be executed
    
    initial begin
        i = 0;
        p1 = 10'bxxxxxxxxxx;
        n_instruction = 0;
        start_offset = 0;
        cacheFull = 1'b0;
        overflow = 1'b0;
        invalidOp = 1'b0;
        instruction_buffer[0] = 7'bxxxxxxx;
        instruction_buffer[1] = 7'bxxxxxxx;
        instruction_buffer[2] = 7'bxxxxxxx;
        instruction_buffer[3] = 7'bxxxxxxx;
        instruction_buffer[4] = 7'bxxxxxxx;
        instruction_buffer[5] = 7'bxxxxxxx;
        instruction_buffer[6] = 7'bxxxxxxx;
        instruction_buffer[7] = 7'bxxxxxxx;
        instruction_buffer[8] = 7'bxxxxxxx;
        instruction_buffer[9] = 7'bxxxxxxx;
        instruction_buffer[10] = 7'bxxxxxxx;
        instruction_buffer[11] = 7'bxxxxxxx;
        instruction_buffer[12] = 7'bxxxxxxx;
        instruction_buffer[13] = 7'bxxxxxxx;
        instruction_buffer[14] = 7'bxxxxxxx;
        instruction_buffer[15] = 7'bxxxxxxx;
        instruction_buffer[16] = 7'bxxxxxxx;
        instruction_buffer[17] = 7'bxxxxxxx;
        instruction_buffer[18] = 7'bxxxxxxx;
        instruction_buffer[19] = 7'bxxxxxxx;
        instruction_buffer[20] = 7'bxxxxxxx;
        instruction_buffer[21] = 7'bxxxxxxx;
        instruction_buffer[22] = 7'bxxxxxxx;
        instruction_buffer[23] = 7'bxxxxxxx;
        instruction_buffer[24] = 7'bxxxxxxx;
        instruction_buffer[25] = 7'bxxxxxxx;
        instruction_buffer[26] = 7'bxxxxxxx;
        instruction_buffer[27] = 7'bxxxxxxx;
        instruction_buffer[28] = 7'bxxxxxxx;
        instruction_buffer[29] = 7'bxxxxxxx;
        instruction_buffer[30] = 7'bxxxxxxx;
        instruction_buffer[31] = 7'bxxxxxxx;
    end

    
    
    
    always @(posedge clk or posedge reset) begin
        if (reset == 1'b1) begin
            instruction_buffer[0] = 7'bxxxxxxx;
            instruction_buffer[1] = 7'bxxxxxxx;
            instruction_buffer[2] = 7'bxxxxxxx;
            instruction_buffer[3] = 7'bxxxxxxx;
            instruction_buffer[4] = 7'bxxxxxxx;
            instruction_buffer[5] = 7'bxxxxxxx;
            instruction_buffer[6] = 7'bxxxxxxx;
            instruction_buffer[7] = 7'bxxxxxxx;
            instruction_buffer[8] = 7'bxxxxxxx;
            instruction_buffer[9] = 7'bxxxxxxx;
            instruction_buffer[10] = 7'bxxxxxxx;
            instruction_buffer[11] = 7'bxxxxxxx;
            instruction_buffer[12] = 7'bxxxxxxx;
            instruction_buffer[13] = 7'bxxxxxxx;
            instruction_buffer[14] = 7'bxxxxxxx;
            instruction_buffer[15] = 7'bxxxxxxx;
            instruction_buffer[16] = 7'bxxxxxxx;
            instruction_buffer[17] = 7'bxxxxxxx;
            instruction_buffer[18] = 7'bxxxxxxx;
            instruction_buffer[19] = 7'bxxxxxxx;
            instruction_buffer[20] = 7'bxxxxxxx;
            instruction_buffer[21] = 7'bxxxxxxx;
            instruction_buffer[22] = 7'bxxxxxxx;
            instruction_buffer[23] = 7'bxxxxxxx;
            instruction_buffer[24] = 7'bxxxxxxx;
            instruction_buffer[25] = 7'bxxxxxxx;
            instruction_buffer[26] = 7'bxxxxxxx;
            instruction_buffer[27] = 7'bxxxxxxx;
            instruction_buffer[28] = 7'bxxxxxxx;
            instruction_buffer[29] = 7'bxxxxxxx;
            instruction_buffer[30] = 7'bxxxxxxx;
            instruction_buffer[31] = 7'bxxxxxxx;
            p1 = 10'bxxxxxxxxxx;
            result = 10'b0000000000; 
            n_instruction = 0;
            start_offset = 0;
            i = 0;
            cacheFull = 1'b0;
            invalidOp = 1'b0;
            overflow = 1'b0;        
        end
        
        
        else begin
        if (mode == 1'b0) begin // Load Mode
            if ((opCode == 3'b011) || (opCode == 3'b111)) begin // Invalid Op-code
                invalidOp = 1'b1;
                if (n_instruction >= 32) cacheFull = 1'b1;
                else cacheFull = 1'b0;
            end
            else begin // Valid Op-Code
                invalidOp = 1'b0;
                if (n_instruction >= 32) cacheFull = 1'b1;
                else begin
                    cacheFull = 1'b0;
                    instruction_buffer[n_instruction] = {opCode,value};
                    n_instruction = n_instruction + 1;
                end
            end
        end

        else begin // Execute (Calculate) Mode
            invalidOp = 1'b0;
            if (n_instruction == 0) begin // There is no instruction in the buffer. 
                overflow = 1'b0;
                p1 = result;
                result = 10'b0000000000;
            end
            else begin
                if (instruction_buffer[i][6:4] == 3'b000) begin // ADD
                    if (result === 10'bxxxxxxxxxx) begin
                        temp = 10'b0000000000 + instruction_buffer[i][3:0];
                        p1 = result;
                    end
                    else begin
                        temp = result + instruction_buffer[i][3:0];
                        p1 = result;
                    end
                    if (temp > 1023) begin
                        overflow = 1'b1;
                        result = temp - 1024;
                    end
                    else begin
                        overflow = 1'b0;
                        result = temp;
                    end
                    if (i == (n_instruction - 1)) i = start_offset;
                    else i = i + 1;
                end
                else if (instruction_buffer[i][6:4] == 3'b001) begin // ADD2
                    if ((result === 10'bxxxxxxxxxx) && (p1 === 10'bxxxxxxxxxx)) begin
                        temp = 10'b0000000000 + 10'b0000000000 + instruction_buffer[i][3:0];
                        p1 = result;
                    end
                    else if ((result !== 10'bxxxxxxxxxx) && (p1 === 10'bxxxxxxxxxx)) begin
                        temp = result + 10'b0000000000 + instruction_buffer[i][3:0];
                        p1 = result;
                    end
                    else if ((result === 10'bxxxxxxxxxx) && (p1 !== 10'bxxxxxxxxxx)) begin // Useless condition; just for clarity
                        temp = p1 + 10'b0000000000 + instruction_buffer[i][3:0];
                        p1 = result;
                    end
                    else begin
                        temp = result + p1 + instruction_buffer[i][3:0];
                        p1 = result;
                    end
                    if (temp > 1023) begin
                        overflow = 1'b1;
                        result = temp - 1024;
                    end
                    else begin
                        overflow = 1'b0;
                        result = temp;
                    end
                    if (i == (n_instruction - 1)) i = start_offset;
                    else i = i + 1;
                end
                else if (instruction_buffer[i][6:4] == 3'b010) begin // FMA
                    if ((result === 10'bxxxxxxxxxx) && (p1 === 10'bxxxxxxxxxx)) begin
                        temp = (10'b0000000000 * 10'b0000000000) + instruction_buffer[i][3:0];
                        p1 = result;
                    end
                    else if ((result !== 10'bxxxxxxxxxx) && (p1 === 10'bxxxxxxxxxx)) begin
                        temp = (10'b0000000000 * result) + instruction_buffer[i][3:0];
                        p1 = result;
                    end
                    else if ((result === 10'bxxxxxxxxxx) && (p1 !== 10'bxxxxxxxxxx)) begin // Useless condition; just for clarity
                        temp = (10'b0000000000 * p1) + instruction_buffer[i][3:0];
                        p1 = result;
                    end
                    else begin
                        temp = (p1 * result) + instruction_buffer[i][3:0];
                        p1 = result;
                    end
                    if (temp > 1023) begin
                        overflow = 1'b1;
                        result = temp - 1024;
                    end
                    else begin
                        overflow = 1'b0;
                        result = temp;
                    end
                    if (i == (n_instruction - 1)) i = start_offset;
                    else i = i + 1;
                end
                else if (instruction_buffer[i][6:4] == 3'b100) begin // POPC
                    temp = result[0] + result[1] + result[2] + result[3] + result[4] + result [5] + result[6] + result[7] + result[8] + result[9];
                    p1 = result;
                    if (temp > 1023) begin
                        overflow = 1'b1;
                        result = temp - 1024;
                    end
                    else begin
                        overflow = 1'b0;
                        result = temp;
                    end
                    if (i == (n_instruction - 1)) i = start_offset;
                    else i = i + 1;               
                end
                else if (instruction_buffer[i][6:4] == 3'b101) begin // BREV
                    temp = (result[0]*(2**9)) + (result[1]*(2**8)) + (result[2]*(2**7)) + (result[3]*(2**6)) + (result[4]*(2**5)) + (result[5]*(2**4)) + (result[6]*(2**3)) + (result[7]*(2**2)) + (result[8]*(2**1)) + (result[9]*(2**0));
                    p1 = result;
                    if (temp > 1023) begin
                        overflow = 1'b1;
                        result = temp - 1024;
                    end
                    else begin
                        overflow = 1'b0;
                        result = temp;
                    end
                    if (i == (n_instruction - 1)) i = start_offset;
                    else i = i + 1;                           
                end
                else if (instruction_buffer[i][6:4] == 3'b110) begin // SETR
                    start_offset = instruction_buffer[i][3:0];
                    if (i == (n_instruction - 1)) i = start_offset;
                    else i = i + 1;                   
                end
            end
        end
        end
    end

endmodule