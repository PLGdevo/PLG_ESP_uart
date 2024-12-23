// #include <PLG_uart.h>

// #define PLG_DHT11

// #ifdef PLG_DHT11
// #use fast_io(D)
// #define DHT22 PIN_D6
// int16 tran = 0;
// char message3[] = "DHT22 Temp = 00.0 C ";
// char message4[] = "DHT22 RH   = 00.0 % ";
// short Time_out;
// unsigned int8 T_byte1, T_byte2, RH_byte1, RH_byte2, CheckSum;
// unsigned int16 Temp, RH;

// void dht22_start_signal()
// {
//     output_drive(DHT22); // Configure connection pin as output
//     output_low(DHT22);   // Connection pin output low
//     delay_ms(25);
//     output_high(DHT22); // Connection pin output high
//     delay_us(30);
//     output_float(DHT22); // Configure connection pin as input
// }
// short dht22_check_response()
// {
//     delay_us(40);
//     if (!input(DHT22))
//     { // Read and test if connection pin is low
//         delay_us(80);
//         if (input(DHT22))
//         { // Read and test if connection pin is high
//             delay_us(50);
//             return 1;
//         }
//     }
// }
// unsigned int8 dht22_Read_Data()
// {
//     unsigned int8 i, k, _data = 0; // k is used to count 1 bit reading duration
//     if (Time_out)
//         break;
//     for (i = 0; i < 8; i++)
//     {
//         k = 0;
//         while (!input_state(dht22))
//         { // Wait for DHT22 pin to go high
//             k++;
//             if (k > 100)
//             {
//                 Time_out = 1;
//                 break;
//             }
//             delay_us(1);
//         }
//         delay_us(30);
//         if (!input_state(dht22))
//             bit_clear(_data, (7 - i)); // Clear bit (7 - i)
//         else
//         {
//             bit_set(_data, (7 - i)); // Set bit (7 - i)
//             while (input_state(dht22))
//             { // Wait for DHT22 pin to go low
//                 k++;
//                 if (k > 100)
//                 {
//                     Time_out = 1;
//                     break;
//                 }
//                 delay_us(1);
//             }
//         }
//     }
//     return _data;
// }
// #INT_TIMER0
// void run_dht22()
// {
//     tran++;
//     set_timer0(6);
//     if (tran > 10000)
//     {
//         Time_out = 0;
//         dht22_Start_signal();
//         if (dht22_check_response())
//         {                                 // If there is a response from sensor
//             RH_byte1 = dht22_Read_Data(); // read RH byte1
//             RH_byte2 = dht22_Read_Data(); // read RH byte2
//             T_byte1 = dht22_Read_Data();  // read T byte1
//             T_byte2 = dht22_Read_Data();  // read T byte2
//             Checksum = dht22_Read_Data(); // read checksum
//             if (Time_out)
//             { // If reading takes long time
//                 printf("%s \n", "  DHT22 Time out!   ");
//             }
//             else
//             {
//                 if (CheckSum == ((RH_Byte1 + RH_Byte2 + T_Byte1 + T_Byte2) & 0xFF))
//                 {
//                     RH = RH_byte1;
//                     RH = (RH << 8) | RH_byte2;
//                     Temp = T_byte1;
//                     Temp = (Temp << 8) | T_byte2;
//                     if (Temp > 0X8000)
//                     {
//                         message3[12] = '-';
//                         Temp = Temp & 0X7FFF;
//                     }
//                     else
//                         message3[12] = ' ';
//                     message3[13] = (Temp / 100) % 10 + 48;
//                     message3[14] = (Temp / 10) % 10 + 48;
//                     message3[16] = Temp % 10 + 48;
//                     message4[13] = (RH / 100) % 10 + 48;
//                     message4[14] = (RH / 10) % 10 + 48;
//                     message4[16] = RH % 10 + 48;
//                     message3[17] = 223;        // Degree symbol
//                     printf("%s \n", message3); // gia tri nhiet do
//                     printf("%s \n", message4); // gia tri o am
//                 }
//                 else
//                 {

//                     printf("%s \n", "DHT22 Checksum Error");
//                 }
//             }
//         }
//         tran = 0;
//     }
// }
// #else

// #endif
// void serial()
// {
//     if (input(IN1) == 0)
//     {
//         printf("%s \n", "CH");
//         delay_ms(200);
//         break;
//     }
// }
// void plg()
// {
//     if (ds[0] == 'f')
//     {
//         OUTPUT_HIGH(OUT1);
//     }
//     if (ds[0] == 'd')
//     {
//         OUTPUT_low(OUT1);
//     }
//     if (ds[0] == 'R' && ds[1] == '2' && ds[3] == '1')
//     {
//         OUTPUT_HIGH(OUT2);
//         //  OUTPUT_HIGH(BUZZER);
//     }
//     if (ds[0] == 'R' && ds[1] == '2' && ds[3] == '0')
//     {
//         OUTPUT_LOW(OUT2);
//         //  OUTPUT_LOW(BUZZER);
//     }
// }
// void uart()
// {
//     if (kbhit()) //      kbhit hàm nhan du lieu
//     {
//         char rx = getc(); // phan tu thu 0 trong rx bang gia tri doc dc tu uart
//         if (rx == '\n')
//         {
//             ds[c1] = '\0';
//             plg();
//             c1 = 0;
//         }
//         else //(rx[0] != '\n')
//         {
//             ds[c1] = rx;
//             c1++;
//         }
//     } // END KBHIT
// }

// void main()
// {

//     set_tris_d(0b00111000);
//     INPUT(IN1);
//     INPUT(IN2);
//     INPUT(IN3);
//     OUTPUT_LOW(BUZZER);
//     OUTPUT_LOW(OUT1);
//     OUTPUT_LOW(OUT2);
//     OUTPUT_LOW(OUT3);
//     delay_ms(200);
//     printf("%s \n", "reset_1");
// #ifdef PLG_DHT11
//     setup_timer_0(RTCC_INTERNAL | RTCC_DIV_8); // cai timer0 chay dinh thoi, bo chia 1/4
//     set_timer0(6);
//     enable_interrupts(INT_TIMER0); // cai dat ngat tran timer0
//     enable_interrupts(GLOBAL);     // bat buoc
// #endif
//     while (TRUE)
//     {
//         uart();
//         serial();
//         // run_dht22();

//     } // end while
// }
