// Prepared by Anıl Eren Göçer
`timescale 1ns / 1ps

module testbench_ic1337;
	// Inputs
	reg I0;
	reg I1;
	reg I2;
	reg clk;
	// Outputs
	wire Q0;
	wire Q1;
	wire Z;

    // You can add additional declarations here
    // ...

	// Instantiate the Unit Under Test (UUT)
	ic1337 uut (.I0(I0),
                .I1(I1),
                .I2(I2),
                .clk(clk),
                .Q0(Q0),
                .Q1(Q1),
                .Z(Z));

    // Clock Related
    // At 5, 15, 25, .. clk will change 0->1
	// At 10, 20, 30, .. clk will change 1->0
    initial clk = 0;
    always #5 clk = ~clk;

	initial begin		
        // $monitor("Time: %.d | I0=%.b I1=%.b I2=%.b | Q0=%.b Z=%.b Q1=%.b",$time,I0,I1,I2,Q0,Z,Q1);
		$display("-----------------------------");
        $display("-----------------------------");
		$display("-----------------------------");
        $display("-----------------------------");
        #1; // 1
        if (Q0 == 1'b0 && Z == 1'b0 && Q1 == 1'b0) $display("Initial values are correct!");
        else $display("Initial values are wrong!");

        #1; // 2
        I0 = 1'b0; I1 = 1'b0; I2 = 1'b0;
        #5; // 7
        if (Q0 == 1'b0 && Z == 1'b1 && Q1 == 1'b1) $display("Case I0=0 I1=0 I2=0 Q0prev=0 Q1prev=0 is correct!");
        else $display("Case I0=0 I1=0 I2=0 Q0prev=0 Q1prev=0 is wrong!");

        #1; // 8
        I0 = 1'b0; I1 = 1'b0; I2 = 1'b0;
        #9; // 17
        if (Q0 == 1'b0 && Z == 1'b0 && Q1 == 1'b0) $display("Case I0=0 I1=0 I2=0 Q0prev=0 Q1prev=1 is correct!");
        else $display("Case I0=0 I1=0 I2=0 Q0prev=0 Q1prev=1 is wrong!");

        #1; // 18
        I0 = 1'b0; I1 = 1'b0; I2 = 1'b1;
        #9; // 27
        if (Q0 == 1'b1 && Z == 1'b0 && Q1 == 1'b1) $display("Case I0=0 I1=0 I2=1 Q0prev=0 Q1prev=0 is correct!");
        else $display("Case I0=0 I1=0 I2=1 Q0prev=0 Q1prev=0 is wrong!");

        #1; // 28
        I0 = 1'b0; I1 = 1'b0; I2 = 1'b1;
        #9; // 37
        if (Q0 == 1'b1 && Z == 1'b0 && Q1 == 1'b1) $display("Case I0=0 I1=0 I2=1 Q0prev=1 Q1prev=1 is correct!");
        else $display("Case I0=0 I1=0 I2=1 Q0prev=1 Q1prev=1 is wrong!");

        #1; // 38
        I0 = 1'b0; I1 = 1'b0; I2 = 1'b0;
        #9; // 47
        if (Q0 == 1'b1 && Z == 1'b1 && Q1 == 1'b0) $display("Case I0=0 I1=0 I2=0 Q0prev=1 Q1prev=1 is correct!");
        else $display("Case I0=0 I1=0 I2=0 Q0prev=1 Q1prev=1 is wrong!");

        #1; // 48
        I0 = 1'b0; I1 = 1'b0; I2 = 1'b0;
        #9; // 57
        if (Q0 == 1'b1 && Z == 1'b0 && Q1 == 1'b1) $display("Case I0=0 I1=0 I2=0 Q0prev=1 Q1prev=0 is correct!");
        else $display("Case I0=0 I1=0 I2=0 Q0prev=1 Q1prev=0 is wrong!");

        #1; // 58
        I0 = 1'b0; I1 = 1'b1; I2 = 1'b0;
        #9; // 67
        if (Q0 == 1'b0 && Z == 1'b0 && Q1 == 1'b0) $display("Case I0=0 I1=1 I2=0 Q0prev=1 Q1prev=1 is correct!");
        else $display("Case I0=0 I1=1 I2=0 Q0prev=1 Q1prev=1 is wrong!");

        #1; // 68
        I0 = 1'b0; I1 = 1'b1; I2 = 1'b0;
        #9; // 77
        if (Q0 == 1'b0 && Z == 1'b0 && Q1 == 1'b0) $display("Case I0=0 I1=1 I2=0 Q0prev=0 Q1prev=0 is correct!");
        else $display("Case I0=0 I1=1 I2=0 Q0prev=0 Q1prev=0 is wrong!");
      
        #1; // 78
        I0 = 1'b0; I1 = 1'b1; I2 = 1'b1;
        #9; // 87
      	if (Q0 == 1'b1 && Z == 1'b0 && Q1 == 1'b1) $display("Case I0=0 I1=1 I2=1 Q0prev=0 Q1prev=0 is correct!");
      	else $display("Case I0=0 I1=1 I2=1 Q0prev=0 Q1prev=0 is wrong!");
      
        #1; // 88
        I0 = 1'b0; I1 = 1'b1; I2 = 1'b1;
        #9; // 97
      	if (Q0 == 1'b1 && Z == 1'b0 && Q1 == 1'b1) $display("Case I0=0 I1=1 I2=1 Q0prev=1 Q1prev=1 is correct!");
      	else $display("Case I0=0 I1=1 I2=1 Q0prev=1 Q1prev=1 is wrong!");      
      
        #1; // 98
        I0 = 1'b1; I1 = 1'b0; I2 = 1'b0;
        #9; // 107
      	if (Q0 == 1'b1 && Z == 1'b1 && Q1 == 1'b0) $display("Case I0=1 I1=0 I2=0 Q0prev=1 Q1prev=1 is correct!");
      	else $display("Case I0=1 I1=0 I2=0 Q0prev=1 Q1prev=1 is wrong!");    

        #1; // 108
        I0 = 1'b0; I1 = 1'b0; I2 = 1'b1;
        #9; // 117
      	if (Q0 == 1'b1 && Z == 1'b0 && Q1 == 1'b1) $display("Case I0=0 I1=0 I2=1 Q0prev=1 Q1prev=0 is correct!");
      	else $display("Case I0= I1=0 I2=1 Q0prev=1 Q1prev=0 is wrong!");
      
        #1; // 118
        I0 = 1'b1; I1 = 1'b0; I2 = 1'b1;
        #9; // 127
      	if (Q0 == 1'b1 && Z == 1'b0 && Q1 == 1'b1) $display("Case I0=1 I1=0 I2=1 Q0prev=1 Q1prev=1 is correct!");
      	else $display("Case I0=1 I1=0 I2=1 Q0prev=1 Q1prev=1 is wrong!");    
      
      	#1; // 128
        I0 = 1'b1; I1 = 1'b1; I2 = 1'b1;
        #9; // 137
      	if (Q0 == 1'b1 && Z == 1'b0 && Q1 == 1'b1) $display("Case I0=1 I1=1 I2=1 Q0prev=1 Q1prev=1 is correct!");
      	else $display("Case I0=1 I1=1 I2=1 Q0prev=1 Q1prev=1 is wrong!");    
      
        #1; // 138
        I0 = 1'b1; I1 = 1'b1; I2 = 1'b0;
        #9; // 147
      	if (Q0 == 1'b1 && Z == 1'b1 && Q1 == 1'b0) $display("Case I0=1 I1=1 I2=0 Q0prev=1 Q1prev=1 is correct!");
      	else $display("Case I0=1 I1=1 I2=0 Q0prev=1 Q1prev=1 is wrong!");   

        #1; // 148
        I0 = 1'b0; I1 = 1'b0; I2 = 1'b1;
        #9; // 157
      	if (Q0 == 1'b1 && Z == 1'b0 && Q1 == 1'b1) $display("Case I0=0 I1=0 I2=1 Q0prev=1 Q1prev=0 is correct!");
      	else $display("Case I0=0 I1=0 I2=1 Q0prev=1 Q1prev=0 is wrong!");   

        #1; // 158
        I0 = 1'b0; I1 = 1'b0; I2 = 1'b0;
        #9; // 167
      	if (Q0 == 1'b1 && Z == 1'b1 && Q1 == 1'b0) $display("Case I0=0 I1=0 I2=0 Q0prev=1 Q1prev=1 is correct!");
      	else $display("Case I0=0 I1=0 I2=0 Q0prev=1 Q1prev=1 is wrong!");   

        #1; // 168
        I0 = 1'b0; I1 = 1'b1; I2 = 1'b0;
        #9; // 177
      	if (Q0 == 1'b0 && Z == 1'b0 && Q1 == 1'b0) $display("Case I0=0 I1=1 I2=0 Q0prev=1 Q1prev=0 is correct!");
      	else $display("Case I0=0 I1=1 I2=0 Q0prev=1 Q1prev=0 is wrong!");   
        
        #1; // 178
        I0 = 1'b1; I1 = 1'b1; I2 = 1'b1;
        #9; // 187
      	if (Q0 == 1'b1 && Z == 1'b1 && Q1 == 1'b0) $display("Case I0=1 I1=1 I2=1 Q0prev=0 Q1prev=0 is correct!");
      	else $display("Case I0=1 I1=1 I2=1 Q0prev=0 Q1prev=0 is wrong!");   

        #1; // 198
        I0 = 1'b1; I1 = 1'b1; I2 = 1'b1;
        #9; // 207
      	if (Q0 == 1'b1 && Z == 1'b1 && Q1 == 1'b0) $display("Case I0=1 I1=1 I2=1 Q0prev=1 Q1prev=0 is correct!");
      	else $display("Case I0=1 I1=1 I2=1 Q0prev=1  Q1prev=0 is wrong!");   

        #1; // 208
        I0 = 1'b1; I1 = 1'b1; I2 = 1'b0;
        #9; // 217
      	if (Q0 == 1'b1 && Z == 1'b0 && Q1 == 1'b1) $display("Case I0=1 I1=1 I2=0 Q0prev=1 Q1prev=0 is correct!");
      	else $display("Case I0=1 I1=1 I2=0 Q0prev=1  Q1prev=0 is wrong!");   


        #1; // 218
        I0 = 1'b1; I1 = 1'b1; I2 = 1'b0;
        #9; // 227
      	if (Q0 == 1'b1 && Z == 1'b1&& Q1 == 1'b0) $display("Case I0=1 I1=1 I2=0 Q0prev=1 Q1prev=1 is correct!");
      	else $display("Case I0=1 I1=1 I2=0 Q0prev=1  Q1prev=1 is wrong!");  
        
        #1; // 228
        I0 = 1'b1; I1 = 1'b0; I2 = 1'b1;
        #9; // 237
      	if (Q0 == 1'b1 && Z == 1'b1 && Q1 == 1'b0) $display("Case I0=1 I1=0 I2=1 Q0prev=1 Q1prev=0 is correct!");
      	else $display("Case I0=1 I1=0 I2=1 Q0prev=1  Q1prev=0 is wrong!");  

        #1; // 238
        I0 = 1'b0; I1 = 1'b1; I2 = 1'b1;
        #9; // 247
      	if (Q0 == 1'b1 && Z == 1'b0 && Q1 == 1'b1) $display("Case I0=0 I1=1 I2=1 Q0prev=1 Q1prev=0 is correct!");
      	else $display("Case I0=0 I1=1 I2=1 Q0prev=1  Q1prev=0 is wrong!");  

        #1; // 248
        I0 = 1'b1; I1 = 1'b1; I2 = 1'b0;
        #9; // 257
      	if (Q0 == 1'b1 && Z == 1'b1 && Q1 == 1'b0) $display("Case I0=0 I1=0 I2=0 Q0prev=1 Q1prev=1 is correct!");
      	else $display("Case I0=0 I1=0 I2=0 Q0prev=1  Q1prev=1 is wrong!");  

        #1; // 258
        I0 = 1'b1; I1 = 1'b0; I2 = 1'b0;
        #9; // 267
      	if (Q0 == 1'b1 && Z == 1'b1 && Q1 == 1'b0) $display("Case I0=1 I1=0 I2=0 Q0prev=1 Q1prev=0 is correct!");
      	else $display("Case I0=1 I1=0 I2=0 Q0prev=1  Q1prev=0 is wrong!");   

        #1; // 258
        I0 = 1'b1; I1 = 1'b0; I2 = 1'b0;
        #9; // 267
      	if (Q0 == 1'b1 && Z == 1'b1 && Q1 == 1'b0) $display("Case I0=1 I1=0 I2=0 Q0prev=1 Q1prev=0 is correct!");
      	else $display("Case I0=1 I1=0 I2=0 Q0prev=1  Q1prev=0 is wrong!");  

        #1; // 268
        I0 = 1'b0; I1 = 1'b1; I2 = 1'b0;
        #9; // 277
      	if (Q0 == 1'b0 && Z == 1'b0 && Q1 == 1'b0) $display("Case I0=0 I1=1 I2=0 Q0prev=1 Q1prev=0 is correct!");
      	else $display("Case I0=0 I1=1 I2=0 Q0prev=1  Q1prev=0 is wrong!");   

        
        #1; // 278
        I0 = 1'b0; I1 = 1'b0; I2 = 1'b0;
        #9; // 287
      	if (Q0 == 1'b0 && Z == 1'b1 && Q1 == 1'b1) $display("Case I0=0 I1=0 I2=0 Q0prev=0 Q1prev=0 is correct!");
      	else $display("Case I0=0 I1=0 I2=0 Q0prev=0  Q1prev=0 is wrong!");
      
        #1; // 288
        I0 = 1'b0; I1 = 1'b1; I2 = 1'b0;
        #9; // 298
      	if (Q0 == 1'b0 && Z == 1'b0 && Q1 == 1'b0) $display("Case I0=0 I1=1 I2=0 Q0prev=0 Q1prev=1 is correct!");
      	else $display("Case I0=0 I1=1 I2=0 Q0prev=0  Q1prev=1 is wrong!");

        #1; // 298  
        I0 = 1'b1; I1 = 1'b1; I2 = 1'b0;
        #9; // 307
      	if (Q0 == 1'b0 && Z == 1'b1 && Q1 == 1'b1) $display("Case I0=1 I1=1 I2=0 Q0prev=0 Q1prev=0 is correct!");
      	else $display("Case I0=1 I1=1 I2=0 Q0prev=0  Q1prev=0 is wrong!");   
      
      
        #1; // 308 
        I0 = 1'b1; I1 = 1'b0; I2 = 1'b1;
        #9; // 317
      	if (Q0 == 1'b1 && Z == 1'b0 && Q1 == 1'b1) $display("Case I0=1 I1=0 I2=1 Q0prev=0 Q1prev=1 is correct!");
      	else $display("Case I0=1 I1=0 I2=1 Q0prev=0  Q1prev=1 is wrong!"); 
      
      
      	#1; // 318 
        I0 = 1'b0; I1 = 1'b1; I2 = 1'b0;
        #9; // 327
      	if (Q0 == 1'b0 && Z == 1'b0 && Q1 == 1'b0) $display("Case I0=0 I1=1 I2=0 Q0prev=1 Q1prev=1 is correct!");
      	else $display("Case I0=1 I1=1 I2=0 Q0prev=1  Q1prev=1 is wrong!");  
        
        #1; // 328 
        I0 = 1'b1; I1 = 1'b0; I2 = 1'b1;
        #9; // 337
      	if (Q0 == 1'b1 && Z == 1'b1 && Q1 == 1'b0) $display("Case I0=1 I1=0 I2=0 Q0prev=0 Q1prev=0 is correct!");
      	else $display("Case I0=1 I1=0 I2=0 Q0prev=0  Q1prev=0 is wrong!"); 
      
      	#1; // 338 
        I0 = 1'b0; I1 = 1'b1; I2 = 1'b0;
        #9; // 347
      	if (Q0 == 1'b0 && Z == 1'b0 && Q1 == 1'b0) $display("Case I0=0 I1=1 I2=0 Q0prev=1 Q1prev=0 is correct!");
      	else $display("Case I0=0 I1=1 I2=0 Q0prev=1  Q1prev=0 is wrong!"); 
      
        #1; // 348 
        I0 = 1'b1; I1 = 1'b0; I2 = 1'b0;
        #9; // 357
      	if (Q0 == 1'b0 && Z == 1'b0 && Q1 == 1'b0) $display("Case I0=1 I1=0 I2=0 Q0prev=0 Q1prev=0 is correct!");
      	else $display("Case I0=1 I1=0 I2=0 Q0prev=0  Q1prev=0 is wrong!");

      
        #1; // 358 
        I0 = 1'b0; I1 = 1'b0; I2 = 1'b0;
        #9; // 367
      	if (Q0 == 1'b0 && Z == 1'b1 && Q1 == 1'b1) $display("Case I0=0 I1=0 I2=0 Q0prev=0 Q1prev=0 is correct!");
      	else $display("Case I0=0 I1=0 I2=0 Q0prev=0  Q1prev=0 is wrong!");


        #1; // 368 
        I0 = 1'b0; I1 = 1'b1; I2 = 1'b1;
        #9; // 377
      	if (Q0 == 1'b1 && Z == 1'b0 && Q1 == 1'b1) $display("Case I0=0 I1=1 I2=1 Q0prev=0 Q1prev=1 is correct!");
      	else $display("Case I0=0 I1=1 I2=1 Q0prev=0  Q1prev=1 is wrong!");
      
        #1; // 378 
        I0 = 1'b0; I1 = 1'b1; I2 = 1'b0;
        #9; // 387
      	if (Q0 == 1'b0 && Z == 1'b0 && Q1 == 1'b0) $display("Case I0=0 I1=1 I2=0 Q0prev=1 Q1prev=1 is correct!");
      	else $display("Case I0=0 I1=1 I2=0 Q0prev=1  Q1prev=1 is wrong!");

        #1; // 388 
        I0 = 1'b0; I1 = 1'b0; I2 = 1'b0;
        #9; // 397
      	if (Q0 == 1'b0 && Z == 1'b1 && Q1 == 1'b1) $display("Case I0=0 I1=0 I2=0 Q0prev=0 Q1prev=0 is correct!");
      	else $display("Case I0=0 I1=0 I2=0 Q0prev=0  Q1prev=0 is wrong!");

        #1; // 408 
        I0 = 1'b1; I1 = 1'b0; I2 = 1'b0;
        #9; // 417
      	if (Q0 == 1'b0 && Z == 1'b0 && Q1 == 1'b0) $display("Case I0=1 I1=0 I2=0 Q0prev=0 Q1prev=1 is correct!");
      	else $display("Case I0=1 I1=0 I2=0 Q0prev=0  Q1prev=1 is wrong!");      
      
        #1; // 418 
        I0 = 1'b0; I1 = 1'b0; I2 = 1'b0;
        #9; // 427
      	if (Q0 == 1'b0 && Z == 1'b1 && Q1 == 1'b1) $display("Case I0=0 I1=0 I2=0 Q0prev=0 Q1prev=0 is correct!");
      	else $display("Case I0=0 I1=0 I2=0 Q0prev=0  Q1prev=0 is wrong!");  
      
      
      	#1; // 428 
        I0 = 1'b1; I1 = 1'b1; I2 = 1'b0;
        #9; // 437
      	if (Q0 == 1'b0 && Z == 1'b0 && Q1 == 1'b0) $display("Case I0=1 I1=1 I2=0 Q0prev=0 Q1prev=1 is correct!");
      	else $display("Case I0=1 I1=1 I2=0 Q0prev=0  Q1prev=1 is wrong!");
      
        #1; // 438 
        I0 = 1'b0; I1 = 1'b0; I2 = 1'b0;
        #9; // 447
      	if (Q0 == 1'b0 && Z == 1'b1 && Q1 == 1'b1) $display("Case I0=0 I1=0 I2=0 Q0prev=0 Q1prev=0 is correct!");
      	else $display("Case I0=0 I1=0 I2=0 Q0prev=0  Q1prev=0 is wrong!");     
      
      	#1; // 418 
        I0 = 1'b1; I1 = 1'b1; I2 = 1'b1;
        #9; // 427
      	if (Q0 == 1'b1 && Z == 1'b0 && Q1 == 1'b1) $display("Case I0=1 I1=1 I2=1 Q0prev=0 Q1prev=1 is correct!");
      	else $display("Case I0=1 I1=1 I2=1 Q0prev=0  Q1prev=1 is wrong!"); 
      
      	$display("End of the testbench!");
		$display("-----------------------------");
        $display("-----------------------------");
		$display("-----------------------------");
        $display("-----------------------------");
        $finish;
	end
endmodule
