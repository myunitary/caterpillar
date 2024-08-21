module top (
    out1, out2, in1, in2, in3
);
    input in1, in2, in3;
    output out1, out2;
    wire n4, n5;
    assign n4 = in1 & in2;
    assign n5 = in2 & in3;
    assign out1 = n4 & n5;
    assign out2 = in1 & in3;
endmodule