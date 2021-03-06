/* ������ ������       _
0 - FF                |  ���������
1 - FF                |_ ������ ��� �������������
2 - N + (code << 4)    N[0..9], code[0..10]
3 - data
4 - crc
*/

/* ������������������ ������
0 - ����� ���������
1 - ������������� ���� �������������, ������� ����� ���� ������ crc ��� ����� ������� 0
2 - ����� � ����� ������ � ���������
3 - ��� ������ � 2,3 ��������� crc8(), �������������� �������
4 - ���������� ����������� crc8 � ������ � 4
5 - ���� ���������� ������� ��������� � crc8 ������������, �� �������� ����� �������
6 - ������� ������ � ����� ����������� �� ����������, ������� ������ �������������� ����� ������� � ��
7 - �� ������ ��������� �������� �� �������� � ���������
*/

/* �������
0 - ������ ����� io1
1 - ������ ����� io1
2 - ������� ����� io1
3 - ������ ����� io3
4 - ������ ����� io3
5 - ������� ����� io3
6 - ������ �������
7 - ������� �������
8 - ��� ������, � ���� ������:
    0 - �����/���������
    1 - �����/crc
    2 - �����/������ � ������������ ���������
9 - ��� �������������, � ���� ������:
    0 - ������
    1 - �����
10 - ���������� ������� �������� (��������� ������ ��):
    0 - ������, ������� ������ �������������� ����� ������� � ��
    1 - �����
*/

/*
---��������
����_�����                    1 ���� ��������� �������
����_������                   1 ���� ��� ��������� �� ����������

���������_crc
    _0            0           ��������� ���� � 0
    _1            1           ��������� ���� � 1
    _2            2           crc8 ���������
    _OK           3           ������� ���������
    _ERR          4           ������
    
���������_code
    _0            0           ������ ������
    
    _STOP                     ���������� �����������


���������� �� ������{
    if (rx_n == 3) rx_n = 0; rx_pack_n++;
    if (rx_pack_n == 10) rx_pack_n = 0;
    ������ � ����������� ����� 3 ����
    rx_n++;
}

main{
    ���� (��������� && ����_����� == 0), ����_����� = 1; rx_n = 0; crc_state = 0;
    ���� (rx_pack_n != rx_pack_n_prev) crc_state = 0; rx_pack_n_prev = rx_pack_n;
    ���� (����_����� == 1) {
        if (crc_state == 0 && rx_n > 0) crc8(������ ����); crc_state++;
        if (crc_state == 1 && rx_n > 1) crc8(������ ����); crc_state++;
        if (crc_state == 2 && rx_n > 2)
            if (crc8 == ������ ���� && �������� �������) crc_state = OK; code_f = 1;
            else ����_����� = 0;
        if (code_f == 1 && code_state == STOP) �������� � code_buf; code_state = 0; code_ready_f = 0;
    }
    switch (code_state){
        case 0: ������ ����������
        break;
        case STOP:
        break;
    }
}
