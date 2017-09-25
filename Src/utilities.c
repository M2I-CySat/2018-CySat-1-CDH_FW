

int ByteToInt(uint8_t[] buf, uint32_t *flag)
{
  if (sizeof(buf) != 4){
    return 1;
  }

  flag = buf[0] | (buf[1] << 8) |
    (buf[2] << 16) | (buf[3] << 24);
  return 0;
}
int IntToByte(uint8_t[] *buf, uint32_t flag)
{
  if (sizeof(buf) != 4){
    return 1;
  }
  buf[0] = flag & 0xff;
	buf[1] = (flag >> 8) & 0xff;
	buf[2] = (flag >> 16) & 0xff;
	buf[3] = (flag >> 24) & 0xff;
  return 0;
}
