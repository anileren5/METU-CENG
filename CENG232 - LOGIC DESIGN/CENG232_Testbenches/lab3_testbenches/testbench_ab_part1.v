// Prepared by Anıl Eren Göçer

`timescale 1ns/1ps

module mytestbench_ab;
    // Inputs 
    reg A;
    reg B;
    reg clk;
    // Outputs
    wire Q;

    // Instantiate the Unit Under Test (UUT)
    ab uut(.A(A),.B(B),.clk(clk),.Q(Q));

    // At 5, 15, 25, .. clk will change 0->1
    // At 10, 20, 30, .. clk will change 1->0

    initial clk = 0;
    always #5 clk = ~clk;
    initial begin
        // Set monitor for inputs and outputs.
        // $monitor("Time: %.2d | A = %.b B = %.b | Q = %.b",$time,A,B,Q);

        #1; // 1 
        if (Q != 1'b0) $display("Initial value of Q is wrong!");
        else $display("Initial value of Q is correct");

        #1; // 2
        A = 1'b0; B = 1'b0;
        #5; // 7
        if (Q == 1'b0) $display("Case A=0 B=0 Qprev=0 is correct!");
        else $display("Case A=0 B=0 Qprev=0 is wrong!");


        #1; // 8
        A = 1'b0; B = 1'b1;
        #9; // 17
        if (Q == 1'b1) $display("Case A=0 B=1 Qprev=0 is correct!");
        else $display("Case A=0 B=1 Qprev=0 is wrong!");


        #1; // 18
        A = 1'b0; B = 1'b0;
        #9; // 27
        if (Q == 1'b1) $display("Case A=0 B=0 Qprev=1 is correct!");
        else $display("Case A=0 B=0 Qprev=1 is wrong!");


        #1; // 28
        A = 1'b0; B = 1'b1;
        #9; // 37
        if (Q == 1'b1) $display("Case A=0 B=1 Qprev=1 is correct!");
        else $display("Case A=0 B=1 Qprev=1 is wrong!");

        #1; // 38
        A = 1'b1; B = 1'b0;
        #9; // 47
        if (Q == 1'b0) $display("Case A=1 B=0 Qprev=1 is correct!");
        else $display("Case A=1 B=0 Qprev=1 is wrong!");

        #10; // 57
        if (Q == 1'b0) $display("Case A=1 B=0 Qprev=0 is correct!");
        else $display("Case A=1 B=0 Qprev=0 is wrong!");
    
        #1; // 58
        A = 1'b1; B = 1'b1;
        #9; // 67
        if (Q == 1'b1) $display("Case A=1 B=1 Qprev=0 is correct!");
        else $display("Case A=1 B=1 Qprev=0 is wrong!");

        #10; // 77
        if (Q == 1'b0) $display("Case A=1 B=1 Qprev=1 is correct!");
        else $display("Case A=1 B=1 Qprev=1 is wrong!");


        $display("End of the testbench!");
        $finish;
    end
endmodule
