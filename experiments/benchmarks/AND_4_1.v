module top (
    n7, n9, n11, n13, n14, n15, in1, in2, in3, in4
);
    input in1, in2, in3, in4;
    output n7, n9, n11, n13, n14, n15;
    wire n5, n6, n8, n10, n12;
    assign  n5 = in1 & in2;
    assign  n6 =  n5 & in3;
    assign  n7 =  n6 & in4;
    assign  n8 = in1 & in3;
    assign  n9 =  n8 & in4;
    assign n10 = in1 & in4;
    assign n11 = n10 & in2;
    assign n12 = in2 & in3;
    assign n13 = n12 & in4;
    assign n14 = in2 & in4;
    assign n15 = in3 & in4;
endmodule