#ifndef FUSB302_H
#define FUSB302_H

#include "Arduino.h"
#include <Wire.h>

#define PD_HEADER_CNT(header)  (((header) >> 12) & 7)
#define PD_HEADER_TYPE(header) ((header) & 0xF)
#define PD_HEADER_ID(header)   (((header) >> 9) & 7)

/*Registers*/
/* Chip Device ID - 302A or 302B */
#define FUSB302_DEVID_302A 0x08
#define FUSB302_DEVID_302B 0x09

/* I2C slave address varies by part number */
/* FUSB302BUCX / FUSB302BMPX */
#define FUSB302_I2C_SLAVE_ADDR 0x22 // 7-bit address for Arduino
/* FUSB302B01MPX */
#define FUSB302_I2C_SLAVE_ADDR_B01 0x23
/* FUSB302B10MPX */
#define FUSB302_I2C_SLAVE_ADDR_B10 0x24
/* FUSB302B11MPX */
#define FUSB302_I2C_SLAVE_ADDR_B11 0x25

/* Default retry count for transmitting */
#define PD_RETRY_COUNT      3

/* Time to wait for TCPC to complete transmit */
#define PD_T_TCPC_TX_TIMEOUT  (100*MSEC)

#define TCPC_REG_DEVICE_ID  0x01

#define TCPC_REG_SWITCHES0  0x02
#define TCPC_REG_SWITCHES0_CC2_PU_EN    (1<<7)
#define TCPC_REG_SWITCHES0_CC1_PU_EN    (1<<6)
#define TCPC_REG_SWITCHES0_VCONN_CC2    (1<<5)
#define TCPC_REG_SWITCHES0_VCONN_CC1    (1<<4)
#define TCPC_REG_SWITCHES0_MEAS_CC2 (1<<3)
#define TCPC_REG_SWITCHES0_MEAS_CC1 (1<<2)
#define TCPC_REG_SWITCHES0_CC2_PD_EN    (1<<1)
#define TCPC_REG_SWITCHES0_CC1_PD_EN    (1<<0)

#define TCPC_REG_SWITCHES1  0x03
#define TCPC_REG_SWITCHES1_POWERROLE    (1<<7)
#define TCPC_REG_SWITCHES1_SPECREV1 (1<<6)
#define TCPC_REG_SWITCHES1_SPECREV0 (1<<5)
#define TCPC_REG_SWITCHES1_DATAROLE (1<<4)
#define TCPC_REG_SWITCHES1_AUTO_GCRC    (1<<2)
#define TCPC_REG_SWITCHES1_TXCC2_EN (1<<1)
#define TCPC_REG_SWITCHES1_TXCC1_EN (1<<0)

#define TCPC_REG_MEASURE    0x04
#define TCPC_REG_MEASURE_VBUS       (1<<6)
#define TCPC_REG_MEASURE_MDAC_MV(mv)    (((mv)/42) & 0x3f)

#define TCPC_REG_CONTROL0   0x06
#define TCPC_REG_CONTROL0_TX_FLUSH  (1<<6)
#define TCPC_REG_CONTROL0_INT_MASK  (1<<5)
#define TCPC_REG_CONTROL0_HOST_CUR_MASK (3<<2)
#define TCPC_REG_CONTROL0_HOST_CUR_3A0  (3<<2)
#define TCPC_REG_CONTROL0_HOST_CUR_1A5  (2<<2)
#define TCPC_REG_CONTROL0_HOST_CUR_USB  (1<<2)
#define TCPC_REG_CONTROL0_TX_START  (1<<0)

#define TCPC_REG_CONTROL1   0x07
#define TCPC_REG_CONTROL1_ENSOP2DB  (1<<6)
#define TCPC_REG_CONTROL1_ENSOP1DB  (1<<5)
#define TCPC_REG_CONTROL1_BIST_MODE2    (1<<4)
#define TCPC_REG_CONTROL1_RX_FLUSH  (1<<2)
#define TCPC_REG_CONTROL1_ENSOP2    (1<<1)
#define TCPC_REG_CONTROL1_ENSOP1    (1<<0)

#define TCPC_REG_CONTROL2   0x08
/* two-bit field, valid values below */
#define TCPC_REG_CONTROL2_MODE      (1<<1)
#define TCPC_REG_CONTROL2_MODE_DFP  (0x3)
#define TCPC_REG_CONTROL2_MODE_UFP  (0x2)
#define TCPC_REG_CONTROL2_MODE_DRP  (0x1)
#define TCPC_REG_CONTROL2_MODE_POS  (1)
#define TCPC_REG_CONTROL2_TOGGLE    (1<<0)

#define TCPC_REG_CONTROL3   0x09
#define TCPC_REG_CONTROL3_SEND_HARDRESET    (1<<6)
#define TCPC_REG_CONTROL3_BIST_TMODE        (1<<5) /* 302B Only */
#define TCPC_REG_CONTROL3_AUTO_HARDRESET    (1<<4)
#define TCPC_REG_CONTROL3_AUTO_SOFTRESET    (1<<3)
/* two-bit field */
#define TCPC_REG_CONTROL3_N_RETRIES         (3<<1)
#define TCPC_REG_CONTROL3_AUTO_RETRY        (1<<0)

#define TCPC_REG_MASK       0x0A

#define TCPC_REG_POWER      0x0B
#define TCPC_REG_POWER_PWR      (1<<0)  /* four-bit field */
#define TCPC_REG_POWER_PWR_LOW      0x1 /* Bandgap + Wake circuitry */
#define TCPC_REG_POWER_PWR_MEDIUM   0x3 /* LOW + Receiver + Current refs */
#define TCPC_REG_POWER_PWR_HIGH     0x7 /* MEDIUM + Measure block */
#define TCPC_REG_POWER_PWR_ALL      0xF /* HIGH + Internal Oscillator */

#define TCPC_REG_RESET      0x0C
#define TCPC_REG_RESET_PD_RESET     (1<<1)
#define TCPC_REG_RESET_SW_RESET     (1<<0)

#define TCPC_REG_MASKA      0x0E

#define TCPC_REG_MASKB      0x0F

#define TCPC_REG_STATUS0A   0x3C
#define TCPC_REG_STATUS0A_SOFTFAIL  (1<<5)
#define TCPC_REG_STATUS0A_RETRYFAIL (1<<4)
#define TCPC_REG_STATUS0A_POWER     (1<<2) /* two-bit field */
#define TCPC_REG_STATUS0A_RX_SOFT_RESET (1<<1)
#define TCPC_REG_STATUS0A_RX_HARD_RESEt (1<<0)

#define TCPC_REG_STATUS1A   0x3D
/* three-bit field, valid values below */
#define TCPC_REG_STATUS1A_TOGSS     (1<<3)
#define TCPC_REG_STATUS1A_TOGSS_RUNNING     0x0
#define TCPC_REG_STATUS1A_TOGSS_SRC1        0x1
#define TCPC_REG_STATUS1A_TOGSS_SRC2        0x2
#define TCPC_REG_STATUS1A_TOGSS_SNK1        0x5
#define TCPC_REG_STATUS1A_TOGSS_SNK2        0x6
#define TCPC_REG_STATUS1A_TOGSS_AA      0x7
#define TCPC_REG_STATUS1A_TOGSS_POS     (3)
#define TCPC_REG_STATUS1A_TOGSS_MASK        (0x7)

#define TCPC_REG_STATUS1A_RXSOP2DB  (1<<2)
#define TCPC_REG_STATUS1A_RXSOP1DB  (1<<1)
#define TCPC_REG_STATUS1A_RXSOP     (1<<0)

#define TCPC_REG_INTERRUPTA 0x3E

#define TCPC_REG_INTERRUPTB 0x3F

#define TCPC_REG_STATUS0    0x40
#define TCPC_REG_STATUS0_VBUSOK     (1<<7)
#define TCPC_REG_STATUS0_ACTIVITY   (1<<6)
#define TCPC_REG_STATUS0_COMP       (1<<5)
#define TCPC_REG_STATUS0_CRC_CHK    (1<<4)
#define TCPC_REG_STATUS0_ALERT      (1<<3)
#define TCPC_REG_STATUS0_WAKE       (1<<2)
#define TCPC_REG_STATUS0_BC_LVL     (3<<0) /* two-bit field */

#define TCPC_REG_STATUS1    0x41
#define TCPC_REG_STATUS1_RXSOP2     (1<<7)
#define TCPC_REG_STATUS1_RXSOP1     (1<<6)
#define TCPC_REG_STATUS1_RX_EMPTY   (1<<5)
#define TCPC_REG_STATUS1_RX_FULL    (1<<4)
#define TCPC_REG_STATUS1_TX_EMPTY   (1<<3)
#define TCPC_REG_STATUS1_TX_FULL    (1<<2)

#define TCPC_REG_INTERRUPT  0x42

#define TCPC_REG_FIFOS      0x43

enum fusb302_txfifo_tokens {
    FUSB302_TKN_TXON = 0xA1,
    FUSB302_TKN_SYNC1 = 0x12,
    FUSB302_TKN_SYNC2 = 0x13,
    FUSB302_TKN_SYNC3 = 0x1B,
    FUSB302_TKN_RST1 = 0x15,
    FUSB302_TKN_RST2 = 0x16,
    FUSB302_TKN_PACKSYM = 0x80,
    FUSB302_TKN_JAMCRC = 0xFF,
    FUSB302_TKN_EOP = 0x14,
    FUSB302_TKN_TXOFF = 0xFE,
};

enum fusb302_interrupt_reason {
    FUSB302_INT_BC_LVL       = (1<<0),
    FUSB302_INT_COLLISION    = (1<<1),
    FUSB302_INT_WAKE         = (1<<2),
    FUSB302_INT_ALERT        = (1<<3),
    FUSB302_INT_CRC_CHK      = (1<<4),
    FUSB302_INT_COMP_CHNG    = (1<<5),
    FUSB302_INT_ACTIVITY     = (1<<6),
    FUSB302_INT_VBUSOK       = (1<<7),
    
    FUSB302_INT_HARDRESET   = (1<<8),
    FUSB302_INT_SOFTRESET   = (1<<9),
    FUSB302_INT_TX_SUCCESS  = (1<<10),
    FUSB302_INT_HARDSENT    = (1<<11),
    FUSB302_INT_RETRYFAIL   = (1<<12),
    FUSB302_INT_SOFTFAIL    = (1<<13),
    FUSB302_INT_TOGDONE     = (1<<14),
    FUSB302_INT_OCP_TEMP    = (1<<15),
    
    FUSB302_INT_GCRCSENT    = (1<<16),
};

class FUSB302
{
  public:
    FUSB302();

    void reset();

    void tcpc_write(uint8_t reg, uint8_t val);
    uint8_t tcpc_read(uint8_t reg);

    void sop_prime_en(bool en);
    void send_hardreset();
    void set_automode(bool retry, uint8_t nretry, bool hrst = false, bool srst = false);

    void rx_fifo_flush();
    void tx_fifo_flush();
    
    uint8_t get_chip_id();
    void cc_pu_switch(bool cc1, bool cc2);
    void cc_vconn_switch(bool cc1, bool cc2);
    void cc_meas_switch(bool cc1, bool cc2);
    void cc_pd_switch(bool cc1, bool cc2);
    void autocrc_config(bool en, bool pwr_role = 0, uint8_t spec_rev = 1, bool datarole = 0);
    void cc_bmc_en(bool cc1, bool cc2);
    void vbus_meas_en(bool en);
    void mdac_set(uint8_t val);
    void cc_pu_current(uint8_t current);

    bool is_rx_empty();

    bool is_vbusok();
    bool is_cc_busy();
    bool get_cc_comp();
    bool is_crc_correct();
    bool is_fifo_full(); 
    bool is_device_present();
    uint8_t get_bc_lvl();
    uint8_t get_message(uint16_t &head, uint32_t* payload);
    void send_message(uint16_t &head, uint32_t* payload, uint8_t sop = 0);
    uint32_t get_interrupt_reason();
    void set_interrupt_mask(uint32_t mask);
    void int_en(bool en);
};

#endif
