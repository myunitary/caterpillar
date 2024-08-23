module top (
    n7, n8, n9, n10, n11, n12, n13, n14, n15, in1, in2, in3, in4
);
    input in1, in2, in3, in4;
    output n7, n8, n9, n10, n11, n12, n13, n14, n15;
    wire n5, n6;
    assign  n5 = in1 & in2;
    assign  n6 = in3 & in4;
    assign  n7 =  n5 &  n6;
    assign  n8 =  n5 & in3;
    assign  n9 =  n5 & in4;
    assign n10 =  n6 & in1;
    assign n11 =  n6 & in2;
    assign n12 = in1 & in3;
    assign n13 = in1 & in4;
    assign n14 = in2 & in3;
    assign n15 = in2 & in4;
endmodule