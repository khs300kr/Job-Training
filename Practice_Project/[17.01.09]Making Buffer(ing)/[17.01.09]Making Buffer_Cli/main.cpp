

void main()
{
	CCircularBuf send_buf;
	TempPacket send_packet;

	send_packet.m_ID = StringData;
	send_packet.m_size = sizeof(send_packet);
	send_packet.str_temp = "����";

	cout << send_buf.WriteBuf(&send_packet, sizeof(send_packet)) << endl;

	CCircularBuf recv_buf;
	TempPacket recv_packet;
	// Buf�� �޴´�.
	recv_buf = send_buf;

	// Buf�� �ִ� �����͸� recv_packet�� ����.
	recv_buf.ReadBuf(&recv_packet, sizeof(recv_packet));

	cout << recv_packet.str_temp << endl;


}