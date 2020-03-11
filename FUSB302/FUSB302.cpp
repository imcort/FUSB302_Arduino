#include "FUSB302.h"

FUSB302::FUSB302()
{
  Wire.begin(18, 23);
  //Wire.setClock(50000);
}

void FUSB302::reset() {

  tcpc_write(TCPC_REG_RESET, TCPC_REG_RESET_SW_RESET);

}

//Communication
void FUSB302::tcpc_write(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(FUSB302_I2C_SLAVE_ADDR);
  Wire.write(reg & 0xFF);
  Wire.write(val & 0xFF);
  Wire.endTransmission();
  delay(1);

}

uint8_t FUSB302::tcpc_read(uint8_t reg) {
  Wire.beginTransmission(FUSB302_I2C_SLAVE_ADDR);
  Wire.write(reg & 0xFF);
  Wire.endTransmission(false);
  Wire.requestFrom(FUSB302_I2C_SLAVE_ADDR, 1, true);

  while (Wire.available())   // slave may send less than requested
  {
    return Wire.read();    // receive a byte as character
  }
  delay(1);
}

//Device ID
uint8_t FUSB302::get_chip_id() {

  return tcpc_read(TCPC_REG_DEVICE_ID);

}

//Switches 0
void FUSB302::cc_pu_switch(bool cc1, bool cc2) {

  uint8_t reg = tcpc_read(TCPC_REG_SWITCHES0);

  if (cc1)
    reg |= TCPC_REG_SWITCHES0_CC1_PU_EN;
  else
    reg &= ~TCPC_REG_SWITCHES0_CC1_PU_EN;

  if (cc2)
    reg |= TCPC_REG_SWITCHES0_CC2_PU_EN;
  else
    reg &= ~TCPC_REG_SWITCHES0_CC2_PU_EN;

  tcpc_write(TCPC_REG_SWITCHES0, reg);

}

void FUSB302::cc_vconn_switch(bool cc1, bool cc2) {

  uint8_t reg = tcpc_read(TCPC_REG_SWITCHES0);

  if (cc1)
    reg |= TCPC_REG_SWITCHES0_VCONN_CC1;
  else
    reg &= ~TCPC_REG_SWITCHES0_VCONN_CC1;

  if (cc2)
    reg |= TCPC_REG_SWITCHES0_VCONN_CC2;
  else
    reg &= ~TCPC_REG_SWITCHES0_VCONN_CC2;

  tcpc_write(TCPC_REG_SWITCHES0, reg);

}

void FUSB302::cc_meas_switch(bool cc1, bool cc2) {

  uint8_t reg = tcpc_read(TCPC_REG_SWITCHES0);

  if (cc1)
    reg |= TCPC_REG_SWITCHES0_MEAS_CC1;
  else
    reg &= ~TCPC_REG_SWITCHES0_MEAS_CC1;

  if (cc2)
    reg |= TCPC_REG_SWITCHES0_MEAS_CC2;
  else
    reg &= ~TCPC_REG_SWITCHES0_MEAS_CC2;

  tcpc_write(TCPC_REG_SWITCHES0, reg);

  delayMicroseconds(250);

}

void FUSB302::cc_pd_switch(bool cc1, bool cc2) {

  uint8_t reg = tcpc_read(TCPC_REG_SWITCHES0);

  if (cc1)
    reg |= TCPC_REG_SWITCHES0_CC1_PD_EN;
  else
    reg &= ~TCPC_REG_SWITCHES0_CC1_PD_EN;

  if (cc2)
    reg |= TCPC_REG_SWITCHES0_CC2_PD_EN;
  else
    reg &= ~TCPC_REG_SWITCHES0_CC2_PD_EN;

  tcpc_write(TCPC_REG_SWITCHES0, reg);

}

//Switches 1
void FUSB302::autocrc_config(bool en,
                             bool pwr_role,
                             uint8_t spec_rev,
                             bool datarole) {

  uint8_t reg = tcpc_read(TCPC_REG_SWITCHES1);

  if (en)
    reg |= TCPC_REG_SWITCHES1_AUTO_GCRC;
  else
    reg &= ~TCPC_REG_SWITCHES1_AUTO_GCRC;

  if (pwr_role)
    reg |= TCPC_REG_SWITCHES1_POWERROLE;
  else
    reg &= ~TCPC_REG_SWITCHES1_POWERROLE;

  if (datarole)
    reg |= TCPC_REG_SWITCHES1_DATAROLE;
  else
    reg &= ~TCPC_REG_SWITCHES1_DATAROLE;

  if (spec_rev)
    reg |= TCPC_REG_SWITCHES1_SPECREV0;
  else
    reg &= ~TCPC_REG_SWITCHES1_SPECREV0;

  tcpc_write(TCPC_REG_SWITCHES1, reg);

}

void FUSB302::cc_bmc_en(bool cc1, bool cc2) {

  uint8_t reg = tcpc_read(TCPC_REG_SWITCHES1);

  if (cc1)
    reg |= TCPC_REG_SWITCHES1_TXCC1_EN;
  else
    reg &= ~TCPC_REG_SWITCHES1_TXCC1_EN;

  if (cc2)
    reg |= TCPC_REG_SWITCHES1_TXCC2_EN;
  else
    reg &= ~TCPC_REG_SWITCHES1_TXCC2_EN;

  tcpc_write(TCPC_REG_SWITCHES1, reg);
  Serial.println("BMC:");

  Serial.println(reg, BIN);

}

//MEASURE
void FUSB302::vbus_meas_en(bool en) {

  if (en)
    tcpc_write(TCPC_REG_MEASURE, TCPC_REG_MEASURE_VBUS);
  else
    tcpc_write(TCPC_REG_MEASURE, 0);

  delayMicroseconds(250);

}

void FUSB302::mdac_set(uint8_t val) {

  uint8_t reg = tcpc_read(TCPC_REG_MEASURE);

  tcpc_write(TCPC_REG_MEASURE, (reg & 0xC0) | (val & 0x3F));

  delayMicroseconds(250);

}

//Control0
void FUSB302::cc_pu_current(uint8_t current) {

  uint8_t reg = tcpc_read(TCPC_REG_CONTROL0);

  reg &= 0xF3;
  reg |= ((current << 2) & 0x0C);

  tcpc_write(TCPC_REG_CONTROL0, reg);


}

void FUSB302::tx_fifo_flush() {

  uint8_t reg = tcpc_read(TCPC_REG_CONTROL0);

  reg |= TCPC_REG_CONTROL0_TX_FLUSH;

  tcpc_write(TCPC_REG_CONTROL0, reg);

}

void FUSB302::int_en(bool en) {

  uint8_t reg = tcpc_read(TCPC_REG_CONTROL0);
  if (en)
    reg &= ~TCPC_REG_CONTROL0_INT_MASK;
  else
    reg |= TCPC_REG_CONTROL0_INT_MASK;

  tcpc_write(TCPC_REG_CONTROL0, reg);
}

//Control1
void FUSB302::sop_prime_en(bool en) {

  uint8_t reg = tcpc_read(TCPC_REG_CONTROL1);

  if (en) {
    reg |= TCPC_REG_CONTROL1_ENSOP1;
    reg |= TCPC_REG_CONTROL1_ENSOP2;
  } else {
    reg &= ~TCPC_REG_CONTROL1_ENSOP1;
    reg &= ~TCPC_REG_CONTROL1_ENSOP2;
  }

  tcpc_write(TCPC_REG_CONTROL1, reg);

}

void FUSB302::rx_fifo_flush() {

  uint8_t reg = tcpc_read(TCPC_REG_CONTROL1);

  reg |= TCPC_REG_CONTROL1_RX_FLUSH;

  tcpc_write(TCPC_REG_CONTROL1, reg);

}

//Control3
void FUSB302::send_hardreset() {

  uint8_t reg = tcpc_read(TCPC_REG_CONTROL3);

  tcpc_write(TCPC_REG_CONTROL3, reg);

}

void FUSB302::set_automode(bool retry, uint8_t nretry, bool hrst, bool srst) {

  uint8_t reg = tcpc_read(TCPC_REG_CONTROL3);

  if (retry)
    reg |= TCPC_REG_CONTROL3_AUTO_RETRY;
  else
    reg &= ~TCPC_REG_CONTROL3_AUTO_RETRY;

  if (hrst)
    reg |= TCPC_REG_CONTROL3_AUTO_HARDRESET;
  else
    reg &= ~TCPC_REG_CONTROL3_AUTO_HARDRESET;

  if (srst)
    reg |= TCPC_REG_CONTROL3_AUTO_SOFTRESET;
  else
    reg &= ~TCPC_REG_CONTROL3_AUTO_SOFTRESET;

  reg &= ~TCPC_REG_CONTROL3_N_RETRIES;
  reg |= (nretry & TCPC_REG_CONTROL3_N_RETRIES);

  tcpc_write(TCPC_REG_CONTROL3, reg);

}

//Status0
bool FUSB302::is_vbusok() { //Occurred when VBUS over 4.0V

  uint8_t reg = tcpc_read(TCPC_REG_STATUS0);

  return (reg & TCPC_REG_STATUS0_VBUSOK);

}

bool FUSB302::is_cc_busy() {

  uint8_t reg = tcpc_read(TCPC_REG_STATUS0);

  return (reg & TCPC_REG_STATUS0_ACTIVITY);
}

bool FUSB302::get_cc_comp() {

  uint8_t reg = tcpc_read(TCPC_REG_STATUS0);

  return (reg & TCPC_REG_STATUS0_COMP);
}

bool FUSB302::is_crc_correct() {

  uint8_t reg = tcpc_read(TCPC_REG_STATUS0);

  return (reg & TCPC_REG_STATUS0_CRC_CHK);
}

bool FUSB302::is_fifo_full() {

  uint8_t reg = tcpc_read(TCPC_REG_STATUS0);

  return (reg & TCPC_REG_STATUS0_ALERT);
}

bool FUSB302::is_device_present() {

  uint8_t reg = tcpc_read(TCPC_REG_STATUS0);

  return (reg & TCPC_REG_STATUS0_WAKE);
}

uint8_t FUSB302::get_bc_lvl() {

  uint8_t reg = tcpc_read(TCPC_REG_STATUS0);

  return (reg & TCPC_REG_STATUS0_BC_LVL);
}

//Status1
bool FUSB302::is_rx_empty() {

  uint8_t reg = tcpc_read(TCPC_REG_STATUS1);

  return (reg & TCPC_REG_STATUS1_RX_EMPTY);
}

//Mask,Interrupt
uint32_t FUSB302::get_interrupt_reason() {
  uint8_t res;
  uint32_t return_val;

  res = tcpc_read(TCPC_REG_INTERRUPTB);
  return_val = ((res & 0xFFl) << 16);

  res = tcpc_read(TCPC_REG_INTERRUPTA);
  return_val |= ((res & 0xFF) << 8);

  res = tcpc_read(TCPC_REG_INTERRUPT);
  return_val |= (res & 0xFF);

  return return_val;
}

void FUSB302::set_interrupt_mask(uint32_t mask) {

  tcpc_write(TCPC_REG_MASKB, !(mask & 0x10000));
  tcpc_write(TCPC_REG_MASKA, ~((mask >> 8) & 0xFF));
  tcpc_write(TCPC_REG_MASK, ~(mask & 0xFF));

}

void USB302_Read_FIFO(uint8_t *data, uint8_t length)
{
  Wire.beginTransmission(FUSB302_I2C_SLAVE_ADDR);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(FUSB302_I2C_SLAVE_ADDR, length, true);
  int cnt = 0;
  while (Wire.available())   // slave may send less than requested
  {
    data[cnt++] = Wire.read();    // receive a byte as character
  }
}

//FIFO
uint8_t FUSB302::get_message(uint16_t &head, uint32_t* payload) {

  uint8_t token = tcpc_read(TCPC_REG_FIFOS) & 0xe0;

  if (token > 0x40) {

    USB302_Read_FIFO((uint8_t*)&head, 2);

    uint8_t len = PD_HEADER_CNT(head);

    USB302_Read_FIFO((uint8_t*)payload, len * 4);

  } 

  rx_fifo_flush();
  return token;

}

void send_message(uint16_t &head, uint32_t* payload, uint8_t sop) {

  uint8_t send_buf[49];
  uint8_t cnt = 0;
  send_buf[cnt++] = TCPC_REG_FIFOS;

  switch (sop) {
    case 0: //SOP
      send_buf[cnt++] = FUSB302_TKN_SYNC1;
      send_buf[cnt++] = FUSB302_TKN_SYNC1;
      send_buf[cnt++] = FUSB302_TKN_SYNC1;
      send_buf[cnt++] = FUSB302_TKN_SYNC2;
      break;
    case 1: //SOP'
      send_buf[cnt++] = FUSB302_TKN_SYNC1;
      send_buf[cnt++] = FUSB302_TKN_SYNC1;
      send_buf[cnt++] = FUSB302_TKN_SYNC3;
      send_buf[cnt++] = FUSB302_TKN_SYNC3;
      break;
    case 2: //SOP''
      send_buf[cnt++] = FUSB302_TKN_SYNC1;
      send_buf[cnt++] = FUSB302_TKN_SYNC3;
      send_buf[cnt++] = FUSB302_TKN_SYNC1;
      send_buf[cnt++] = FUSB302_TKN_SYNC3;
      break;
    default:
      break;
  }
  uint8_t total_len = PD_HEADER_CNT(head) * 4 + 2;
  send_buf[cnt++] = FUSB302_TKN_PACKSYM | total_len;

  memcpy(&send_buf[cnt], payload, total_len - 2);

  cnt += total_len - 2;

  send_buf[cnt++] = FUSB302_TKN_JAMCRC;
  send_buf[cnt++] = FUSB302_TKN_EOP;
  send_buf[cnt++] = FUSB302_TKN_TXON;

  Wire.writeTransmission(FUSB302_I2C_SLAVE_ADDR, send_buf, cnt, true);

}
