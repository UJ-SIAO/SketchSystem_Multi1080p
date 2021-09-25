`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/06/07 20:21:24
// Design Name: 
// Module Name: conv1
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module conv1(
input        i_clk,
input [71:0] i_pixel_data,
input        i_pixel_data_valid,
output reg [7:0] o_convolved_data,
output reg   o_convolved_data_valid
    );

integer i; 

reg [15:0]stage1_data[8:0];
reg [7:0] i_pixel_data_light_s1[8:0];
//reg [7:0] temp[8:0];
reg [7:0] i_pixel_data_light_s1_threshold[8:0];
reg [15:0] curved_data[8:0];
reg [7:0] i_pixel_data_light_s2[8:0];
reg [7:0] i_pixel_data_light_s3[8:0];
reg [7:0] multData1;
reg [7:0] multData2[8:0];
reg [7:0] multData3;
reg [7:0] sumDataInt2[8:0];
reg [7:0] compared_data[8:0];
reg [15:0] compared_data1;
reg [7:0] lonely_data[7:0];
reg [7:0] multData1_s3;
reg [7:0] multData1_s4;
reg [7:0] multData1_s5;
wire [7:0] inverse_multData1_s6;
reg [15:0] multData1_s6;
(* EQUIVALENT_REGISTER_REMOVAL="NO" *) reg [15:0] dodge_data_s1;
reg [7:0] multData1_s7;
reg [7:0] multData1_s8;
wire [23:0]m_axis_dout_tdata;
reg [15:0] dodge_data_s2;

/*(* EQUIVALENT_REGISTER_REMOVAL="NO" *)reg [7:0] stage1;
(* EQUIVALENT_REGISTER_REMOVAL="NO" *)reg [7:0] stage2;
(* EQUIVALENT_REGISTER_REMOVAL="NO" *)reg [7:0] stage3;
(* EQUIVALENT_REGISTER_REMOVAL="NO" *)reg [7:0] stage4;

(* EQUIVALENT_REGISTER_REMOVAL="NO" *)reg [15:0] stage5[8:0];
(* EQUIVALENT_REGISTER_REMOVAL="NO" *)reg stage6;
(* EQUIVALENT_REGISTER_REMOVAL="NO" *)reg [7:0] stage7;
(* EQUIVALENT_REGISTER_REMOVAL="NO" *)reg [7:0] stage8;
(* EQUIVALENT_REGISTER_REMOVAL="NO" *)reg [7:0] stage9;*/

reg stage1_Vaild;
reg lightDataValid;
reg threshold_vaild;
reg curved_Vaild;
reg multDataValid;
reg sumDataInt2_vaild;
reg sumDataValid;
reg convolved_data_int_valid;
reg compared_data_valid;
reg compared_data_valid_s2;
reg dodged_data_valid_s1;
reg dodged_data_valid_s2;   
wire m_axis_dout_tvalid;   

always @(posedge i_clk)
begin
	for(i=0;i<9;i=i+1)
	begin
		stage1_data[i]  <= (i_pixel_data[i*8+:8] << 4)/10;
	end
	/*for(i=4;i<9;i=i+1)
	begin
		stage1_data[i]  <= i_pixel_data[i*8+:8];
	end*/
	stage1_Vaild    <= i_pixel_data_valid;
end

/*always @(posedge i_clk)
begin
	for(i=0;i<4;i=i+1)
	begin
		stage5[i]  <= stage1_data[i];
	end
	for(i=4;i<9;i=i+1)
	begin
		stage5[i]  <=( stage1_data[i] * 15 ) / 10;
	end
	stage6    <= stage1_Vaild;
end*/

always @(posedge i_clk)
begin
	for(i=0;i<9;i=i+1)
	begin
		i_pixel_data_light_s1[i] <= (stage1_data[i] >= 8'd255) ? 8'd255 : stage1_data[i];
	end	
	lightDataValid <= stage1_Vaild;
end

always@(*)
begin
	for(i=0;i<9;i=i+1)
	begin
		i_pixel_data_light_s1_threshold[i] <= i_pixel_data_light_s1[i] - 20;
	end	
end

/*always @(posedge i_clk)
begin
	for(i=0;i<9;i=i+1)
	begin
		i_pixel_data_light_s2[i] <= i_pixel_data_light_s1[i];
		
		threshold_vaild<= lightDataValid;
	end
end*/

always @(posedge i_clk)
begin	
	for(i=0;i<9;i=i+1)
	begin
		curved_data[i]  <= (i_pixel_data_light_s1_threshold[i] * 33 ) >> 5;  //(40,0)->(255,240)
		i_pixel_data_light_s3[i] <= i_pixel_data_light_s1[i];
	end
	curved_Vaild    <= lightDataValid;		
end

always @(posedge i_clk)
begin
	for(i=0;i<9;i=i+1)
	begin
		multData2[i] <= (i_pixel_data_light_s3[i] > 20) ?  curved_data[i]  : 8'b0; //(40,0)->(255,230)
	end
	
	multDataValid <= curved_Vaild;
end


always @(posedge i_clk)
begin
	for(i=0;i<9;i=i+1)
	begin
		sumDataInt2[i] <= 8'd255 - multData2[i];
	end
		multData3 <= multData2[4];	

	sumDataInt2_vaild<= multDataValid;
end

always @(posedge i_clk)
begin
	compared_data[0] <= (sumDataInt2[0] <= sumDataInt2[1]) ? sumDataInt2[0] : sumDataInt2[1];
	compared_data[1] <= (sumDataInt2[2] <= sumDataInt2[3]) ? sumDataInt2[2] : sumDataInt2[3];
	compared_data[2] <= (sumDataInt2[4] <= sumDataInt2[5]) ? sumDataInt2[4] : sumDataInt2[5];
	compared_data[3] <= (sumDataInt2[6] <= sumDataInt2[7]) ? sumDataInt2[6] : sumDataInt2[7];
	lonely_data[0]   <= sumDataInt2[8];
	multData1 <= multData3;
	//stage1 <= multData3;
	
	sumDataValid <= sumDataInt2_vaild;
end

always @(posedge i_clk)
begin
	compared_data[5] <= (compared_data[0] <= compared_data[1]) ? compared_data[0] : compared_data[1];
	compared_data[6] <= (compared_data[2] <= compared_data[3]) ? compared_data[2] : compared_data[3];
	lonely_data[1]   <= lonely_data[0];
	multData1_s3<=multData1;
	//stage2 <= stage1;
	
    convolved_data_int_valid <= sumDataValid;
end

always @(posedge i_clk)
begin
	compared_data[7] <= (compared_data[6] <= compared_data[5]) ? compared_data[6] : compared_data[5];
	lonely_data[2]   <=  lonely_data[1];
	multData1_s4 <= multData1_s3;
	//stage3 <= stage2;
	
	compared_data_valid <= convolved_data_int_valid;
end

always @(posedge i_clk)
begin
	compared_data1 <= (lonely_data[2] <= compared_data[7]) ? lonely_data[2] : compared_data[7];
	multData1_s5 <= multData1_s4;
	//stage4 <= stage3;
	
	compared_data_valid_s2 <= compared_data_valid;
end

always @(posedge i_clk)
begin
	dodge_data_s1	<= (compared_data1 << 8);
	multData1_s6 	<= multData1_s5;
	//stage9 <= stage4;

	dodged_data_valid_s1 <= compared_data_valid_s2;
end

assign inverse_multData1_s6 = 8'd255 - multData1_s6;

/*always @(posedge i_clk)
begin
	dodge_data_s2 <= dodge_data_s1 / multData1_s6;
	multData1_s7  <= stage9;

	dodged_data_valid_s2 <= dodged_data_valid_s1;
end*/

div_gen_0 div1 (
  .aclk(i_clk),                                      		// input wire aclk
  .s_axis_divisor_tvalid(dodged_data_valid_s1),    	// input wire s_axis_divisor_tvalid
  .s_axis_divisor_tdata(inverse_multData1_s6),      	// input wire [7 : 0] s_axis_divisor_tdata
  .s_axis_dividend_tvalid(dodged_data_valid_s1),  	// input wire s_axis_dividend_tvalid
  .s_axis_dividend_tdata(dodge_data_s1),    	// input wire [15 : 0] s_axis_dividend_tdata
  .m_axis_dout_tvalid(m_axis_dout_tvalid),          	// output wire m_axis_dout_tvalid
  .m_axis_dout_tdata(m_axis_dout_tdata)            	// output wire [23 : 0] m_axis_dout_tdata
);

always @(posedge i_clk)
begin
	multData1_s7  <= multData1_s6;
	
	//m_axis_dout_tvalid <= dodged_data_valid_s1;
end

always @(posedge i_clk)
begin
	dodge_data_s2 <= m_axis_dout_tdata[23:8];
	multData1_s8  <=multData1_s7;
	
	dodged_data_valid_s2 <= m_axis_dout_tvalid;
end


always @(posedge i_clk)
begin
	o_convolved_data <= (multData1_s8 == 255 ) ? 8'd255 : 
							(8'd255 > dodge_data_s2) ? dodge_data_s2 : 8'd255;

	o_convolved_data_valid <= dodged_data_valid_s2;
end

endmodule