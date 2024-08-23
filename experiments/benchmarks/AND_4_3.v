module top (
    n7, n9, n11, n13, n15, in1, in2, in3, in4
);
    input in1, in2, in3, in4;
    output n7, n9, n11, n13, n15;
    wire n5, n6, n8, n10, n12, n14;
    assign  n5 = in1 & in2;
    assign  n6 = in3 & in4;
    assign  n7 =  n5 &  n6;
    assign  n8 = in1 & in3;
    assign  n9 =  n8 & in2;
    assign n10 = in1 & in4;
    assign n11 = n10 & in3;
    assign n12 = in2 & in3;
    assign n13 = n12 & in4;
    assign n14 = in2 & in4;
    assign n15 = n14 & in1;
endmodule