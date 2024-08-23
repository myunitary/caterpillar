module top (
    n7, n8, n9, n10, n11, n12, n14, n15, in1, in2, in3, in4
);
    input in1, in2, in3, in4;
    output n7, n8, n9, n10, n11, n12, n14, n15;
    wire n5, n6, n13;
    assign  n5 = in1 & in2;
    assign  n6 =  n5 & in3;
    assign  n7 =  n6 & in4;
    assign  n8 =  n5 & in4;
    assign  n9 = in1 & in3;
    assign n10 = in1 & in4;
    assign n11 = in2 & in3;
    assign n12 = in2 & in4;
    assign n13 = in3 & in4;
    assign n14 = n13 & in1;
    assign n15 = n13 & in2;
endmodule