typedef pd_msg_header {

  bool Extended;
  uint8_t DataNum;
  uint8_t MsgID;
  bool PowerRole;
  uint8_t Spec;
  bool DataRole;
  uint8_t MsgType;

};

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
      return (String)"ERROR" + token;
  }
}

String Source_Capabilities_PARSE(uint32_t source) {

  String ret = "";
  uint8_t num = (source >> 30) & 0x3;
  float cachevolmax, cachevolmin, cachecur;
  switch (num) {
    case 0:
      cachevolmax = ((source >> 10) & 0x3FF) * 0.05f;
      cachecur = (source & 0x3FF) * 0.01f;
      ret += (String)"Fixed, Voltage:" + cachevolmax + ", Current:" + cachecur;
      break;
    case 1:
      ret += (String)"Battery,";
      break;
    case 2:
      cachevolmax = ((source >> 20) & 0x3FF) * 0.05f;
      cachevolmin = ((source >> 10) & 0x3FF) * 0.05f;
      cachecur = (source & 0x3FF) * 0.01f;
      ret += (String)"Variable, Voltage:" + cachevolmax + " - " + cachevolmin + ", Current:" + cachecur;
      break;
    case 3:
      cachevolmax = ((source >> 17) & 0x7F) * 0.1f;
      cachevolmin = ((source >> 8) & 0x7F) * 0.1f;
      cachecur = (source & 0x7F) * 0.05f;
      ret += (String)"APDO, Voltage:" + cachevolmax + " - " + cachevolmin + ", Current:" + cachecur;
      break;
  }
  return ret;


}

String HEADER_PARSE(uint16_t header, uint32_t* buffer) {

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
        for (int i = 0; i < num; i++) {
          ret += Source_Capabilities_PARSE(buffer[i]);
          ret += "\r\n";
        }
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
