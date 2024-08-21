module top (
    out1, out2, out3, in1, in2, in3
);
    input in1, in2, in3;
    output out1, out2, out3;
    wire n4;
    assign n4 = in1 & in2;
    assign out1 = n4 & in3;
    assign out2 = in2 & in3;
    assign out3 = in1 & in3;
endmodule