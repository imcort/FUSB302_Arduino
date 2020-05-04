#include "FUSB302.h"
#include "parse.h"

FUSB302 usbc;

void msg_recv() {

  if (!usbc.is_rx_empty()) {

    Serial.print("RX");
    uint16_t  usb_pd_message_header;
    uint32_t  usb_pd_message_buffer[10];
    uint8_t token = usbc.get_message(usb_pd_message_header, usb_pd_message_buffer);

    if (token) {
      Serial.print("Token: ");
      Serial.print(TOKEN_PARSE(token));
      Serial.print(", Header: ");
      Serial.println(HEADER_PARSE(usb_pd_message_header, usb_pd_message_buffer));

    }
  }

}

void interrupt_handler() {
  uint32_t reason = usbc.get_interrupt_reason();
  Serial.println();
  if (reason & FUSB302_INT_BC_LVL) {
    //Serial.println("FUSB302_INT_BC_LVL");
  }
  if (reason & FUSB302_INT_COLLISION) {
    Serial.println("FUSB302_INT_COLLISION");
  }
  if (reason & FUSB302_INT_WAKE) {
    Serial.println("FUSB302_INT_WAKE");
  }
  if (reason & FUSB302_INT_ALERT) {
    Serial.println("FUSB302_INT_ALERT");
  }
  if (reason & FUSB302_INT_CRC_CHK) {
    Serial.println("FUSB302_INT_CRC_CHK");
    //
  }
  if (reason & FUSB302_INT_COMP_CHNG) {
    Serial.println("FUSB302_INT_COMP_CHNG");
  }
  if (reason & FUSB302_INT_ACTIVITY) {
    //Serial.println("FUSB302_INT_ACTIVITY");
  }
  if (reason & FUSB302_INT_VBUSOK) {
    Serial.println("FUSB302_INT_VBUSOK");
    usbc.ufp_auto_polarity();
  }
  if (reason & FUSB302_INT_HARDRESET) {
    Serial.println("FUSB302_INT_HARDRESET");
  }
  if (reason & FUSB302_INT_SOFTRESET) {
    Serial.println("FUSB302_INT_SOFTRESET");
  }
  if (reason & FUSB302_INT_TX_SUCCESS) {
    Serial.println("FUSB302_INT_TX_SUCCESS");
  }
  if (reason & FUSB302_INT_HARDSENT) {
    Serial.println("FUSB302_INT_HARDSENT");
  }
  if (reason & FUSB302_INT_RETRYFAIL) {
    Serial.println("FUSB302_INT_RETRYFAIL");
  }
  if (reason & FUSB302_INT_SOFTFAIL) {
    Serial.println("FUSB302_INT_SOFTFAIL");
  }
  if (reason & FUSB302_INT_TOGDONE) {
    Serial.println("FUSB302_INT_TOGDONE");
  }
  if (reason & FUSB302_INT_OCP_TEMP) {
    Serial.println("FUSB302_INT_OCP_TEMP");
  }
  if (reason & FUSB302_INT_GCRCSENT) {
    Serial.println("FUSB302_INT_GCRCSENT");

    msg_recv();
  }
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(500000);

  usbc.reset();
  Serial.println();
  Serial.println(usbc.get_chip_id(), HEX);
  pinMode(5, INPUT_PULLUP);
  pinMode(22, OUTPUT);

  //Configure UFP:
  usbc.cc_pu_switch(false, false);
  //usbc.cc_pu_current(3);
  usbc.cc_pd_switch(true, true);

  usbc.cc_vconn_switch(false, false);
  usbc.cc_meas_switch(false, false);
  usbc.vbus_meas_en(false);

  usbc.autocrc_config(true);
  usbc.cc_bmc_en(false, false);
  usbc.sop_prime_en(true);

  usbc.mdac_set(0x34); //2.05V

  usbc.get_interrupt_reason();//Clear INT
  //usbc.set_interrupt_mask(FUSB302_INT_CRC_CHK | FUSB302_INT_VBUSOK);
  usbc.int_en(true);
  usbc.get_interrupt_reason();//Clear INT

  usbc.tcpc_write(TCPC_REG_POWER, TCPC_REG_POWER_PWR_ALL);

  usbc.tcpc_write(TCPC_REG_RESET, TCPC_REG_RESET_PD_RESET);

  usbc.ufp_auto_polarity();

}

bool LEDSTATUS = 0;

void loop() {
  // put your main code here, to run repeatedly:





  if (!digitalRead(5)) {
    interrupt_handler();
    digitalWrite(22, LEDSTATUS);
    LEDSTATUS = !LEDSTATUS;
  }

  if (Serial.available()) {
    uint32_t msg = 0x03000000 | (1 << 28) | (0x12C << 10) | 0x12C;
    usbc.send_message(0x1442, &msg);
    Serial.flush();
  }




}
