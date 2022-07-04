`timescale 1ns / 1ps

module ab(input A, input B, input clk, output reg Q);
    initial Q = 0;
    always@(posedge clk)
        case ({A,B})
            2'b00 : Q <= Q;
            2'b01 : Q <= 1;
            2'b10 : Q <= 0;
            2'b11 : Q <= (~Q); 
        endcase
endmodule

module ic1337(input I0, input I1, input I2, input clk, output Q0, output Q1, output Z);
    wire x1,x2,x3,x4;
    nor g1(x1,~I1,I0);
    and g2(x2,~I2,x1);
    nor g3(x3,I2,~I1);
    xnor g4(x4,x3,I0);
    ab ff1(x2,I2,clk,Q0);
    ab ff2(~I2,x4,clk,Q1);
    xor g5(Z,Q0,Q1);
endmodule
