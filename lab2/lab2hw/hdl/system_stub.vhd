-------------------------------------------------------------------------------
-- system_stub.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity system_stub is
  port (
    RS232_Uart_1_sout : out std_logic;
    RS232_Uart_1_sin : in std_logic;
    RESET : in std_logic;
    Push_Buttons_5Bits_TRI_I : in std_logic_vector(0 to 4);
    LEDs_8Bits_TRI_O : out std_logic_vector(7 downto 0);
    GCLK : in std_logic;
    DIP_Switches_8Bits_TRI_I : in std_logic_vector(7 downto 0);
    pitiful_0_S_AXI_ACLK_pin : in std_logic;
    pitiful_0_S_AXI_ARESETN_pin : in std_logic;
    pitiful_0_S_AXI_AWADDR_pin : in std_logic_vector(31 downto 0);
    pitiful_0_S_AXI_AWVALID_pin : in std_logic;
    pitiful_0_S_AXI_WDATA_pin : in std_logic_vector(31 downto 0);
    pitiful_0_S_AXI_WSTRB_pin : in std_logic_vector(3 downto 0);
    pitiful_0_S_AXI_WVALID_pin : in std_logic;
    pitiful_0_S_AXI_BREADY_pin : in std_logic;
    pitiful_0_S_AXI_ARADDR_pin : in std_logic_vector(31 downto 0);
    pitiful_0_S_AXI_ARVALID_pin : in std_logic;
    pitiful_0_S_AXI_RREADY_pin : in std_logic;
    pitiful_0_S_AXI_ARREADY_pin : out std_logic;
    pitiful_0_S_AXI_RDATA_pin : out std_logic_vector(31 downto 0);
    pitiful_0_S_AXI_RRESP_pin : out std_logic_vector(1 downto 0);
    pitiful_0_S_AXI_RVALID_pin : out std_logic;
    pitiful_0_S_AXI_WREADY_pin : out std_logic;
    pitiful_0_S_AXI_BRESP_pin : out std_logic_vector(1 downto 0);
    pitiful_0_S_AXI_BVALID_pin : out std_logic;
    pitiful_0_S_AXI_AWREADY_pin : out std_logic
  );
end system_stub;

architecture STRUCTURE of system_stub is

  component system is
    port (
      RS232_Uart_1_sout : out std_logic;
      RS232_Uart_1_sin : in std_logic;
      RESET : in std_logic;
      Push_Buttons_5Bits_TRI_I : in std_logic_vector(0 to 4);
      LEDs_8Bits_TRI_O : out std_logic_vector(7 downto 0);
      GCLK : in std_logic;
      DIP_Switches_8Bits_TRI_I : in std_logic_vector(7 downto 0);
      pitiful_0_S_AXI_ACLK_pin : in std_logic;
      pitiful_0_S_AXI_ARESETN_pin : in std_logic;
      pitiful_0_S_AXI_AWADDR_pin : in std_logic_vector(31 downto 0);
      pitiful_0_S_AXI_AWVALID_pin : in std_logic;
      pitiful_0_S_AXI_WDATA_pin : in std_logic_vector(31 downto 0);
      pitiful_0_S_AXI_WSTRB_pin : in std_logic_vector(3 downto 0);
      pitiful_0_S_AXI_WVALID_pin : in std_logic;
      pitiful_0_S_AXI_BREADY_pin : in std_logic;
      pitiful_0_S_AXI_ARADDR_pin : in std_logic_vector(31 downto 0);
      pitiful_0_S_AXI_ARVALID_pin : in std_logic;
      pitiful_0_S_AXI_RREADY_pin : in std_logic;
      pitiful_0_S_AXI_ARREADY_pin : out std_logic;
      pitiful_0_S_AXI_RDATA_pin : out std_logic_vector(31 downto 0);
      pitiful_0_S_AXI_RRESP_pin : out std_logic_vector(1 downto 0);
      pitiful_0_S_AXI_RVALID_pin : out std_logic;
      pitiful_0_S_AXI_WREADY_pin : out std_logic;
      pitiful_0_S_AXI_BRESP_pin : out std_logic_vector(1 downto 0);
      pitiful_0_S_AXI_BVALID_pin : out std_logic;
      pitiful_0_S_AXI_AWREADY_pin : out std_logic
    );
  end component;

  attribute BOX_TYPE : STRING;
  attribute BOX_TYPE of system : component is "user_black_box";

begin

  system_i : system
    port map (
      RS232_Uart_1_sout => RS232_Uart_1_sout,
      RS232_Uart_1_sin => RS232_Uart_1_sin,
      RESET => RESET,
      Push_Buttons_5Bits_TRI_I => Push_Buttons_5Bits_TRI_I,
      LEDs_8Bits_TRI_O => LEDs_8Bits_TRI_O,
      GCLK => GCLK,
      DIP_Switches_8Bits_TRI_I => DIP_Switches_8Bits_TRI_I,
      pitiful_0_S_AXI_ACLK_pin => pitiful_0_S_AXI_ACLK_pin,
      pitiful_0_S_AXI_ARESETN_pin => pitiful_0_S_AXI_ARESETN_pin,
      pitiful_0_S_AXI_AWADDR_pin => pitiful_0_S_AXI_AWADDR_pin,
      pitiful_0_S_AXI_AWVALID_pin => pitiful_0_S_AXI_AWVALID_pin,
      pitiful_0_S_AXI_WDATA_pin => pitiful_0_S_AXI_WDATA_pin,
      pitiful_0_S_AXI_WSTRB_pin => pitiful_0_S_AXI_WSTRB_pin,
      pitiful_0_S_AXI_WVALID_pin => pitiful_0_S_AXI_WVALID_pin,
      pitiful_0_S_AXI_BREADY_pin => pitiful_0_S_AXI_BREADY_pin,
      pitiful_0_S_AXI_ARADDR_pin => pitiful_0_S_AXI_ARADDR_pin,
      pitiful_0_S_AXI_ARVALID_pin => pitiful_0_S_AXI_ARVALID_pin,
      pitiful_0_S_AXI_RREADY_pin => pitiful_0_S_AXI_RREADY_pin,
      pitiful_0_S_AXI_ARREADY_pin => pitiful_0_S_AXI_ARREADY_pin,
      pitiful_0_S_AXI_RDATA_pin => pitiful_0_S_AXI_RDATA_pin,
      pitiful_0_S_AXI_RRESP_pin => pitiful_0_S_AXI_RRESP_pin,
      pitiful_0_S_AXI_RVALID_pin => pitiful_0_S_AXI_RVALID_pin,
      pitiful_0_S_AXI_WREADY_pin => pitiful_0_S_AXI_WREADY_pin,
      pitiful_0_S_AXI_BRESP_pin => pitiful_0_S_AXI_BRESP_pin,
      pitiful_0_S_AXI_BVALID_pin => pitiful_0_S_AXI_BVALID_pin,
      pitiful_0_S_AXI_AWREADY_pin => pitiful_0_S_AXI_AWREADY_pin
    );

end architecture STRUCTURE;

