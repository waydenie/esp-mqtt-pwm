#define ESP_I2C_ADDRESS 0x57
#define PWM_I2C_ADDRESS 0x40
#define RTC_I2C_ADDRESS 0x68
#define MEM_I2C_ADDRESS 0x70

#define PIN_I2C_SDA 0
#define PIN_I2C_SCL 2

#define MODE_CLOCK     1
#define MODE_COUNTDOWN 2
#define MODE_GAUGES    3

volatile int current_mode = MODE_CLOCK;
volatile bool process_clocktick = false;
volatile time_t lastTime=0;

char MAC_char[18];

char MQTTClientName[20];
char* MQTTTopicName;

#define CONFIG_LEN 32
typedef struct {
  char wifi_ssid[CONFIG_LEN];
  char wifi_pwd[CONFIG_LEN];
  char admin_pwd[CONFIG_LEN];
  char mqtt_server[CONFIG_LEN];
  time_t cntdwn_clock1;
  time_t cntdwn_clock2;
} Config;

// 20250501T0000 => 1746057600 : "Early" Retirement Date
// 20160101T0000 => 1451606400 : New Years 2016
Config C; // = {"WhyFi","test!123","test!123","iot.eclipse.org",1746057600,1451606400};

//const char* server = "204.187.102.78";


