#include <LoRa.h>
#include <SPI.h>
#include <sys/types.h>

#define ss 17
#define rst 15
#define dio0 20
#define LED 25

#pragma pack(1)
struct packet {
    // Packet information
    uint32_t seq_no;
    uint8_t vid;         // Vehicle ID

    // Status
    char     state;       // Current state
    uint32_t time_ms;     // ms since boot

    // GPS data
    uint8_t  time_utc[3]; // Seconds since midnight
    int32_t  lat, lng;    // Position
    uint8_t  sat;         // Satellites

    // Baro data
    uint32_t pres;        // Pascals
    int16_t  temp;        // centidegrees
};
#pragma pack()
unsigned char recv[sizeof(struct packet)];
packet *data;

void onReceive(int packetSize) {

  int toRead = packetSize >= sizeof(struct packet)
             ? sizeof(struct packet) : packetSize;

  for (int i = 0; i < toRead; i++) {
    recv[i] = LoRa.read();
  }

  data = (packet *)recv;

  // Serial.print(data->seq_no);
  // Serial.print(" | Altitude:");
  // Serial.print(data->bmp_alt);
  // Serial.print(" Temp:");
  // Serial.print(data->bmp_temp);

  // vehicle ID, state, internal clock, lat, lng, sats, GPS time, temp, pres, SNR

  Serial.print(data->vid);
  Serial.print(", ");
  Serial.print(data->seq_no);
  Serial.print(", ");
  Serial.print(data->state);
  Serial.print(", ");
  Serial.print(data->time_ms);
  Serial.print(", ");
  Serial.print(data->lat);
  Serial.print(", ");
  Serial.print(data->lng);
  Serial.print(", ");
  Serial.print(data->sat);
  Serial.print(", ");
  Serial.print(data->time_utc[0]);
  Serial.print(":");
  Serial.print(data->time_utc[1]);
  Serial.print(":");
  Serial.print(data->time_utc[2]);

  Serial.print(", ");
  Serial.print(data->temp);
  Serial.print(", ");
  Serial.print(data->pres);

  Serial.print(", ");
  Serial.println(LoRa.packetSnr());
}

void setup() {
  Serial.begin(115200);

  while (!Serial)
    ;


  Serial.println("LoRa Receiver Callback");
  Serial.println(sizeof(packet));

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
  LoRa.setSyncWord(0x89);
  // // Set a relatively wide 250kHz bandwidth
  LoRa.setSignalBandwidth(250E3);
  LoRa.setSpreadingFactor(9);
  LoRa.setCodingRate4(8);

  Serial.println("LoRa Initializing OK!");

  // put the radio into receive mode
  LoRa.receive();
}

bool toggle = true;

void loop() {
    toggle = !toggle;
    digitalWrite(LED,toggle);
    int packetSize = LoRa.parsePacket();
    Serial.println(packetSize);
    if (packetSize) {
        onReceive(packetSize);
    }
    delay(50);
}
