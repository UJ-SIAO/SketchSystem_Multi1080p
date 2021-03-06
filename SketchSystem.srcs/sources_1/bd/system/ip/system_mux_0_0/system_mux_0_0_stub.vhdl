-- Copyright 1986-2017 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2017.4 (win64) Build 2086221 Fri Dec 15 20:55:39 MST 2017
-- Date        : Sat Sep 25 20:26:39 2021
-- Host        : DESKTOP-AC8B3K3 running 64-bit major release  (build 9200)
-- Command     : write_vhdl -force -mode synth_stub
--               D:/Project/SketchSystem_Multi1080p/SketchSystem.srcs/sources_1/bd/system/ip/system_mux_0_0/system_mux_0_0_stub.vhdl
-- Design      : system_mux_0_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7z020clg484-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity system_mux_0_0 is
  Port ( 
    i_cntrl : in STD_LOGIC;
    i_data : in STD_LOGIC_VECTOR ( 23 downto 0 );
    o_data : out STD_LOGIC_VECTOR ( 23 downto 0 )
  );

end system_mux_0_0;

architecture stub of system_mux_0_0 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "i_cntrl,i_data[23:0],o_data[23:0]";
attribute X_CORE_INFO : string;
attribute X_CORE_INFO of stub : architecture is "mux,Vivado 2017.4";
begin
end;
