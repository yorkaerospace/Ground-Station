#include <LoRa.h>
#include <SPI.h>
#include <sys/types.h>

#define ss 5
#define rst 14
#define dio0 2
#define LED 12

struct Packet {
  uint32_t seq_no;
  uint32_t time_ms;
  double gps_lat, gps_lng, gps_alt;
  uint32_t gps_sat, gps_tme;
  double bmp_tmp, bmp_prs, bmp_alt;
};

unsigned char *recv = (unsigned char *)malloc(sizeof(Packet));
Packet *data;

void onReceive(int packetSize) {
  // read packet
  for (int i = 0; i < packetSize; i++) {
    recv[i] = LoRa.read();
  }

  data = (Packet *)recv;

  // Serial.print(data->seq_no);
  // Serial.print(" | Altitude:");
  // Serial.print(data->bmp_alt);
  // Serial.print(" Temp:");
  // Serial.print(data->bmp_temp);

  Serial.print("Lat:");
  Serial.print(data->gps_lat);
  Serial.print(" Lng:");
  Serial.print(data->gps_lng);
  Serial.print(" Alt:");
  Serial.print(data->gps_alt);
  Serial.print(" Sat:");
  Serial.print(data->gps_sat);
  Serial.print(" Tme:");
  Serial.println(data->gps_tme);

  Serial.print("Tmp:");
  Serial.print(data->bmp_tmp);
  Serial.print(" Prs:");
  Serial.print(data->bmp_prs);
  Serial.print(" Alt:");
  Serial.println(data->bmp_alt);
  

  // print RSSI of packet
  Serial.print("RSSI: ");
  Serial.print(LoRa.packetRssi());
  Serial.print(" SNR: ");
  Serial.println(LoRa.packetSnr());
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println("LoRa Receiver Callback");
  Serial.println(sizeof(Packet));

  // setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }

  // Uncomment the next line to disable the default AGC and set LNA gain, values
  // between 1 - 6 are supported
  // LoRa.setGain(6);
  // // Sync word to avoid confusing other transceivers
  // LoRa.setSyncWord(0xD3);
  // // Set a relatively wide 250kHz bandwidth
  // LoRa.setSignalBandwidth(250E3);

  Serial.println("LoRa Initializing OK!");

  // register the receive callback
  LoRa.onReceive(onReceive);

  // put the radio into receive mode
  LoRa.receive();
}

void loop() {
  // do nothing
}