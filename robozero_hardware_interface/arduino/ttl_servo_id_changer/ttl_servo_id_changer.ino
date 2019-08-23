#include "stdio.h"
// ID A を Bに変更
byte IDAtoB[] = {0xFA, 0xAF, 0x0A, 0x00, 0x04, 0x01, 0x01, 0x0B, 0x05};
// ROM Write
byte RomWrite[] = {0xFA, 0xAF, 0x0A, 0x40, 0xFF, 0x00, 0x00, 0xBE};
// Servo Restart
byte ServoreStart[] = {0xFA, 0xAF, 0x0A, 0x20, 0xFF, 0x00, 0x00, 0xDE};

void servo_write(byte packet[],int size){
    byte xor_buff = packet[2];
    int cnt = 3;
    while(cnt < size-1){
        xor_buff = xor_buff ^ packet[cnt];
        cnt++;
    }
    packet[size-1] = xor_buff;
    Serial1.write(packet,size);
}

void setup() {
    Serial.begin(9600);
    Serial.setTimeout(2000UL); // タイムアウト設定（1秒）
    Serial1.begin(115200);
    
}

void loop() {
  // put your main code here, to run repeatedly:
    // analogWrite(3,50);
    String before_id,after_id;
    byte xor_buff=0;
    int cnt=3;
    Serial.println("現在のサーボアドレスを入力");
    if (Serial.available() > 0) { // 受信したデータが存在する
        before_id = Serial.readStringUntil('\r');
        Serial.println("変更したいサーボアドレスを入力");
        after_id = Serial.readStringUntil('\r');
        Serial.print("現在のサーボアドレス,");
        Serial.print(before_id);
        Serial.print("変更したいサーボアドレス,");
        Serial.print(after_id);
        Serial.println("でおｋ？(y/n)");
        if(Serial.readStringUntil('\r') == "y"){
            IDAtoB[2] = atoi(before_id.c_str());
            IDAtoB[7] = atoi(after_id.c_str());

            RomWrite[2] = atoi(after_id.c_str());
            ServoreStart[2] = atoi(after_id.c_str());
            
            //check_sumの計算と書き込み
            servo_write(IDAtoB,9);
            delay(1000);
            servo_write(RomWrite,8);
            delay(2000);
            servo_write(ServoreStart,8);
            delay(1000);
            Serial.println("書き換え終了");
        }
    }else{
        while(Serial.available() == 0);
    }
}
