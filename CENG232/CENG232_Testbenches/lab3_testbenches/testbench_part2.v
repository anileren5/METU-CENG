// Prepared by Anıl Eren Göçer

`timescale 1ns /1ps

module testbench_lab3_2;
    // Inputs 
    reg [4:0] smartCode;
    reg CLK;
    reg lab;
    reg [1:0] mode;
    // Outputs
    wire [5:0] numOfStuInMera;
    wire [5:0] numOfStuInDigital;
    wire restrictionWarnMera;
    wire isFullMera;
    wire isEmptyMera;
    wire unlockMera;
    wire restrictionWarnDigital;
    wire isFullDigital;
    wire isEmptyDigital;
    wire unlockDigital;


    // (UUT)
    lab3_2 uut (
        .smartCode(smartCode),
        .CLK(CLK),
        .lab(lab),
        .mode(mode),
        .numOfStuInMera(numOfStuInMera),
        .numOfStuInDigital(numOfStuInDigital),
        .restrictionWarnMera(restrictionWarnMera),
        .isFullMera(isFullMera),
        .isEmptyMera(isEmptyMera),
        .unlockMera(unlockMera),
        .restrictionWarnDigital(restrictionWarnDigital),
        .isFullDigital(isFullDigital),
        .isEmptyDigital(isEmptyDigital),
        .unlockDigital(unlockDigital)
    );


    // Clock Related
    // At 5, 15, 25, .. CLK will change 0->1
    // At 10, 20, 30, .. CLK will change 1->0
    initial CLK = 0;
    always #5 CLK = ~CLK;

    

    initial begin
        $display("---------------------");
        $display("---------------------");
        $display("---------------------");
        $display("---------------------");
        // $monitor("Time: %.d | SmartCode=%.b mode=%.b lab=%.b | Mera: Warn=%.b Full=%.b Empty=%.b unlock=%.b num=%.b| Digital: Warn=%.b Full=%.b Empty=%.b unlock=%.b num=%.b",$time,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,numOfStuInMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInDigital);
        
        #1; 
      $display("Initials--> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);
        
        #1; 
        smartCode = 5'b00000; mode = 2'b01; lab = 1'b0;
        #5; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

        
        #1; 
        smartCode = 5'b11001; mode = 2'b01; lab = 1'b1;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

        
        #1; 
        smartCode = 5'b11011; mode = 2'b01; lab = 1'b1;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

        #1; 
        smartCode = 5'bxxxxx; mode = 2'b00; lab = 1'b1;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

        #1; 
        smartCode = 5'bxxxxx; mode = 2'b00; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

        #1; 
        smartCode = 5'bxxxxx; mode = 2'b10; lab = 1'bx;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

        #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

        #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);
        
        #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'b00111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      #1; 
        smartCode = 5'bxxxxx; mode = 2'b10; lab = 1'bx;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

      
      $display("After some time, now there are 29 students in Digital Lab.");
        
        #1; 
        smartCode = 5'b11000; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);
        

        #1; 
        smartCode = 5'b11110; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

        #1; 
        smartCode = 5'b11111; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);

        #1; 
        smartCode = 5'b11110; mode = 2'b01; lab = 1'b0;
        #9; 
      $display("Current --> |SC:%.b|M:%.b|L:%.b|Wm:%.b|Fm:%.b|Em:%.b|Um:%.b|Wd:%.b|Fd:%.b|Ed:%.b|Ud:%.b|Nm%.d |Nd:%.d",smartCode,mode,lab,restrictionWarnMera,isFullMera,isEmptyMera,unlockMera,restrictionWarnDigital,isFullDigital,isEmptyDigital,unlockDigital,numOfStuInMera,numOfStuInDigital);



        $display("End of the testbench!");
        $display("---------------------");
        $display("---------------------");
        $display("---------------------");
        $display("---------------------");
        $finish;
    end
endmodule