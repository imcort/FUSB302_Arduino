String TOKEN_PARSE(uint8_t token) {

  switch ((token >> 5) & 0x7) {
    case 7:
      return (String)"SOP";
    case 6:
      return (String)"SOP1";
    case 5:
      return (String)"SOP2";
    case 4:
      return (String)"SOP1DB";
    case 3:
      return (String)"SOP2DB";
    default:
      return (String)"ERROR";
  }
}

String HEADER_PARSE(uint16_t header) {

  String ret = "";

  if (header & 0x8000)
    ret += "Extended,v";

  ret += (String)"Spec:" + (int)PD_HEADER_SPEC(header);
  ret += (String)" PortRole:" + (int)PD_HEADER_PORTROLE(header);
  ret += (String)" DataRole:" + (int)PD_HEADER_DATAROLE(header);

  uint8_t num = PD_HEADER_CNT(header);
  if (num) {

    ret += (String)" DataMsg Len=" + num + " ";
    switch (PD_HEADER_TYPE(header)) {
      case 1:
        ret += (String)"Source_Capabilities";
        break;
      case 2:
        ret += (String)"Request";
        break;
      case 3:
        ret += (String)"BIST";
        break;
      case 4:
        ret += (String)"Sink_Capabilities";
        break;
      case 5:
        ret += (String)"Battery_Status";
        break;
      case 6:
        ret += (String)"Alert";
        break;
      case 7:
        ret += (String)"Get_Contry_Info";
        break;
      case 15:
        ret += (String)"Get_Contry_Info";
        break;
      default:
        ret += (String)"Reserved" + (int)PD_HEADER_TYPE(header);
        break;
    }
  }

  else {
    ret += " CtrlMsg ";
    switch (PD_HEADER_TYPE(header)) {
      case 1:
        ret += (String)"GoodCRC";
        break;
      case 2:
        ret += (String)"GotoMin";
        break;
      case 3:
        ret += (String)"Accept";
        break;
      case 4:
        ret += (String)"Reject";
        break;
      case 5:
        ret += (String)"Ping";
        break;
      case 6:
        ret += (String)"PS_RDY";
        break;
      case 7:
        ret += (String)"Get_Source_Cap";
        break;
      case 8:
        ret += (String)"Get_Sink_Cap";
        break;
      default:
        ret += (String)"Reserved" + (int)PD_HEADER_TYPE(header);
        break;
    }
  }


  num = PD_HEADER_ID(header);
  ret += (String)" MsgID=" + num;

  return ret;

}
