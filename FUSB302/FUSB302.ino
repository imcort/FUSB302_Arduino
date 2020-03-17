#include "FUSB302.h"
#include "parse.h"

FUSB302 usbc;
bool is_cc_configured = false;

enum ufp_cc_status {
  UFP_CC_STATUS_NOT_CONN = 0,
  UFP_CC_STATUS_vRd_USB = 1,
  UFP_CC_STATUS_vRd_1_5 = 2,
  UFP_CC_STATUS_vRd_3_0 = 3,
  UFP_CC_STATUS_ERROR = 4,
};

ufp_cc_status get_ufp_cc_status(uint8_t cc_line) { //cc_line = 1 or 2

  uint8_t cc_lvl;
  bool comp;

  if (cc_line == 1)
    usbc.cc_meas_switch(true, false);
  else if (cc_line == 2)
    usbc.cc_meas_switch(false, true);
  else
    return UFP_CC_STATUS_ERROR;

  cc_lvl = usbc.get_bc_lvl();

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
      comp = usbc.get_cc_comp();
      if (!comp) {
        Serial.println(" Status: UFP_CC_STATUS_vRd_3_0");
        return UFP_CC_STATUS_vRd_3_0;
      }

    default:
      return UFP_CC_STATUS_ERROR;

  }

}

ufp_cc_status ufp_auto_polarity() {

  if (usbc.is_vbusok()) {
    Serial.println("UFP Attached.");

    if (!is_cc_configured) {
      ufp_cc_status cc1_status = get_ufp_cc_status(1);
      ufp_cc_status cc2_status = get_ufp_cc_status(2);

      if (cc1_status == UFP_CC_STATUS_ERROR || cc1_status == UFP_CC_STATUS_ERROR)
        return UFP_CC_STATUS_ERROR;

      if (cc1_status > cc2_status) {
        usbc.cc_bmc_en(true, false);
        usbc.cc_meas_switch(true, false);

        Serial.println("CC1 BMC EN");
        usbc.tcpc_write(TCPC_REG_RESET, TCPC_REG_RESET_PD_RESET);
        return cc1_status;

      } else if (cc2_status > cc1_status) {
        usbc.cc_bmc_en(false, true);
        usbc.cc_meas_switch(false, true);

        Serial.println("CC2 BMC EN");
        usbc.tcpc_write(TCPC_REG_RESET, TCPC_REG_RESET_PD_RESET);
        return cc2_status;

      }
      is_cc_configured = true;
    }


  } else {
    Serial.println("Not Attached. ");
    usbc.cc_bmc_en(false, false);
    is_cc_configured = false;
    return UFP_CC_STATUS_NOT_CONN;

  }
}

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

      //      for (int i = 0; i < PD_HEADER_CNT(usb_pd_message_header); i++) {
      //        Serial.print(i, HEX);
      //        Serial.print(" = 0x");
      //        Serial.println(usb_pd_message_buffer[i], BIN);
      //      }

    }
  }

}

void gcrcsend() {
  uint16_t msg = 0x21;
  usbc.send_message(msg, NULL);
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
    ufp_auto_polarity();
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

  ufp_auto_polarity();

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
