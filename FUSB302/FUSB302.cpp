#include "FUSB302.h"

FUSB302::FUSB302()
{
  Wire.begin(16, 17);
  Wire.setClock(1000000);
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

  delayMicroseconds(10);

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

  if (spec_rev & 1)
    reg |= TCPC_REG_SWITCHES1_SPECREV0;
  else
    reg &= ~TCPC_REG_SWITCHES1_SPECREV0;

  if (spec_rev & 2)
    reg |= TCPC_REG_SWITCHES1_SPECREV1;
  else
    reg &= ~TCPC_REG_SWITCHES1_SPECREV1;

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

}

//MEASURE
void FUSB302::vbus_meas_en(bool en) {

  if (en)
    tcpc_write(TCPC_REG_MEASURE, TCPC_REG_MEASURE_VBUS);
  else
    tcpc_write(TCPC_REG_MEASURE, 0);

  delayMicroseconds(10);

}

void FUSB302::mdac_set(uint8_t val) {

  uint8_t reg = tcpc_read(TCPC_REG_MEASURE);

  tcpc_write(TCPC_REG_MEASURE, (reg & 0xC0) | (val & 0x3F));

  delayMicroseconds(10);

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

void FUSB302::auto_pre(bool en) {

  uint8_t reg = tcpc_read(TCPC_REG_CONTROL0);

  if (en)
    reg |= TCPC_REG_CONTROL0_AUTO_PRE;
  else
    reg &= ~TCPC_REG_CONTROL0_AUTO_PRE;

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

ufp_cc_status FUSB302::get_ufp_cc_status(uint8_t cc_line) { //cc_line = 1 or 2

  uint8_t cc_lvl;
  bool comp;

  if (cc_line == 1)
    cc_meas_switch(true, false);
  else if (cc_line == 2)
    cc_meas_switch(false, true);
  else
    return UFP_CC_STATUS_ERROR;

  cc_lvl = get_bc_lvl();

  Serial.print("CC Line ");
  Serial.print(cc_line);

  switch (cc_lvl) {
    case 0:
      Serial.println(" Status: UFP_CC_STATUS_NOT_CONN");
      return UFP_CC_STATUS_NOT_CONN;
    case 1:
      Serial.println(" Status: UFP_CC_STATUS_vRd_USB");
      return UFP_CC_STATUS_vRd_USB;
    case 2:
      Serial.println(" Status: UFP_CC_STATUS_vRd_1_5");
      return UFP_CC_STATUS_vRd_1_5;
    case 3:
      comp = get_cc_comp();
      if (!comp) {
        Serial.println(" Status: UFP_CC_STATUS_vRd_3_0");
        return UFP_CC_STATUS_vRd_3_0;
      }

    default:
      return UFP_CC_STATUS_ERROR;

  }

}

ufp_cc_status FUSB302::ufp_auto_polarity() {

  if (is_vbusok()) {
    Serial.println("UFP Attached.");

    if (!is_cc_configured) {
      ufp_cc_status cc1_status = get_ufp_cc_status(1);
      ufp_cc_status cc2_status = get_ufp_cc_status(2);

      if (cc1_status == UFP_CC_STATUS_ERROR || cc1_status == UFP_CC_STATUS_ERROR)
        return UFP_CC_STATUS_ERROR;

      if (cc1_status > cc2_status) {
        cc_bmc_en(true, false);
        cc_meas_switch(true, false);

        Serial.println("CC1 BMC EN");
        tcpc_write(TCPC_REG_RESET, TCPC_REG_RESET_PD_RESET);
        return cc1_status;

      } else if (cc2_status > cc1_status) {
        cc_bmc_en(false, true);
        cc_meas_switch(false, true);

        Serial.println("CC2 BMC EN");
        tcpc_write(TCPC_REG_RESET, TCPC_REG_RESET_PD_RESET);
        return cc2_status;

      }
      is_cc_configured = true;
    }


  } else {
    Serial.println("Not Attached. ");
    cc_bmc_en(false, false);
    is_cc_configured = false;
    return UFP_CC_STATUS_NOT_CONN;

  }
}

void USB302_Read_FIFO(uint8_t *data, uint8_t len)
{
  Wire.beginTransmission(FUSB302_I2C_SLAVE_ADDR);
  Wire.write(TCPC_REG_FIFOS);
  Wire.endTransmission(false);
  Wire.requestFrom(FUSB302_I2C_SLAVE_ADDR, len, true);
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

void FUSB302::send_message(uint16_t head, uint32_t* payload, uint8_t sop) {

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

  send_buf[cnt++] = head & 0xff;
  send_buf[cnt++] = (head >> 8) & 0xff;

  memcpy(&send_buf[cnt], payload, total_len - 2);

  cnt += total_len - 2;

  send_buf[cnt++] = FUSB302_TKN_JAMCRC;
  send_buf[cnt++] = FUSB302_TKN_EOP;
  send_buf[cnt++] = FUSB302_TKN_TXON;
  tcpc_write(TCPC_REG_CONTROL0, 0x40);
  Wire.beginTransmission(FUSB302_I2C_SLAVE_ADDR);


  for (int i = 0; i < (total_len + 9); i++) {
    Wire.write(send_buf[i]);
    Serial.println(send_buf[i], HEX);
  }

  Wire.endTransmission();
  tcpc_write(TCPC_REG_CONTROL0, 0x05);

}
