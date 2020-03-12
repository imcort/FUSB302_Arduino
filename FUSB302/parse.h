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
  if (num)
    ret += (String)" DataMsg Len=" + num;
  else
    ret += " CtrlMsg";

  ret += (String)" Type:" + PD_HEADER_ID(header)+ ",";
    
  num = PD_HEADER_ID(header);
  ret += (String)"MsgID=" + num + ",";

  return ret;

}
